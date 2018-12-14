/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		26 nov. 2018
 *  \addtogroup	TODO
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		FM25H20.c
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "DSP2834x_Device.h"
#include "DSP2834x_Examples.h"

#include "spi.h"
#include "FM25H20.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
#define WREN    0x06              //Set Write Enable Latch 0000 0110b
#define WRDI    0x04              //Write Disable 0000 0100b
#define RDSR    0x05              //Read Status Register 0000 0101b
#define WRSR    0x01              //Write Status Register 0000 0001b
#define READ    0x03              //Read Memory Data 0000 0011b
#define WRITE   0x02              // Write Memory Data 0000 0010b
#define SLEEP   0xB9              // Enter Sleep Mode 1011 1001b


#define CS_HIGH()   GpioDataRegs.GPASET.bit.GPIO19 = 1
#define CS_LOW()    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1

#define SWAP_32(x)              ( (((x)&0xFF)<<24) | (((x)&0xFF00)<<8) | (((x)&0xFF0000)>>8) | (((x)&0xFF000000)>>24) )
#define ADDRESS_FORMAT(x)    ( SWAP_32( ((x)&0x0003FFFF)<<14 ) )

/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint32_t address:24;
    uint32_t data:8;
}tMemoryData;

/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/
/* Private functions prototypes ------------------------------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/

/**
 *********************************************************
 * \brief 
 *
 * \param [in]  
 * \param [out]  
 *
 * \return
 *********************************************************/
void FM25H20_init(void)
{
    //TODO SPI 8 bit
    spi_init();
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;
    EDIS;
    CS_HIGH();
}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
uint16_t FM25H250_statusRead(void)
{
    uint16_t tx[] = {RDSR, 0};
    uint16_t status[2];
    CS_LOW();
    spi_readwrite(tx, status, 2);
    CS_HIGH();

    return status[1];
}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
uint16_t FM25H20_memWrite(uint32_t address, const uint16_t *pData, uint32_t size)
{
    uint16_t tx = WREN;
    uint32_t realAddress = ADDRESS_FORMAT(address);
    uint16_t tab_address[3] = {(realAddress>>16), (realAddress&0xFF00)>>8, (realAddress&0xFF)};

    CS_LOW();
    /* Write Enable */
    spi_write(&tx, 1);
    CS_HIGH();

    CS_LOW();

    /* Write Memory Code */
    tx = WRITE;
    spi_write(&tx, 1);

    /* Write address */
    spi_write(tab_address, 3);

    /* Write data */
    spi_write(pData, size);

    CS_HIGH();

    return 0;
}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
uint16_t FM25H20_memRead(uint32_t address, uint16_t *pData, uint32_t size)
{
    uint16_t tx = READ;
    uint32_t realAddress = ADDRESS_FORMAT(address);
    uint16_t tab_address[3] = {(realAddress>>16), (realAddress&0xFF00)>>8, (realAddress&0xFF)};

    CS_LOW();
    /* Memory read */
    spi_write(&tx, 1);


    /* Write address */
    spi_write(tab_address, 3);

    /* Write data */
    spi_read(pData, size);

    CS_HIGH();

    return 0;

}

/** \} */
/******************************************************** EOF *******************************************************/
