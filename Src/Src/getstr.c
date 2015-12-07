

#include <stdio.h>
#include "bsp.h"

//====================================================
// 0xa = \r
// 0xd = \n
UINT8 getstr(UINT8 *buf,UINT8 maxlen)
{
    UINT8  index=0;
    if( buf == 0 || maxlen == 0)
    {
        getch();
    }
    else
    {
        do{
            buf[index]='\0';//replace the last char with 0 (string terminal flag)
            buf[index]=getchar();
            switch(buf[index])
            {
            case '\b': //<Backspace>
                if(index>0)
                {
                    index -= 1;
                    printf("\033[1D\033[K");
                }
                buf[index]='\0';
                break;
            case '\r': //<Enaber>
                printf("\r\n");
                break;
            default:
                printf("%c",buf[index]);
                index++;
                break;
            }
        }while( (buf[index] != '\r') && ((index+1)<maxlen) );
        buf[index]='\0';//replace the last char with 0 (string terminal flag)
    }
    return index;
}
