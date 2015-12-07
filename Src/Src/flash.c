/****************************************************************
* File Name          : flash.h
* Author             : cczy&lys
* Version            : V0.0.1
* Date               : 2009-11-12
* Description        : This file provides some flash control 
                       functions,such as Write,Erase and Read.
*****************************************************************/
#include <stdio.h>
#include "flash.h"

#define PRINT_DEBUG

/* Private variables */  
static vu32 WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0x0;
static volatile TestStatus MemoryProgramStatus;

// used to backup all data of a page
static UINT16 PageBakupBuf[FLASH_PAGE_SIZE];

static void Page_Backup(const UINT16* buf)
{
}

/*****************************************************************************
 * Function Name  : Flash_Write
 * Description    : Write data to flash
 * Input          : - beginAddr : specifies the address to be programmed
                    - buf       : specifies the data buffer to be programmed 
                    - bufLen  : length of the buf
                    - datLen  : data width,value: HALFWORD, WORD
                    - safe    : protect other data of the same page
 * Output         : None 
 * Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
                    FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                    FLASH_TIMEOUT.
 * Note           : The function to backup data of a page doesn't take action 
                    now. So you should be careful when programming data.
 *****************************************************************************/
static FLASH_Status Flash_Write(const UINT32 beginAddr, const void* buf, 
        const UINT8 bufLen,const UINT8 datLen,const boolean safe)
{
    //pgIndex = current page
    //bPage   = the begin Page Number
    //epage   = the end   Page Number
    UINT32 bAddr,eAddr;
    UINT8  pgIndex,bPage,ePage; 
    UINT8  bufIndex;
    FLASH_Status FLASHStatus;
    
    bAddr = beginAddr;

    // as byte,datLen=2b (half word), or 4b (word)
    eAddr=beginAddr + bufLen * datLen - 1;
    bPage=ADDR_TO_PAGE(beginAddr);
    ePage=ADDR_TO_PAGE(eAddr);

    //Flash_Init();
    FLASHStatus = FLASH_COMPLETE;
    MemoryProgramStatus = PASSED;  

    //Unlock the Flash Program Erase controller 
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP
                        |FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);

    //Get pages write protection status 
    WRPR_Value = FLASH_GetWriteProtectionOptionByte();
    ProtectedPages = WRPR_Value & 0x000000C0;

    if (ProtectedPages == 0x00)
    {   //Pages are write protected 
        //Disable the write protection 
        FLASHStatus = FLASH_EraseOptionBytes();

        //Generate System Reset to load the new option byte values 
        //NVIC_GenerateSystemReset();
    }
    
    if (ProtectedPages != 0x00)
    {
    //Clear All pending flags 
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP
                |FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);    

        if(safe==TRUE)  Page_Backup(PageBakupBuf);
    //Erase FLASH pages which will be modified
        for(pgIndex = bPage; pgIndex <= ePage; pgIndex++)
        {
            FLASHStatus = FLASH_ErasePage(PAGE_TO_ADDR(pgIndex));
        }
     
    //FLASH Half Word program of data 0x1753 at addresses defined by  
    //StartAddr and EndAddr
        bufIndex=0;
        if(datLen == WORD)
        {   
            while((bufIndex < bufLen) && (FLASHStatus == FLASH_COMPLETE))
            {
                FLASHStatus = FLASH_ProgramWord(bAddr, (UINT32)((UINT32*)buf)[bufIndex++]);
                bAddr += 4;
            }

            //Verify the corectness of written data 
            bufIndex = 0;
            bAddr = beginAddr;
            while((bufIndex < bufLen) && (MemoryProgramStatus != FAILED))
            {
                if((*(vu32*) bAddr) != (UINT32)((UINT32*)buf)[bufIndex++]) 
                    MemoryProgramStatus = FAILED;
                bAddr += 4;
            }
        }
        else if(datLen == HALFWORD)
        {
            while((bufIndex < bufLen) && (FLASHStatus == FLASH_COMPLETE))
            {
                FLASHStatus = FLASH_ProgramHalfWord(bAddr, (UINT16)((UINT16*)buf)[bufIndex++]);
                bAddr += 2;
            }

            //Verify the corectness of written data 
            bufIndex = 0;
            bAddr = beginAddr;
            while((bufIndex < bufLen) && (MemoryProgramStatus != FAILED))
            {
                if((*(vu32*) bAddr) != (UINT16)((UINT16*)buf)[bufIndex++]) 
                    MemoryProgramStatus = FAILED;
                bAddr += 2;
            }   
        }
    }   
    return FLASHStatus;
}

/*****************************************************************************
* Function Name  : Flash_Write_HalfWord
* Description    : Write half word to flash
* Input          : - beginAddr : specifies the address to be programmed
                   - buf  : specifies the data buffer to be programmed 
                   - len  : length of the buf,as halfword
                   - safe : protect other data of the same page
* Output         : None 
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
******************************************************************************/
FLASH_Status Flash_Write_HalfWord(const UINT32 beginAddr, 
                const UINT16* buf, const UINT8 len,const boolean safe)
{            
    return Flash_Write(beginAddr,buf,len,HALFWORD,FALSE);
}

/*****************************************************************************
* Function Name  : Flash_Write_Word
* Description    : Write word to flash
* Input          : - beginAddr : specifies the address to be programmed
                   - buf  : specifies the data buffer to be programmed 
                   - len  : length of the buf,as word
                   - safe : protect other data of the same page
* Output         : None 
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
******************************************************************************/
FLASH_Status Flash_Write_Word(const UINT32 beginAddr, 
                const UINT32* buf, const UINT8 len,const boolean safe)
{            
    return Flash_Write(beginAddr,buf,len,WORD,FALSE);
}

/*****************************************************************************
* Function Name  : Flash_Erase_Page
* Description    : Erase a page contained the argument addr
* Input          : - addr : specifies the address to be erased
* Output         : None 
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
                   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
                   FLASH_TIMEOUT.
******************************************************************************/
FLASH_Status Flash_Erase_Page(const UINT32 addr)
{
    FLASH_Status FLASHStatus;
    UINT32 bAddr,eAddr;

    //Flash_Init();
    FLASHStatus = FLASH_COMPLETE;
    MemoryProgramStatus = PASSED;  

    //Unlock the Flash Program Erase controller 
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP
                        |FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);

    //Get pages write protection status 
    WRPR_Value = FLASH_GetWriteProtectionOptionByte();
    ProtectedPages = WRPR_Value & 0x000000C0;

    if (ProtectedPages == 0x00)
    {   //Pages are write protected 
    //Disable the write protection 
    FLASHStatus = FLASH_EraseOptionBytes();

    //Generate System Reset to load the new option byte values 
        // NVIC_GenerateSystemReset();
    }

    if (ProtectedPages != 0x00)
    {
        //Clear All pending flags 
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP
                                |FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);    

        //Erase the FLASH pages
        FLASHStatus = FLASH_ErasePage(PAGE_TO_ADDR(addr));
        bAddr=addr;
        eAddr=addr + FLASH_PAGE_SIZE;
        while((bAddr < eAddr) && (MemoryProgramStatus != FAILED))
        {
            if((*(vu32*) bAddr) != 0xFFFF) MemoryProgramStatus = FAILED;
            bAddr += 2;
        }
    }   
    return FLASHStatus;
}   

/*****************************************************************************
 * Function Name  : FLASH_ReadHalfWord
 * Description    : Read a half word at a specified Data address.
 * Input          : - Address: specifies the address to be programmed.
 *                    This parameter can be 0x1FFFF804 or 0x1FFFF806. 
 * Output         : None
 * Return         : success=value, error=-1
 *****************************************************************************/
UINT32 Flash_Read_Word(UINT32 Address)
{
  /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Address));
    return *(UINT32*)Address & 0xFFFF + ((*(UINT32*)(Address+2))<<16);
}

/*****************************************************************************
 * Function Name  : FLASH_ReadHalfWord
 * Description    : Read a half word at a specified Data address.
 * Input          : - Address: specifies the address to be programmed.
 *                    This parameter can be 0x1FFFF804 or 0x1FFFF806. 
 * Output         : None
 * Return         : success=value, error=-1
 *****************************************************************************/
UINT16 Flash_Read_HalfWord(UINT32 Address)
{
  /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Address));
    return *(UINT32*)Address & 0xffff;
}

#ifdef  DEBUG

/*****************************************************************************
* Function Name: assert_failed
* Description  : Reports the name of the source file and the source 
                 line number where the assert_param error has occurred.
* Input        : - file: pointer to the source file name
*                - line: assert_param error line source number
* Output       : None
* Return       : None
******************************************************************************/
static void assert_failed(UINT8* file, UINT32 line)
{ 
  /* User can add his own implementation to report the file name 
       and line number, ex: printf("Wrong parameters value: file %s 
         on line %d\r\n", file, line) */
  while (1){}
}

#endif

