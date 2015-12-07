
#include "datatype.h"
#define P_CCITT 0x1021
static UINT16 crc_tabccitt[256];

/*******************************************************************
 *                                               
 *   unsigned short update_crc_ccitt( unsigned long crc, char c );  
 *                                                                 *
 *   The function update_crc_ccitt calculates  a  new  CRC-CCITT  
 *   value  based  on the previous value of the CRC and the next   
 *   byte of the data to be checked.               
 *                                                               
 *******************************************************************
 */
UINT16 update_crc_ccitt( UINT16 crc, UINT8 ch )
{
    UINT16 tmp, short_c;

    short_c  = 0x00ff & (UINT16)ch;
    tmp = (crc >> 8) ^ short_c;
    crc = (crc << 8) ^ crc_tabccitt[tmp];

    return crc;
} 

/*
  *****************************************************************
  *                                                                   
  *   static void init_crcccitt_tab( void );     
  *                                      
  *   The function init_crcccitt_tab() is used to fill the  array 
  *   for calculation of the CRC-CCITT with values.
  *                                                         
  ******************************************************************
  */
void init_crcccitt_tab( void ) 
{
    int i, j;
    UINT16 crc,ch;
    for (i=0; i<256; i++) 
    {
        crc = 0;
        ch   = ((UINT16)i) << 8;

        for (j=0; j<8; j++) 
        {
            if ( (crc ^ ch) & 0x8000 ) 
                crc = ( crc << 1 ) ^ P_CCITT;
            else                      
                crc =   crc << 1;
            ch = ch << 1;
        }
        crc_tabccitt[i] = crc;
    }
}
