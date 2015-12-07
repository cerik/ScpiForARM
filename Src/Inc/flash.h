/*
 * Inc/param.h
 *
 * Flash Control Interface
 */
 
#ifndef _FLASH_h
#define _FLASH_h

#include "stm32f10x_flash.h"
#include "datatype.h"


/********************************************************** 
 * Private define : 
 *   STM32F103C8 :
          Flash ----- 64k : 0x08000000 ~ 0x0800FFFF
          SRAM ------ 20k 
 *    Total Page=64 (1k/page) 
 
 **********************************************************
 *   STM32F103VE :
 *        Flash ----- 512k : 0x08000000 ~ 0x0807FFFF
 *        SRAM ------ 64k  : 0x20000000 ~ 0x2000FFFF
 *   The last two page used for parameter;
 **********************************************************/
#define PARAM_PAGE_SIZE       2                /* Parameter takes 2 page */
#define FLASH_PAGE_SIZE      ((UINT16)0x400)      /* Page Size=1K for STM32F103C8 */

#define FLASH_END            ((UINT32)0x0800FFFF) /* Flash End Address   */

#define PARAM_FLASH_LENGTH   (PARAM_PAGE_SIZE * FLASH_PAGE_SIZE)   /* parameter area size */ 
#define PARAM_FLASH_ADDR     (FLASH_END - PARAM_FLASH_LENGTH + 1)  /* start address where stored parameters */

/********************************************************** 
 * Convert Function between page and physical address 
***********************************************************/ 
#define PAGE_TO_ADDR(page) (UINT32)((FLASH_BASE + (page) * FLASH_PAGE_SIZE)) 
#define ADDR_TO_PAGE(addr) ((addr - FLASH_BASE)/FLASH_PAGE_SIZE)

/********************************************************** 
 * Private define : 
 *    Flash of STM32F103RB: 0x08000000 ~ 0x0801FFFF
 *    Total Page=127 (1k/page) 
***********************************************************/
//#define FLASH_PAGE_SIZE    ((UINT16)0x400)   //1K for stm32f103rb
//#define FLASH_BASE ((UINT32)0x08000000)
//#define FLASH_END  ((UINT32)0x0801FFFF)

/********************************************************** 
 * Convert Function between page and physical address 
***********************************************************/ 
//#define PAGE_TO_ADDR(page) (UINT32)((FLASH_BASE + (page) * FLASH_PAGE_SIZE)) 
//#define ADDR_TO_PAGE(addr) ((addr - FLASH_BASE)/FLASH_PAGE_SIZE)

// arg    : page ( 0~127 )
// return : addr if ok, -1 if wrong
//#define PAGE_TO_ADDR(page) (UINT32)((page >=0 && page <=127)?\
//                      (FLASH_BASE + (page) * FLASH_PAGE_SIZE):-1) 
//#define ADDR_TO_PAGE(addr) ((addr >= FLASH_BASE && addr <= FLASH_END)?\
//                      ((addr - FLASH_BASE)/FLASH_PAGE_SIZE):-1)


#define HALFWORD sizeof(UINT16)
#define WORD     sizeof(UINT32)


/********************************************************************
 * Function Name : FLASH_ReadHalfWord
 * Description   : Read a half word at a specified Data address.
 * Input         : - Address: specifies the address to be programmed.
 *                 This parameter can be 0x1FFFF804 or 0x1FFFF806. 
 * Output        : None
 * Return        : success=value, error=-1
 ********************************************************************/
UINT32 Flash_Read_Word(UINT32 Address);

/********************************************************************
 * Function Name : FLASH_ReadHalfWord
 * Description   : Read a half word at a specified Data address.
 * Input         : - Address: specifies the address to be programmed.
 *                 This parameter can be 0x1FFFF804 or 0x1FFFF806. 
 * Output        : None
 * Return        : success=value, error=-1
 *******************************************************************
UINT16 Flash_Read_HalfWord(UINT32 Address);*/

/********************************************************************
 * Function Name : Flash_Write_HalfWord
 * Description   : Write half word to flash
 * Input         : - beginAddr : specifies the address to be programmed
                   - buf  : specifies the data buffer to be programmed 
                   - len  : length of the buf, as halfword
                   - safe : protect other data of the same page
 * Output        : None 
 * Return        : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
********************************************************************
FLASH_Status Flash_Write_HalfWord(const UINT32 beginAddr, 
        const UINT16* buf, const UINT8 len,const bool safe);*/

/********************************************************************
 * Function Name : Flash_Write_Word
 * Description   : Write word to flash
 * Input         : - beginAddr : specifies the address to be programmed
                   - buf  : specifies the data buffer to be programmed 
                   - len  : length of the buf, as word
                   - safe : protect other data of the same page
 * Output        : None 
 * Return        : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
*********************************************************************/
FLASH_Status Flash_Write_Word(const UINT32 beginAddr, 
        const UINT32* buf, const UINT8 len,const boolean safe);

/********************************************************************
 * Function Name : Flash_Erase_Page
 * Description   : Erase a page contained the argument addr
 * Input         : - addr : specifies the address to be erased
 * Output        : None 
 * Return        : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
*********************************************************************/
FLASH_Status Flash_Erase_Page(const UINT32 addr);

#endif
