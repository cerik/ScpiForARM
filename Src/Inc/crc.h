#ifndef _CRC_H_
#define _CRC_H_

#include "common.h"
u16 update_crc_ccitt( u16 crc, u8 ch );
void init_crcccitt_tab( void ) ;

#endif