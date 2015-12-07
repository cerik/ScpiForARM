/*-
 * Copyright (c) 2012-2015 Jan Breuer,
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
 * @file   utils.h
 * 
 * @brief  Conversion routines and string manipulation routines
 * 
 * 
 */

#ifndef SCPI_UTILS_H
#define	SCPI_UTILS_H

//#include <stdint.h>
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

    UINT32 SCPI_UInt32ToStrBase(UINT32 val, char * str, UINT32 len, INT8 base);
    UINT32 SCPI_Int32ToStr(INT32 val, char * str, UINT32 len);
    UINT32 SCPI_UInt64ToStrBase(UINT64 val, char * str, UINT32 len, INT8 base);
    UINT32 SCPI_Int64ToStr(INT64 val, char * str, UINT32 len);
    UINT32 SCPI_FloatToStr(float val, char * str, UINT32 len);
    UINT32 SCPI_DoubleToStr(double val, char * str, UINT32 len);
    UINT32 strnlen(const char *s, UINT32 max);

    // deprecated finction, should be removed later
#define SCPI_LongToStr(val, str, len, base) SCPI_Int32ToStr((val), (str), (len), (base), TRUE)

#ifdef	__cplusplus
}
#endif

#endif	/* SCPI_UTILS_H */

