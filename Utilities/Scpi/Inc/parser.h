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
 * @file   scpi_parser.h
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser implementation
 *
 *
 */

#ifndef SCPI_PARSER_H
#define	SCPI_PARSER_H

#include <string.h>
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif
    void SCPI_Init(scpi_t * context);

    scpi_bool_t SCPI_Input(scpi_t * context, const char * data, int len);
    scpi_bool_t SCPI_Parse(scpi_t * context, char * data, int len);

    UINT32 SCPI_ResultCharacters(scpi_t * context, const char * data, UINT32 len);
#define SCPI_ResultMnemonic(context, data) SCPI_ResultCharacters((context), (data), strlen(data))
    UINT32 SCPI_ResultUInt32Base(scpi_t * context, UINT32 val, INT8 base);
    UINT32 SCPI_ResultInt32(scpi_t * context, INT32 val);
    UINT32 SCPI_ResultUInt64Base(scpi_t * context, UINT64 val, INT8 base);
    UINT32 SCPI_ResultInt64(scpi_t * context, INT64 val);
    UINT32 SCPI_ResultFloat(scpi_t * context, float val);
    UINT32 SCPI_ResultDouble(scpi_t * context, double val);
    UINT32 SCPI_ResultText(scpi_t * context, const char * data);
    UINT32 SCPI_ResultArbitraryBlock(scpi_t * context, const char * data, UINT32 len);
    UINT32 SCPI_ResultBool(scpi_t * context, scpi_bool_t val);

    scpi_bool_t SCPI_Parameter(scpi_t * context, scpi_parameter_t * parameter, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamIsValid(scpi_parameter_t * parameter);
    scpi_bool_t SCPI_ParamErrorOccurred(scpi_t * context);
    scpi_bool_t SCPI_ParamIsNumber(scpi_parameter_t * parameter, scpi_bool_t suffixAllowed);
    scpi_bool_t SCPI_ParamToInt32(scpi_t * context, scpi_parameter_t * parameter, INT32 * value);
    scpi_bool_t SCPI_ParamToUInt32(scpi_t * context, scpi_parameter_t * parameter, UINT32 * value);
    scpi_bool_t SCPI_ParamToInt64(scpi_t * context, scpi_parameter_t * parameter, INT64 * value);
    scpi_bool_t SCPI_ParamToUInt64(scpi_t * context, scpi_parameter_t * parameter, UINT64 * value);
    scpi_bool_t SCPI_ParamToFloat(scpi_t * context, scpi_parameter_t * parameter, float * value);
    scpi_bool_t SCPI_ParamToDouble(scpi_t * context, scpi_parameter_t * parameter, double * value);
    scpi_bool_t SCPI_ParamToChoice(scpi_t * context, scpi_parameter_t * parameter, const scpi_choice_def_t * options, INT32 * value);
    scpi_bool_t SCPI_ChoiceToName(const scpi_choice_def_t * options, INT32 tag, const char ** text);

    scpi_bool_t SCPI_ParamInt32(scpi_t * context, INT32 * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamUInt32(scpi_t * context, UINT32 * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamInt64(scpi_t * context, INT64 * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamUInt64(scpi_t * context, UINT64 * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamFloat(scpi_t * context, float * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamDouble(scpi_t * context, double * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamCharacters(scpi_t * context, const char ** value, UINT32 * len, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamArbitraryBlock(scpi_t * context, const char ** value, UINT32 * len, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamCopyText(scpi_t * context, char * buffer, UINT32 buffer_len, UINT32 * copy_len, scpi_bool_t mandatory);

    scpi_bool_t SCPI_ParamBool(scpi_t * context, scpi_bool_t * value, scpi_bool_t mandatory);
    scpi_bool_t SCPI_ParamChoice(scpi_t * context, const scpi_choice_def_t * options, INT32 * value, scpi_bool_t mandatory);

    scpi_bool_t SCPI_IsCmd(scpi_t * context, const char * cmd);
#if USE_COMMAND_TAGS
    INT32 SCPI_CmdTag(scpi_t * context);
#endif /* USE_COMMAND_TAGS */
    scpi_bool_t SCPI_Match(const char * pattern, const char * value, UINT32 len);
    scpi_bool_t SCPI_CommandNumbers(scpi_t * context, INT32 * numbers, UINT32 len, INT32 default_value);

#if USE_DEPRECATED_FUNCTIONS
    // deprecated finction, should be removed later
#define SCPI_ResultIntBase(context, val, base) SCPI_ResultInt32Base ((context), (val), (base), TRUE)
#define SCPI_ResultInt(context, val) SCPI_ResultInt32 ((context), (val))
#define SCPI_ParamToInt(context, parameter, value) SCPI_ParamToInt32((context), (parameter), (value))
#define SCPI_ParamToUnsignedInt(context, parameter, value) SCPI_ParamToUInt32((context), (parameter), (value))
#define SCPI_ParamInt(context, value, mandatory) SCPI_ParamInt32((context), (value), (mandatory))
#define SCPI_ParamUnsignedInt(context, value, mandatory) SCPI_ParamUInt32((context), (value), (mandatory))
#endif /* USE_DEPRECATED_FUNCTIONS */

#ifdef	__cplusplus
}
#endif

#endif	/* SCPI_PARSER_H */

