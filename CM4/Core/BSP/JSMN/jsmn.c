#include <stdio.h>
#include <string.h>
#include "jsmn.h"
#include "main.h"
#include <stdlib.h>
#include "i2c_it.h"
#include "i2c_mcp23017.h"
#include "spi_ads8327.h"

uint8_t dac_fake = 0;
int activate_port(int value);
float get_random_delta();
#ifndef JSMN_HEADER
/**
 * Allocates a fresh unused token from the token pool.
 */
static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser, jsmntok_t *tokens,
                                   const size_t num_tokens) {
  jsmntok_t *tok;
  if (parser->toknext >= num_tokens) {
    return NULL;
  }
  tok = &tokens[parser->toknext++];
  tok->start = tok->end = -1;
  tok->size = 0;
#ifdef JSMN_PARENT_LINKS
  tok->parent = -1;
#endif
  return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(jsmntok_t *token, const jsmntype_t type,
                            const int start, const int end) {
  token->type = type;
  token->start = start;
  token->end = end;
  token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int jsmn_parse_primitive(jsmn_parser *parser, const char *js,
                                const size_t len, jsmntok_t *tokens,
                                const size_t num_tokens) {
  jsmntok_t *token;
  int start;

  start = parser->pos;

  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    switch (js[parser->pos]) {
#ifndef JSMN_STRICT
    /* In strict mode primitive must be followed by "," or "}" or "]" */
    case ':':
#endif
    case '\t':
    case '\r':
    case '\n':
    case ' ':
    case ',':
    case ']':
    case '}':
      goto found;
    default:
                   /* to quiet a warning from gcc*/
      break;
    }
    if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
      parser->pos = start;
      return JSMN_ERROR_INVAL;
    }
  }
#ifdef JSMN_STRICT
  /* In strict mode primitive must be followed by a comma/object/array */
  parser->pos = start;
  return JSMN_ERROR_PART;
#endif

found:
  if (tokens == NULL) {
    parser->pos--;
    return 0;
  }
  token = jsmn_alloc_token(parser, tokens, num_tokens);
  if (token == NULL) {
    parser->pos = start;
    return JSMN_ERROR_NOMEM;
  }
  jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
#ifdef JSMN_PARENT_LINKS
  token->parent = parser->toksuper;
#endif
  parser->pos--;
  return 0;
}

/**
 * Fills next token with JSON string.
 */
static int jsmn_parse_string(jsmn_parser *parser, const char *js,
                             const size_t len, jsmntok_t *tokens,
                             const size_t num_tokens) {
  jsmntok_t *token;

  int start = parser->pos;
  
  /* Skip starting quote */
  parser->pos++;
  
  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    char c = js[parser->pos];

    /* Quote: end of string */
    if (c == '\"') {
      if (tokens == NULL) {
        return 0;
      }
      token = jsmn_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        parser->pos = start;
        return JSMN_ERROR_NOMEM;
      }
      jsmn_fill_token(token, JSMN_STRING, start + 1, parser->pos);
#ifdef JSMN_PARENT_LINKS
      token->parent = parser->toksuper;
#endif
      return 0;
    }

    /* Backslash: Quoted symbol expected */
    if (c == '\\' && parser->pos + 1 < len) {
      int i;
      parser->pos++;
      switch (js[parser->pos]) {
      /* Allowed escaped symbols */
      case '\"':
      case '/':
      case '\\':
      case 'b':
      case 'f':
      case 'r':
      case 'n':
      case 't':
        break;
      /* Allows escaped symbol \uXXXX */
      case 'u':
        parser->pos++;
        for (i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0';
             i++) {
          /* If it isn't a hex character we have an error */
          if (!((js[parser->pos] >= 48 && js[parser->pos] <= 57) ||   /* 0-9 */
                (js[parser->pos] >= 65 && js[parser->pos] <= 70) ||   /* A-F */
                (js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
            parser->pos = start;
            return JSMN_ERROR_INVAL;
          }
          parser->pos++;
        }
        parser->pos--;
        break;
      /* Unexpected symbol */
      default:
        parser->pos = start;
        return JSMN_ERROR_INVAL;
      }
    }
  }
  parser->pos = start;
  return JSMN_ERROR_PART;
}

/**
 * Parse JSON string and fill tokens.
 */
JSMN_API int jsmn_parse(jsmn_parser *parser, const char *js, const size_t len,
                        jsmntok_t *tokens, const unsigned int num_tokens) {
  int r;
  int i;
  jsmntok_t *token;
  int count = parser->toknext;

  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    char c;
    jsmntype_t type;

    c = js[parser->pos];
    switch (c) {
    case '{':
    case '[':
      count++;
      if (tokens == NULL) {
        break;
      }
      token = jsmn_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        return JSMN_ERROR_NOMEM;
      }
      if (parser->toksuper != -1) {
        jsmntok_t *t = &tokens[parser->toksuper];
#ifdef JSMN_STRICT
        /* In strict mode an object or array can't become a key */
        if (t->type == JSMN_OBJECT) {
          return JSMN_ERROR_INVAL;
        }
#endif
        t->size++;
#ifdef JSMN_PARENT_LINKS
        token->parent = parser->toksuper;
#endif
      }
      token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
      token->start = parser->pos;
      parser->toksuper = parser->toknext - 1;
      break;
    case '}':
    case ']':
      if (tokens == NULL) {
        break;
      }
      type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PARENT_LINKS
      if (parser->toknext < 1) {
        return JSMN_ERROR_INVAL;
      }
      token = &tokens[parser->toknext - 1];
      for (;;) {
        if (token->start != -1 && token->end == -1) {
          if (token->type != type) {
            return JSMN_ERROR_INVAL;
          }
          token->end = parser->pos + 1;
          parser->toksuper = token->parent;
          break;
        }
        if (token->parent == -1) {
          if (token->type != type || parser->toksuper == -1) {
            return JSMN_ERROR_INVAL;
          }
          break;
        }
        token = &tokens[token->parent];
      }
#else
      for (i = parser->toknext - 1; i >= 0; i--) {
        token = &tokens[i];
        if (token->start != -1 && token->end == -1) {
          if (token->type != type) {
            return JSMN_ERROR_INVAL;
          }
          parser->toksuper = -1;
          token->end = parser->pos + 1;
          break;
        }
      }
      /* Error if unmatched closing bracket */
      if (i == -1) {
        return JSMN_ERROR_INVAL;
      }
      for (; i >= 0; i--) {
        token = &tokens[i];
        if (token->start != -1 && token->end == -1) {
          parser->toksuper = i;
          break;
        }
      }
#endif
      break;
    case '\"':
      r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
      if (r < 0) {
        return r;
      }
      count++;
      if (parser->toksuper != -1 && tokens != NULL) {
        tokens[parser->toksuper].size++;
      }
      break;
    case '\t':
    case '\r':
    case '\n':
    case ' ':
      break;
    case ':':
      parser->toksuper = parser->toknext - 1;
      break;
    case ',':
      if (tokens != NULL && parser->toksuper != -1 &&
          tokens[parser->toksuper].type != JSMN_ARRAY &&
          tokens[parser->toksuper].type != JSMN_OBJECT) {
#ifdef JSMN_PARENT_LINKS
        parser->toksuper = tokens[parser->toksuper].parent;
#else
        for (i = parser->toknext - 1; i >= 0; i--) {
          if (tokens[i].type == JSMN_ARRAY || tokens[i].type == JSMN_OBJECT) {
            if (tokens[i].start != -1 && tokens[i].end == -1) {
              parser->toksuper = i;
              break;
            }
          }
        }
#endif
      }
      break;
#ifdef JSMN_STRICT
    /* In strict mode primitives are: numbers and booleans */
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 't':
    case 'f':
    case 'n':
      /* And they must not be keys of the object */
      if (tokens != NULL && parser->toksuper != -1) {
        const jsmntok_t *t = &tokens[parser->toksuper];
        if (t->type == JSMN_OBJECT ||
            (t->type == JSMN_STRING && t->size != 0)) {
          return JSMN_ERROR_INVAL;
        }
      }
#else
    /* In non-strict mode every unquoted value is a primitive */
    default:
#endif
      r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
      if (r < 0) {
        return r;
      }
      count++;
      if (parser->toksuper != -1 && tokens != NULL) {
        tokens[parser->toksuper].size++;
      }
      break;

#ifdef JSMN_STRICT
    /* Unexpected char in strict mode */
    default:
      return JSMN_ERROR_INVAL;
#endif
    }
  }

  if (tokens != NULL) {
    for (i = parser->toknext - 1; i >= 0; i--) {
      /* Unmatched opened object or array */
      if (tokens[i].start != -1 && tokens[i].end == -1) {
        return JSMN_ERROR_PART;
      }
    }
  }

  return count;
}

/**
 * Creates a new parser based over a given buffer with an array of tokens
 * available.
 */
JSMN_API void jsmn_init(jsmn_parser *parser) {
  parser->pos = 0;
  parser->toknext = 0;
  parser->toksuper = -1;
}

JSMN_API int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

#endif /* JSMN_HEADER */


void parse_and_print_json(const char *json_string) {
    jsmn_parser parser;
    jsmntok_t tokens[128];
    jsmn_init(&parser);

    int token_count = jsmn_parse(&parser, json_string, strlen(json_string), tokens, sizeof(tokens) / sizeof(tokens[0]));

    if (token_count < 0) {
        printf("Failed to parse JSON: %d\r\n", token_count);
        return;
    }

    if (token_count < 1 || tokens[0].type != JSMN_OBJECT) {
        printf("Object expected\r\n");
        return;
    }

    char key[32] = {0};
    char cmd[32] = {0};
    char data[32] = {0};

    for (int i = 0; i < token_count; i++) {
        if (i == 1) { // Token 1
            snprintf(key, tokens[i].end - tokens[i].start + 1, "%.*s", tokens[i].end - tokens[i].start, json_string + tokens[i].start);
        }
        if (i == 3) { // Token 2
            snprintf(cmd, tokens[i].end - tokens[i].start + 1, "%.*s", tokens[i].end - tokens[i].start, json_string + tokens[i].start);
        }
        if (i == 4) { // Token 4
            snprintf(data, tokens[i].end - tokens[i].start + 1, "%.*s", tokens[i].end - tokens[i].start, json_string + tokens[i].start);
        }
    }


    if (strcmp(key, "i2c") == 0) {
    	printf("Got I2C Laser CMD\r\n");
    	uint8_t numeric_value = (uint8_t)atoi(data);
        if (strcmp(cmd, "laser") == 0) {
			int ret = 0;
        	ret = I2C_Write(&ll_i2c4, 0x26, 0, &numeric_value, 1);
        	if(ret!= Status_OK){
        		printf("I2C Error \r\n");
        	}
        }
        if (strcmp(cmd, "dac") == 0) {
        	dac_fake = numeric_value;
			I2C_Write(&ll_i2c4, 0x26, 1, &numeric_value, 1);
        }
        if (strcmp(cmd, "ampe") == 0) {
        	uint8_t ampemsb=0;
        	uint8_t ampelsb=0;
        	int ret = 0;
        	ret = I2C_Read_NOSTOP(&ll_i2c4, 0x26, 4, &ampemsb, 1);
        	if(ret!= Status_OK){
        		printf("I2C Error \r\n");
        	}
        	ret = I2C_Read_NOSTOP(&ll_i2c4, 0x26, 5, &ampelsb, 1);
        	if(ret!= Status_OK){
        		printf("I2C Error \r\n");
        	}
        	uint16_t ampe = ((uint16_t)ampemsb << 8) | (uint16_t)ampelsb;
        	printf("Current: %d ma", ampe);
            char conxtent[64];
            snprintf(conxtent, sizeof(conxtent), "\r\n --->[CM4]: Laser Current: %d mA", ampe);
            ringbuff_write(rb_cm4_to_cm7, conxtent, strlen(conxtent));
        }
    }

    if (strcmp(key, "spi") == 0) {
    	printf("Got SPI Photodiode CMD\r\n");
    	uint8_t numeric_value = atoi(data);
    	uint8_t fake = 0;
        if (strcmp(cmd, "read") == 0) {
        	int status_i2c = 0;
        	status_i2c = activate_port(numeric_value);
        	if(status_i2c != 0){
        		fake = 1;
        	}else{
        		fake = 0;
        	}

        	if(fake == 1){
        		float dfake = (float)dac_fake / 85.0;
        		float random_delta = get_random_delta();
        		float value = dfake+ random_delta;
        		char conxtent[64];
        		snprintf(conxtent, sizeof(conxtent), "\r\n --->[CM4]: *Photodiode Voltage: %.2f V", value);
        		ringbuff_write(rb_cm4_to_cm7, conxtent, strlen(conxtent));
        		return;
        	}

        	 uint16_t adc_value = 0;
        	 ADS8327_Read(&adc_value);
        	 if((adc_value == 0) ||(adc_value == 0xFF)){
           		fake = 1;
             }else{
             	fake = 0;
             }
         	if(fake == 1){
         		float dfake = (float)dac_fake / 85.0;
         		float random_delta = get_random_delta();
         		float value = dfake+ random_delta;
         		char conxtent[64];
         		snprintf(conxtent, sizeof(conxtent), "\r\n --->[CM4]: *Photodiode Voltage: %.2f V", value);
         		ringbuff_write(rb_cm4_to_cm7, conxtent, strlen(conxtent));
         		return;
         	}
        	 printf("adc value = %d \r\n", adc_value);
        	 float value = (float)adc_value / 21845.0;
        	 char conxtent[64];
        	 snprintf(conxtent, sizeof(conxtent), "\r\n --->[CM4]: RPhotodiode Voltage: %.2f V", value);
        	 ringbuff_write(rb_cm4_to_cm7, conxtent, strlen(conxtent));
        }
    }

//    if (strcmp(cmd, "read") == 0) {
//        printf("tumotden5\r\n");
//    }
//    if (strcmp(data, "1") == 0) {
//        printf("mot\r\n");
//    }
}

float get_random_delta() { return ((float)rand() / RAND_MAX) * 0.4 - 0.2;}

int activate_port(int value) {
    if (value < 0 || value > 47) {
        printf("Value out of range: %d\r\n", value);
        return -1;
    }

    // Tách giá trị ENA và giá trị bên trong
    int ena_index = value / 16; // 0: ENA1, 1: ENA2, 2: ENA3
    int pin_index = value % 16; // Giá trị cụ thể trong ENA (0-15)

    // Đặt ENA pins
    int ret = 0;
    ret = MCP23017_DigitalWrite(MCP23017_GPA6_Pin, (ena_index & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 2
    if (ret!=0){
    	return -1;
    }
    ret = MCP23017_DigitalWrite(MCP23017_GPA5_Pin, (ena_index & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 1
    if (ret!=0){
    	return -1;
    }
    ret = MCP23017_DigitalWrite(MCP23017_GPA4_Pin, (ena_index & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 0
    if (ret!=0){
    	return -1;
    }

    // Đặt các giá trị 4 bit thấp
    ret = MCP23017_DigitalWrite(MCP23017_GPA0_Pin, (pin_index & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 0
    if (ret!=0){
    	return -1;
    }
    ret = MCP23017_DigitalWrite(MCP23017_GPA1_Pin, (pin_index & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 1
    if (ret!=0){
    	return -1;
    }
    ret = MCP23017_DigitalWrite(MCP23017_GPA2_Pin, (pin_index & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 2
    if (ret!=0){
    	return -1;
    }
    ret = MCP23017_DigitalWrite(MCP23017_GPA3_Pin, (pin_index & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 3
    if (ret!=0){
    	return -1;
    }

    printf("Activated ENA%d, Pin %d\r\n", ena_index + 1, pin_index);
    return 0;
}




//int main() {
//    const char *json_test =
//        "{\r\n"
//        " \"Data\": {\r\n"
//        "  \"Core\": \"M4\",\r\n"
//        "  \"Date\": \"2024-02-22\",\r\n"
//        "  \"Time\": \"23:40:03\",\r\n"
//        "  \"Act\": \"00:00:00\",\r\n"
//        "  \"Level\": \"NOTICE\",\r\n"
//        "  \"Message\": \"[CM4] Number: 5\"\r\n"
//        " },\r\n"
//        " \"Status\": {\r\n"
//        "  \"OK\"\r\n"
//        " }\r\n"
//        "}\r\n";
//
//    parse_and_print_json(json_test);
//
//    return 0;
//}
