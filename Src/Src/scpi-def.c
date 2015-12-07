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
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 * 
 * @brief  SCPI parser test
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scpi.h"
#include "scpi-def.h"

static FLOAT32 gMatrix[5][5][5]={0.0};


UINT32 SCPI_Write(scpi_t * context, const char * data, UINT32 len) {
    (void) context;
    return fwrite(data, 1, len, stdout);
}

scpi_result_t SCPI_Flush(scpi_t * context) {
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, INT16 err) {
    (void) context;

    printf("**ERROR: %d, \"%s\"\r\n", (INT16) err, SCPI_ErrorTranslate(err));
    return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    if (SCPI_CTRL_SRQ == ctrl) {
        printf("**SRQ: 0x%X (%d)\r\n", val, val);
    } else {
        printf("**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
    }
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
    printf("**Reset\r\n");
    return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemCommTcpipControlQ(scpi_t * context) {
    return SCPI_RES_ERR;
}

static scpi_result_t DMM_MeasureVoltageDcQ(scpi_t * context) {
    scpi_number_t param1, param2;
    char bf[15];
    printf("meas:volt:dc\r\n"); // debug command name   

    // read first parameter if present
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param1, FALSE)) {
        // do something, if parameter not present
    }

    // read second paraeter if present
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param2, FALSE)) {
        // do something, if parameter not present
    }


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param1, bf, 15);
    printf("\tP1=%s\r\n", bf);


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param2, bf, 15);
    printf("\tP2=%s\r\n", bf);

    SCPI_ResultDouble(context, 0);

    return SCPI_RES_OK;
}

static scpi_result_t DMM_MeasureVoltageAcQ(scpi_t * context) {
    scpi_number_t param1, param2;
    char bf[15];
    printf("meas:volt:ac\r\n"); // debug command name   

    // read first parameter if present
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param1, FALSE)) {
        // do something, if parameter not present
    }

    // read second paraeter if present
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param2, FALSE)) {
        // do something, if parameter not present
    }


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param1, bf, 15);
    printf("\tP1=%s\r\n", bf);


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param2, bf, 15);
    printf("\tP2=%s\r\n", bf);

    SCPI_ResultDouble(context, 0);

    return SCPI_RES_OK;
}

static scpi_result_t DMM_ConfigureVoltageDc(scpi_t * context) {
    double param1, param2;
    printf("conf:volt:dc\r\n"); // debug command name   

    // read first parameter if present
    if (!SCPI_ParamDouble(context, &param1, TRUE)) {
        return SCPI_RES_ERR;
    }

    // read second paraeter if present
    if (!SCPI_ParamDouble(context, &param2, FALSE)) {
        // do something, if parameter not present
    }

    printf("\tP1=%lf\r\n", param1);
    printf("\tP2=%lf\r\n", param2);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Bool(scpi_t * context) {
    scpi_bool_t param1;
    printf("TEST:BOOL\r\n"); // debug command name   

    // read first parameter if present
    if (!SCPI_ParamBool(context, &param1, TRUE)) {
        return SCPI_RES_ERR;
    }

    printf("\tP1=%d\r\n", param1);

    return SCPI_RES_OK;
}

scpi_choice_def_t trigger_source[] = {
    {"BUS", 5},
    {"IMMediate", 6},
    {"EXTernal", 7},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

static scpi_result_t TEST_ChoiceQ(scpi_t * context) {

    INT32 param;
    const char * name;

    if (!SCPI_ParamChoice(context, trigger_source, &param, TRUE)) {
        return SCPI_RES_ERR;
    }

    SCPI_ChoiceToName(trigger_source, param, &name);
    printf("\tP1=%s (%ld)\r\n", name, (long int) param);

    SCPI_ResultInt32(context, param);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Numbers(scpi_t * context) {
    INT32 numbers[2];

    SCPI_CommandNumbers(context, numbers, 2, 1);

    printf("TEST numbers %d %d\r\n", numbers[0], numbers[1]);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Text(scpi_t * context) {
    char buffer[100];
    size_t copy_len;

    if (!SCPI_ParamCopyText(context, buffer, sizeof (buffer), &copy_len, FALSE)) {
        buffer[0] = '\0';
    }

    printf("TEXT: ***%s***\r\n", buffer);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_ArbQ(scpi_t * context) {
    const char * data;
    size_t len;

    SCPI_ParamArbitraryBlock(context, &data, &len, FALSE);

    SCPI_ResultArbitraryBlock(context, data, len);

    return SCPI_RES_OK;
}

struct _scpi_channel_value_t {
    INT32 row;
    INT32 col;
};
typedef struct _scpi_channel_value_t scpi_channel_value_t;

/**
 * @brief 
 * parses lists
 * channel numbers > 0.
 * no checks yet.
 * valid: (@1), (@3!1:1!3), ...
 * (@1!1:3!2) would be 1!1, 1!2, 2!1, 2!2, 3!1, 3!2.
 * (@3!1:1!3) would be 3!1, 3!2, 3!3, 2!1, 2!2, 2!3, ... 1!3.
 * 
 * @param channel_list channel list, compare to SCPI99 Vol 1 Ch. 8.3.2
 */
static scpi_result_t TEST_Chanlst(scpi_t *context) 
{
#define maxrow 2    //maximum number of rows
#define maxcol 6    //maximum number of columns
#define maxdim 2    //maximum number of dimensions

    scpi_parameter_t channel_list_param;
    scpi_channel_value_t array[maxrow * maxcol]; //array which holds values in order (2D)
    size_t chanlst_idx; //index for channel list
    size_t arr_idx = 0; //index for array
    size_t n, m = 1; //counters for row (n) and columns (m)

    // get channel list
    if (SCPI_Parameter(context, &channel_list_param, TRUE)) {
        scpi_bool_t is_range;
        INT32 values_from[maxdim];
        INT32 values_to[maxdim];
        size_t dimensions;

        boolean for_stop_row = FALSE; //TRUE if iteration for rows has to stop
        boolean for_stop_col = FALSE; //TRUE if iteration for columns has to stop
        INT32 dir_row = 1; //direction of counter for rows, +/-1
        INT32 dir_col = 1; //direction of counter for columns, +/-1

        // the next statement is valid usage and it gets only real number of dimensions for the first item (index 0)
        if (!SCPI_ExprChannelListEntry(context, &channel_list_param, 0, &is_range, NULL, NULL, 0, &dimensions)) {
            chanlst_idx = 0; //call first index
            arr_idx = 0; //set arr_idx to 0
            do { //if valid, iterate over channel_list_param index while res == valid (do-while cause we have to do it once)
                SCPI_ExprChannelListEntry(context, &channel_list_param, chanlst_idx, &is_range, values_from, values_to, 4, &dimensions);
                if (is_range == FALSE) { //still can have multiple dimensions
                    if (dimensions == 1) {
                        //here we have our values
                        //row == values_from[0]
                        //col == 0 (fixed number)
                        //call a function or something
                        array[arr_idx].row = values_from[0];
                        array[arr_idx].col = 0;
                    } else if (dimensions == 2) {
                        //here we have our values
                        //row == values_fom[0]
                        //col == values_from[1]
                        //call a function or something
                        array[arr_idx].row = values_from[0];
                        array[arr_idx].col = values_from[1];
                    } else {
                        return SCPI_RES_ERR;
                    }
                    arr_idx++; //inkrement array where we want to save our values to, not neccessary otherwise
                } else if (is_range == TRUE) {
                    if (values_from[0] > values_to[0]) {
                        dir_row = -1; //we have to decrement from values_from
                    } else { //if (values_from[0] < values_to[0])
                        dir_row = +1; //default, we increment from values_from
                    }

                    //iterating over rows, do it once -> set for_stop_row = FALSE
                    //needed if there is channel list index isn't at end yet
                    for_stop_row = FALSE;
                    for (n = values_from[0]; for_stop_row == FALSE; n += dir_row) {
                        //usual case for ranges, 2 dimensions
                        if (dimensions == 2) {
                            if (values_from[1] > values_to[1]) {
                                dir_col = -1;
                            } else if (values_from[1] < values_to[1]) {
                                dir_col = +1;
                            }
                            //iterating over columns, do it at least once -> set for_stop_col = FALSE
                            //needed if there is channel list index isn't at end yet
                            for_stop_col = FALSE;
                            for (m = values_from[1]; for_stop_col == FALSE; m += dir_col) {
                                //here we have our values
                                //row == n
                                //col == m
                                //call a function or something
                                array[arr_idx].row = n;
                                array[arr_idx].col = m;
                                arr_idx++;
                                if (m == (size_t)values_to[1]) {
                                    //endpoint reached, stop column for-loop
                                    for_stop_col = TRUE;
                                }
                            }
                            //special case for range, example: (@2!1)
                        } else if (dimensions == 1) {
                            //here we have values
                            //row == n
                            //col == 0 (fixed number)
                            //call function or sth.
                            array[arr_idx].row = n;
                            array[arr_idx].col = 0;
                            arr_idx++;
                        }
                        if (n == (size_t)values_to[0]) {
                            //endpoint reached, stop row for-loop
                            for_stop_row = TRUE;
                        }
                    }
                } else {
                    return SCPI_RES_ERR;
                }
                //increase index
                chanlst_idx++;
            } while (SCPI_EXPR_OK == SCPI_ExprChannelListEntry(context, &channel_list_param, chanlst_idx, &is_range, values_from, values_to, 4, &dimensions));
            //while checks, whether incremented index is valid
        }
        //do something at the end if needed
        array[arr_idx].row = 0;
        array[arr_idx].col = 0;
    }
    
    {
        size_t i;
        printf("TEST_Chanlst: ");
        for (i = 0; i< arr_idx; i++) {
            printf("%d!%d, ", array[i].row, array[i].col);
        }
        printf("\r\n");
    }
    return SCPI_RES_OK;
}

/**
 * Reimplement IEEE488.2 *TST?
 *
 * Result should be 0 if everything is ok
 * Result should be 1 if something goes wrong
 *
 * Return SCPI_RES_OK
 */
static scpi_result_t My_CoreTstQ(scpi_t * context) {

    SCPI_ResultInt32(context, 0);

    return SCPI_RES_OK;
}


//===============================================================
// Configure system database
// normal_matrix[arg1][arg2][arg3]=arg4
// arg1 & arg2 is interger
// arg4 is float32.
static scpi_result_t CFG_MatrixQ(scpi_t *context)
{
    scpi_number_t param;
    UINT8 i,j,k;
    
    // read first parameter if present
    if (SCPI_ParamNumber(context, scpi_special_numbers_def, &param, FALSE)) {
        i = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }

    // read second paraeter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        j = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }

    // read third paraeter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        k = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }
    SCPI_ResultDouble(context,gMatrix[i][j][k]);
    return SCPI_RES_OK;
}

static scpi_result_t CFG_Matrix(scpi_t *context)
{
    scpi_number_t param;
    FLOAT32 value;
    UINT8 i,j,k;
    
    // read first parameter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        i = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }

    // read second paraeter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        j = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }

    // read third paraeter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        k = (UINT8)param.value;
    }else{
        return SCPI_RES_ERR;
    }

    // read fourth paraeter if present
    if (SCPI_ParamNumber(context,scpi_special_numbers_def, &param, FALSE)) {
        value =(FLOAT32) param.value;
    }else{
        return SCPI_RES_ERR;
    }
    gMatrix[i][j][k]=value;
    printf("%s:%d --->[%d][%d][%d]=%f\r\n",__FUNCTION__,__LINE__,i,j,k,gMatrix[i][j][k]);
    return SCPI_RES_OK;
}

static const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { "*CLS",  SCPI_CoreCls,  0},
    { "*ESE",  SCPI_CoreEse,  0},
    { "*ESE?", SCPI_CoreEseQ, 0},
    { "*ESR?", SCPI_CoreEsrQ, 0},
    { "*IDN?", SCPI_CoreIdnQ, 0},
    { "*OPC",  SCPI_CoreOpc,  0},
    { "*OPC?", SCPI_CoreOpcQ, 0},
    { "*RST",  SCPI_CoreRst,  0},
    { "*SRE",  SCPI_CoreSre,  0},
    { "*SRE?", SCPI_CoreSreQ, 0},
    { "*STB?", SCPI_CoreStbQ, 0},
    { "*TST?", My_CoreTstQ,   0},
    { "*WAI",  SCPI_CoreWai,  0},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    { "SYSTem:ERRor[:NEXT]?", SCPI_SystemErrorNextQ,  0},
    { "SYSTem:ERRor:COUNt?",  SCPI_SystemErrorCountQ, 0},
    { "SYSTem:VERSion?",      SCPI_SystemVersionQ,    0},

  //{ "STATus:OPERation?", .         scpi_stub_callback, 0},
  //{ "STATus:OPERation:EVENt?",     scpi_stub_callback, 0},
  //{ "STATus:OPERation:CONDition?", scpi_stub_callback, 0},
  //{ "STATus:OPERation:ENABle",     scpi_stub_callback, 0},
  //{ "STATus:OPERation:ENABle?",    scpi_stub_callback, 0},

    { "STATus:QUEStionable[:EVENt]?",   SCPI_StatusQuestionableEventQ, 0},
  //{ "STATus:QUEStionable:CONDition?", scpi_stub_callback,            0},
    { "STATus:QUEStionable:ENABle",     SCPI_StatusQuestionableEnable, 0},
    { "STATus:QUEStionable:ENABle?",    SCPI_StatusQuestionableEnableQ,0},

    { "STATus:PRESet",                  SCPI_StatusPreset,             0},

    /* CONF */
    { "CONFigure:MATrix",          CFG_Matrix,             0},
    { "CONFigure:MATrix?",         CFG_MatrixQ,            0},
    { "CONFigure:VOLTage:DC",      DMM_ConfigureVoltageDc, 0},
    
    /* DMM */
    { "MEASure:VOLTage:DC?",       DMM_MeasureVoltageDcQ,  0},
    { "MEASure:VOLTage:DC:RATio?", SCPI_StubQ,             0},
    { "MEASure:VOLTage:AC?",       DMM_MeasureVoltageAcQ,  0},
    { "MEASure:CURRent:DC?",       SCPI_StubQ,             0},
    { "MEASure:CURRent:AC?",       SCPI_StubQ,             0},
    { "MEASure:RESistance?",       SCPI_StubQ,             0},
    { "MEASure:FRESistance?",      SCPI_StubQ,             0},
    { "MEASure:FREQuency?",        SCPI_StubQ,             0},
    { "MEASure:PERiod?",           SCPI_StubQ,             0},

    { "SYSTem:COMMunication:TCPIP:CONTROL?", SCPI_SystemCommTcpipControlQ,0},
    
    { "TEST:BOOL",        TEST_Bool,   0},
    { "TEST:CHOice?",     TEST_ChoiceQ,0},
    { "TEST#:NUMbers#",   TEST_Numbers,0},
    { "TEST:TEXT",        TEST_Text,   0},
    { "TEST:ARBitrary?",  TEST_ArbQ,   0},
    { "TEST:CHANnellist", TEST_Chanlst,0},

    SCPI_CMD_LIST_END
};

static scpi_interface_t scpi_interface = {
     SCPI_Error,
     SCPI_Write,
     SCPI_Control,
     SCPI_Flush,
     SCPI_Reset,
};

#define SCPI_INPUT_BUFFER_LENGTH 256

static char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
static scpi_reg_val_t scpi_regs[SCPI_REG_COUNT];


scpi_t scpi_context = {
    scpi_commands,//cmdlist
    {//buffer
        SCPI_INPUT_BUFFER_LENGTH,//length
        0,                       //position
        scpi_input_buffer        //*data
    },
    {0,0,0},                     //param_list
    &scpi_interface,             //interface
    0,                           //output_count
    0,                           //input_count
    FALSE,                       //cmd_error
    0,                           //error_queue
    scpi_regs,                   //registers
    scpi_units_def,              //*units
    0,                           //*user_context
    {0},                         //parser_state
    {"MANUFACTURE","INSTR2013",NULL,"01-02"} //* idn[4]
};

