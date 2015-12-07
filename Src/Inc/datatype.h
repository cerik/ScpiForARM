//=============================================================================
// File Description:
//
//=============================================================================
// Log:
//=============================================================================

#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#ifndef __FUNCTION__
#define __FUNCTION__  __FILE__
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef         unsigned char  UINT8;
typedef           signed char  INT8;
typedef        unsigned short  UINT16;
typedef          signed short  INT16;
typedef          unsigned int  UINT32;
typedef            signed int  INT32;
typedef                 float  FLOAT32;
typedef                double  FLOAT64;
typedef         unsigned long long  UINT64;
typedef           signed long long INT64;

typedef volatile   signed int  VINT32;
typedef volatile unsigned int  VUINT32;

typedef enum {FAILED=0,PASSED=1}TestStatus;
typedef unsigned char boolean;
#define FALSE 0
#define TRUE  1

typedef union tagDataBytes {
    INT16   i16Val[4];
    UINT16  ui16Val[4];
    INT32   i32Val[2];
    UINT32  ui32Val[2];
    FLOAT64 f64Val;
    UINT64  ui64Val;
    INT64   i64Val;
    UINT8   ucBytes[8];
}DATABYTES;


#define NULLCHAR  '\0'

#define MAXUINT8  0xFF
#define MAXUINT16 0xFFFF
#define MAXUINT32 0xFFFFFFFF

typedef enum{GPIO_IN_MODE=0,GPIO_OUT_MODE=1} GPIOMODE;
typedef struct{
    INT32 ms;
    UINT32 t0,t1;
}tagCounter;

#define MAX_CMD_LEN 16   //最长命令字符串的长度
#define MAXARGLEN    5   //最大有效参数长度
#define MAXBUFF    255   //最大消息缓冲区长度

#define MSG_BUFFER_SIZE 100
typedef struct _msg_buffer_t{
   char     uartBuffer[MSG_BUFFER_SIZE];
   char     scpiBuffer[MSG_BUFFER_SIZE];
   UINT16   size;
   boolean  rxdone; //TRUE when msg hasn't been processed, and FALSE when buffer is available to store new msg;
}msg_buffer_t;



#endif
