/*-
 * Copyright (c) 2012-2013 Jan Breuer,
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
 * @file   scpi_utils.h
 * @date   Thu Nov 15 10:58:45 UTC 2012
 * 
 * @brief  Conversion routines and string manipulation routines
 * 
 * 
 */

#ifndef SCPI_UTILS_PRIVATE_H
#define	SCPI_UTILS_PRIVATE_H

//#include <stdint.h>
#include "config.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define LOCAL __attribute__((visibility ("hidden")))
#else
#define LOCAL
#endif

    char * strnpbrk(const char *str, UINT32 size, const char *set) LOCAL;
    scpi_bool_t compareStr(const char * str1, UINT32 len1, const char * str2, UINT32 len2) LOCAL;
    scpi_bool_t compareStrAndNum(const char * str1, UINT32 len1, const char * str2, UINT32 len2, INT32 * num) LOCAL;
    UINT32 UInt32ToStrBaseSign(UINT32 val, char * str, UINT32 len, INT8 base, scpi_bool_t sign) LOCAL;
    UINT32 UInt64ToStrBaseSign(UINT64 val, char * str, UINT32 len, INT8 base, scpi_bool_t sign) LOCAL;
    UINT32 strBaseToInt32(const char * str, INT32 * val, INT8 base) LOCAL;
    UINT32 strBaseToUInt32(const char * str, UINT32 * val, INT8 base) LOCAL;
    UINT32 strBaseToInt64(const char * str, INT64 * val, INT8 base) LOCAL;
    UINT32 strBaseToUInt64(const char * str, UINT64 * val, INT8 base) LOCAL;
    UINT32 strToFloat(const char * str, float * val) LOCAL;
    UINT32 strToDouble(const char * str, double * val) LOCAL;
    scpi_bool_t locateText(const char * str1, UINT32 len1, const char ** str2, UINT32 * len2) LOCAL;
    scpi_bool_t locateStr(const char * str1, UINT32 len1, const char ** str2, UINT32 * len2) LOCAL;
    UINT32 skipWhitespace(const char * cmd, UINT32 len) LOCAL;
    scpi_bool_t matchPattern(const char * pattern, UINT32 pattern_len, const char * str, UINT32 str_len, INT32 * num) LOCAL;
    scpi_bool_t matchCommand(const char * pattern, const char * cmd, UINT32 len, INT32 *numbers, UINT32 numbers_len, INT32 default_value) LOCAL;
    scpi_bool_t composeCompoundCommand(const scpi_token_t * prev, scpi_token_t * current) LOCAL;

#if !HAVE_STRNLEN
    UINT32 BSD_strnlen(const char *s, UINT32 maxlen) LOCAL;
#endif

#if !HAVE_STRNCASECMP && !HAVE_STRNICMP
    int OUR_strncasecmp(const char *s1, const char *s2, UINT32 n) LOCAL;
#endif

#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#if 0
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif

#ifdef	__cplusplus
}
#endif

#endif	/* SCPI_UTILS_PRIVATE_H */

