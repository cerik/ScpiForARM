/*-
 * Copyright (c) 2013 Jan Breuer
 *                    Richard.hmm
 * Copyright (c) 2012 Jan Breuer
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi_utils.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  Conversion routines and string manipulation routines
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils_private.h"
#include "utils.h"

static UINT32 patternSeparatorShortPos(const char * pattern, UINT32 len);
static UINT32 patternSeparatorPos(const char * pattern, UINT32 len);
static UINT32 cmdSeparatorPos(const char * cmd, UINT32 len);

/**
 * Find the first occurrence in str of a character in set.
 * @param str
 * @param size
 * @param set
 * @return
 */
char * strnpbrk(const char *str, UINT32 size, const char *set) {
    const char *scanp;
    long c, sc;
    const char * strend = str + size;

    while ((strend != str) && ((c = *str++) != 0)) {
        for (scanp = set; (sc = *scanp++) != '\0';)
            if (sc == c)
                return ((char *) (str - 1));
    }
    return (NULL);
}

#if 0
#define FLT_MAX 1E+37
#define ERANGE 34
float strtof(const char *s00, char **se)
{
    double d;

    d = strtod(s00, se);
    if (d > FLT_MAX) 
    {
        return (FLT_MAX);
    } else if (d < -FLT_MAX)
    {
        return (-FLT_MAX);
    }
    return ((float) d);
}
#endif

/* just like strlen(3), but cap the number of bytes we count */
UINT32 strnlen(const char *s, UINT32 max) {
    const char *p;
    for(p = s; *p && max--; ++p);
    return(p - s);
}


#ifdef _MSC_VER
int strcasecmp(const char *s1,const char *s2)
{
   while  (toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
       if (*s1++ == '\0') return 0;
   return(toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

int strncasecmp(const char *s1, const char *s2, int n)
{
  while (--n >= 0 && toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
      if (*s1++ == '\0')  return 0;
  return(n < 0 ? 0 : toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

#endif

/**
 * Converts signed/unsigned 32 bit integer value to string in specific base
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @param base  output base
 * @param sign
 * @return number of bytes written to str (without '\0')
 */
UINT32 UInt32ToStrBaseSign(UINT32 val, char * str, UINT32 len, INT8 base, scpi_bool_t sign) {
    const char digits[] = "0123456789ABCDEF";

#define ADD_CHAR(c) if (pos < len) str[pos++] = (c)
    UINT32 x = 0;
    INT16 digit;
    UINT32 pos = 0;
    UINT32 uval = val;

    if (uval == 0) {
        ADD_CHAR('0');
    } else {

        switch (base) {
            case 2:
                x = 0x80000000L;
                break;
            case 8:
                x = 0x40000000L;
                break;
            case 10:
                x = 1000000000L;
                break;
            case 16:
                x = 0x10000000L;
                break;
            default:
                x = 1000000000L;
                base = 10;
                break;
        }

        // add sign for numbers in base 10
        if (sign && ((INT32) val < 0) && (base == 10)) {
            uval = -val;
            ADD_CHAR('-');
        }

        // remove leading zeros
        while ((uval / x) == 0) {
            x /= base;
        }

        do {
            digit = (UINT8) (uval / x);
            ADD_CHAR(digits[digit]);
            uval -= digit * x;
            x /= base;
        } while (x && (pos < len));
    }

    if (pos < len) str[pos] = 0;
    return pos;
#undef ADD_CHAR
}

/**
 * Converts signed 32 bit integer value to string
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_Int32ToStr(INT32 val, char * str, UINT32 len) {
    return UInt32ToStrBaseSign((UINT32) val, str, len, 10, TRUE);
}

/**
 * Converts unsigned 32 bit integer value to string in specific base
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @param base  output base
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_UInt32ToStrBase(UINT32 val, char * str, UINT32 len, INT8 base) {
    return UInt32ToStrBaseSign(val, str, len, base, FALSE);
}

/**
 * Converts signed/unsigned 64 bit integer value to string in specific base
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @param base  output base
 * @param sign
 * @return number of bytes written to str (without '\0')
 */
UINT32 UInt64ToStrBaseSign(UINT64 val, char * str, UINT32 len, INT8 base, scpi_bool_t sign) {
    const char digits[] = "0123456789ABCDEF";

#define ADD_CHAR(c) if (pos < len) str[pos++] = (c)
    UINT64 x = 0;
    INT16 digit;
    UINT32 pos = 0;
    UINT64 uval = val;

    if (uval == 0) {
        ADD_CHAR('0');
    } else {
        switch (base) {
            case 2:
                x = 0x8000000000000000ULL;
                break;
            case 8:
                x = 0x8000000000000000ULL;
                break;
            case 10:
                x = 10000000000000000000ULL;
                break;
            case 16:
                x = 0x1000000000000000ULL;
                break;
            default:
                x = 10000000000000000000ULL;
                base = 10;
                break;
        }

        // add sign for numbers in base 10
        if (sign && ((INT64) val < 0) && (base == 10)) {
            uval = -val;
            ADD_CHAR('-');
        }

        // remove leading zeros
        while ((uval / x) == 0) {
            x /= base;
        }

        do {
            digit = (UINT8) (uval / x);
            ADD_CHAR(digits[digit]);
            uval -= digit * x;
            x /= base;
        } while (x && (pos < len));
    }

    if (pos < len) str[pos] = 0;
    return pos;
#undef ADD_CHAR
}

/**
 * Converts signed 64 bit integer value to string
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_Int64ToStr(INT64 val, char * str, UINT32 len) {
    return UInt64ToStrBaseSign((UINT64) val, str, len, 10, TRUE);
}

/**
 * Converts signed/unsigned 64 bit integer value to string in specific base
 * @param val   integer value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @param base  output base
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_UInt64ToStrBase(UINT64 val, char * str, UINT32 len, INT8 base) {
    return UInt64ToStrBaseSign(val, str, len, base, FALSE);
}

/**
 * Converts float (32 bit) value to string
 * @param val   long value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_FloatToStr(float val, char * str, UINT32 len) {
    return SCPIDEFINE_floatToStr(val, str, len);
}

/**
 * Converts double (64 bit) value to string
 * @param val   double value
 * @param str   converted textual representation
 * @param len   string buffer length
 * @return number of bytes written to str (without '\0')
 */
UINT32 SCPI_DoubleToStr(double val, char * str, UINT32 len) {
    return SCPIDEFINE_doubleToStr(val, str, len);
}

/**
 * Converts string to signed 32bit integer representation
 * @param str   string value
 * @param val   32bit integer result
 * @return      number of bytes used in string
 */
UINT32 strBaseToInt32(const char * str, INT32 * val, INT8 base) {
    char * endptr;
    *val = strtol(str, &endptr, base);
    return endptr - str;
}

/**
 * Converts string to unsigned 32bit integer representation
 * @param str   string value
 * @param val   32bit integer result
 * @return      number of bytes used in string
 */
UINT32 strBaseToUInt32(const char * str, UINT32 * val, INT8 base) {
    char * endptr;
    *val = strtoul(str, &endptr, base);
    return endptr - str;
}

/**
 * Converts string to signed 64bit integer representation
 * @param str   string value
 * @param val   64bit integer result
 * @return      number of bytes used in string
 */
UINT32 strBaseToInt64(const char * str, INT64 * val, INT8 base) {
    char * endptr;
    *val = strtol(str, &endptr, base);//*val = strtoll(str, &endptr, base);
    return endptr - str;
}

/**
 * Converts string to unsigned 64bit integer representation
 * @param str   string value
 * @param val   64bit integer result
 * @return      number of bytes used in string
 */
UINT32 strBaseToUInt64(const char * str, UINT64 * val, INT8 base) {
    char * endptr;
    *val = strtoul(str, &endptr, base);//*val = strtoull(str, &endptr, base);
    return endptr - str;
}

/**
 * Converts string to float (32 bit) representation
 * @param str   string value
 * @param val   float result
 * @return      number of bytes used in string
 */
UINT32 strToFloat(const char * str, float * val) {
    char * endptr;
    *val = strtof(str, &endptr);
    return endptr - str;
}

/**
 * Converts string to double (64 bit) representation
 * @param str   string value
 * @param val   double result
 * @return      number of bytes used in string
 */
UINT32 strToDouble(const char * str, double * val) {
    char * endptr;
    *val = strtod(str, &endptr);
    return endptr - str;
}

/**
 * Compare two strings with exact length
 * @param str1
 * @param len1
 * @param str2
 * @param len2
 * @return TRUE if len1==len2 and "len" characters of both strings are equal
 */
scpi_bool_t compareStr(const char * str1, UINT32 len1, const char * str2, UINT32 len2) {
    if (len1 != len2) { 
        return FALSE;
    }

    if (SCPIDEFINE_strncasecmp(str1, str2, len2) == 0) {
        return TRUE;
    }

    return FALSE;
}

/**
 * Compare two strings, one be longer but may contains only numbers in that section
 * @param str1
 * @param len1
 * @param str2
 * @param len2
 * @return TRUE if strings match
 */
scpi_bool_t compareStrAndNum(const char * str1, UINT32 len1, const char * str2, UINT32 len2, INT32 * num) {
    scpi_bool_t result = FALSE;
    UINT32 i;

    if (len2 < len1) {
        return FALSE;
    }

    if (SCPIDEFINE_strncasecmp(str1, str2, len1) == 0) {
        result = TRUE;

        if (num) {
            if (len1 == len2) {
                //*num = 1;
            } else {
                INT32 tmpNum;
                i = len1 + strBaseToInt32(str2 + len1, &tmpNum, 10);
                if (i != len2) {
                    result = FALSE;
                } else {
                    *num = tmpNum;
                }
            }
        } else {
            for (i = len1; i < len2; i++) {
                if (!isdigit((int) str2[i])) {
                    result = FALSE;
                    break;
                }
            }
        }
    }

    return result;
}

/**
 * Count white spaces from the beggining
 * @param cmd - command
 * @param len - max search length
 * @return number of white spaces
 */
UINT32 skipWhitespace(const char * cmd, UINT32 len) {
    UINT32 i;
    for (i = 0; i < len; i++) {
        if (!isspace((unsigned char) cmd[i])) {
            return i;
        }
    }
    return len;
}

/**
 * Pattern is composed from upper case an lower case letters. This function
 * search the first lowercase letter
 * @param pattern
 * @param len - max search length
 * @return position of separator or len
 */
static UINT32 patternSeparatorShortPos(const char * pattern, UINT32 len) {
    UINT32 i;
    for (i = 0; (i < len) && pattern[i]; i++) {
        if (islower((unsigned char) pattern[i])) {
            return i;
        }
    }
    return i;
}

/**
 * Find pattern separator position
 * @param pattern
 * @param len - max search length
 * @return position of separator or len
 */
static UINT32 patternSeparatorPos(const char * pattern, UINT32 len) {

    char * separator = strnpbrk(pattern, len, "?:[]");
    if (separator == NULL) {
        return len;
    } else {
        return separator - pattern;
    }
}

/**
 * Find command separator position
 * @param cmd - input command
 * @param len - max search length
 * @return position of separator or len
 */
static UINT32 cmdSeparatorPos(const char * cmd, UINT32 len) {
    char * separator = strnpbrk(cmd, len, ":?");
    UINT32 result;
    if (separator == NULL) {
        result = len;
    } else {
        result = separator - cmd;
    }

    return result;
}

/**
 * Match pattern and str. Pattern is in format UPPERCASElowercase
 * @param pattern
 * @param pattern_len
 * @param str
 * @param str_len
 * @return
 */
scpi_bool_t matchPattern(const char * pattern, UINT32 pattern_len, const char * str, UINT32 str_len, INT32 * num) {
    UINT32 pattern_sep_pos_short;
    UINT32 new_pattern_len;

    if ((pattern_len > 0) && pattern[pattern_len - 1] == '#') {
        new_pattern_len = pattern_len - 1;
        pattern_sep_pos_short = patternSeparatorShortPos(pattern, new_pattern_len);
        return compareStrAndNum(pattern, new_pattern_len,       str, str_len, num) ||
               compareStrAndNum(pattern, pattern_sep_pos_short, str, str_len, num);
    } else {
        pattern_sep_pos_short = patternSeparatorShortPos(pattern, pattern_len);
        return compareStr(pattern, pattern_len,           str, str_len) ||
               compareStr(pattern, pattern_sep_pos_short, str, str_len);
    }
}

/**
 * Compare pattern and command
 * @param pattern eg. [:MEASure]:VOLTage:DC?
 * @param cmd - command
 * @param len - max search length
 * @return TRUE if pattern matches, FALSE otherwise
 */
scpi_bool_t matchCommand(const char * pattern, const char * cmd, UINT32 len, INT32 *numbers, UINT32 numbers_len, INT32 default_value) {
#define SKIP_PATTERN(n) do {pattern_ptr += (n);  pattern_len -= (n);} while(0)
#define SKIP_CMD(n) do {cmd_ptr += (n);  cmd_len -= (n);} while(0)

    scpi_bool_t result = FALSE;
    int brackets = 0;
    int cmd_sep_pos = 0;

    UINT32 numbers_idx = 0;
    INT32 *number_ptr = NULL;

    const char * pattern_ptr = pattern;
    int pattern_len = strlen(pattern);

    const char * cmd_ptr = cmd;
    UINT32 cmd_len = SCPIDEFINE_strnlen(cmd, len);

    /* both commands are query commands? */
    if (pattern_ptr[pattern_len - 1] == '?') {
        if (cmd_ptr[cmd_len - 1] == '?') {
            cmd_len -= 1;
            pattern_len -= 1;
        } else {
            return FALSE;
        }
    }

    /* now support optional keywords in pattern style, e.g. [:MEASure]:VOLTage:DC? */
    if (pattern_ptr[0] == '[') { // skip first '['
        SKIP_PATTERN(1);
        brackets++;
    }
    if (pattern_ptr[0] == ':') { // skip first ':'
        SKIP_PATTERN(1);
    }

    if (cmd_ptr[0] == ':') {
        /* handle errornouse ":*IDN?" */
        if (cmd_len >= 2) {
            if (cmd_ptr[1] != '*') {
                SKIP_CMD(1);
            } else {
                return FALSE;
            }
        }
    }

    while (1) {
        int pattern_sep_pos = patternSeparatorPos(pattern_ptr, pattern_len);

        cmd_sep_pos = cmdSeparatorPos(cmd_ptr, cmd_len);

        if ((pattern_sep_pos > 0) && pattern_ptr[pattern_sep_pos - 1] == '#') {
            if (numbers && (numbers_idx < numbers_len)) {
                number_ptr = numbers + numbers_idx;
                *number_ptr = default_value; // default value
            } else {
                number_ptr = NULL;
            }
            numbers_idx++;
        } else {
            number_ptr = NULL;
        }

        if (matchPattern(pattern_ptr, pattern_sep_pos, cmd_ptr, cmd_sep_pos, number_ptr)) {
            SKIP_PATTERN(pattern_sep_pos);
            SKIP_CMD(cmd_sep_pos);
            result = TRUE;

            /* command is complete */
            if ((pattern_len == 0) && (cmd_len == 0)) {
                break;
            }

            /* pattern complete, but command not */
            if ((pattern_len == 0) && (cmd_len > 0)) {
                result = FALSE;
                break;
            }

            /* command complete, but pattern not */
            if (cmd_len == 0) {
                // verify all subsequent pattern parts are also optional
                while (pattern_len) {
                    pattern_sep_pos = patternSeparatorPos(pattern_ptr, pattern_len);
                    switch (pattern_ptr[pattern_sep_pos]) {
                        case '[':
                            brackets++;
                            break;
                        case ']':
                            brackets--;
                            break;
                        default:
                            break;
                    }
                    SKIP_PATTERN(pattern_sep_pos + 1);
                    if (brackets == 0) {
                        if ((pattern_len > 0) && (pattern_ptr[0] == '[')) {
                            continue;
                        } else {
                            break;
                        }
                    }
                }
                if (pattern_len != 0) {
                    result = FALSE;
                }
                break; /* exist optional keyword, command is complete */
            }

            /* both command and patter contains command separator at this position */
            if ((pattern_len > 0)
                    && ((pattern_ptr[0] == cmd_ptr[0])
                    && (pattern_ptr[0] == ':'))) {
                SKIP_PATTERN(1);
                SKIP_CMD(1);
            } else if ((pattern_len > 1)
                    && (pattern_ptr[1] == cmd_ptr[0])
                    && (pattern_ptr[0] == '[')
                    && (pattern_ptr[1] == ':')) {
                SKIP_PATTERN(2); // for skip '[' in "[:"
                SKIP_CMD(1);
                brackets++;
            } else if ((pattern_len > 1)
                    && (pattern_ptr[1] == cmd_ptr[0])
                    && (pattern_ptr[0] == ']')
                    && (pattern_ptr[1] == ':')) {
                SKIP_PATTERN(2); // for skip ']' in "]:"
                SKIP_CMD(1);
                brackets--;
            } else if ((pattern_len > 2)
                    && (pattern_ptr[2] == cmd_ptr[0])
                    && (pattern_ptr[0] == ']')
                    && (pattern_ptr[1] == '[')
                    && (pattern_ptr[2] == ':')) {
                SKIP_PATTERN(3); // for skip '][' in "][:"
                SKIP_CMD(1);
                //brackets++;
                //brackets--;
            } else {
                result = FALSE;
                break;
            }
        } else {
            SKIP_PATTERN(pattern_sep_pos);
            if ((pattern_ptr[0] == ']') && (pattern_ptr[1] == ':')) {
                SKIP_PATTERN(2); // for skip ']' in "]:" , pattern_ptr continue, while cmd_ptr remain unchanged
                brackets--;
            } else if ((pattern_len > 2) && (pattern_ptr[0] == ']')
                    && (pattern_ptr[1] == '[')
                    && (pattern_ptr[2] == ':')) {
                SKIP_PATTERN(3); // for skip ']' in "][:" , pattern_ptr continue, while cmd_ptr remain unchanged
                //brackets++;
                //brackets--;
            } else {
                result = FALSE;
                break;
            }
        }
    }

    return result;
#undef SKIP_PATTERN
#undef SKIP_CMD
}

/**
 * Compose command from previous command anc current command
 *
 * @param prev pointer to previous command
 * @param current pointer of current command
 *
 * prev and current should be in the same memory buffer
 */
scpi_bool_t composeCompoundCommand(const scpi_token_t * prev, scpi_token_t * current) {
    UINT32 i;

    /* Invalid input */
    if (current == NULL || current->ptr == NULL || current->len == 0)
        return FALSE;

    /* no previous command - nothing to do*/
    if (prev->ptr == NULL || prev->len == 0)
        return TRUE;

    /* Common command or command root - nothing to do */
    if (current->ptr[0] == '*' || current->ptr[0] == ':')
        return TRUE;

    /* Previsou command was common command - nothing to do */
    if (prev->ptr[0] == '*')
        return TRUE;

    /* Find last occurence of ':' */
    for (i = prev->len; i > 0; i--) {
        if (prev->ptr[i - 1] == ':') {
            break;
        }
    }

    /* Previous command was simple command - nothing to do*/
    if (i == 0)
        return TRUE;

    current->ptr -= i;
    current->len += i;
    memmove(current->ptr, prev->ptr, i);
    return TRUE;
}



#if !HAVE_STRNLEN
/* use FreeBSD strnlen */

/*-
 * Copyright (c) 2009 David Schultz <das@FreeBSD.org>
 * All rights reserved.
 */
UINT32
BSD_strnlen(const char *s, UINT32 maxlen) {
    UINT32 len;

    for (len = 0; len < maxlen; len++, s++) {
        if (!*s)
            break;
    }
    return (len);
}
#endif

#if !HAVE_STRNCASECMP && !HAVE_STRNICMP

int OUR_strncasecmp(const char *s1, const char *s2, UINT32 n) {
    unsigned char c1, c2;

    for (; n != 0; n--) {
        c1 = tolower((unsigned char) *s1++);
        c2 = tolower((unsigned char) *s2++);
        if (c1 != c2) {
            return c1 - c2;
        }
        if (c1 == '\0') {
            return 0;
        }
    }
    return 0;
}
#endif

