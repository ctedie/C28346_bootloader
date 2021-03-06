/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		23 nov. 2018
 *  \addtogroup	TODO
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		spi.c
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
//#include "DSP2834x_GlobalPrototypes.h"
#include "DSP2834x_Spi.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/
/* Private functions prototypes ------------------------------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/

/**
 *********************************************************
 * \brief SPI Initialization
 *********************************************************/
int16_t spi_init(void)
{
    InitSpiaGpio();

    /* Put SPI in reset */
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;

    /* set speed */
    SpiaRegs.SPIBRR = (300000000 / ((SysCtrlRegs.LOSPCP.bit.LSPCLK << 1) * 1000000)) - 1;

    SpiaRegs.SPICCR.bit.SPICHAR = 7; //8 bits

    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; //Master

    /* SPI Mode 0 */
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

    SpiaRegs.SPICTL.bit.TALK = 1;

    /* Release from reset */
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;


    /* TODO */
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;

    return 0;
}

/**
 *********************************************************
 * \brief SPI Write function
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
int16_t spi_write(const uint16_t *pDataTx, uint32_t size)
{
    uint32_t index;
    uint16_t dummy;
    for (index = 0; index < size; ++index)
    {
        SpiaRegs.SPITXBUF = ((*pDataTx) & 0xFF)<<8;
        while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
        dummy = (SpiaRegs.SPIRXBUF & 0xFF);
        pDataTx++;
    }

    return 0;
}

/**
 *********************************************************
 * \brief   SPI Read function
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
void spi_read(uint16_t *pDataRx, uint32_t size)
{
    uint32_t index;
    uint16_t dummy = 0xAA;
    for (index = 0; index < size; ++index)
    {
        SpiaRegs.SPITXBUF = (dummy & 0xFF)<<8;
        while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
        *(pDataRx) = (SpiaRegs.SPIRXBUF & 0xFF);
        pDataRx++;
    }

}

/**
 *********************************************************
 * \brief   SPI Write and Read function
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
void spi_readwrite(const uint16_t *pDataTx, uint16_t *pDataRx, uint32_t size)
{
    uint32_t index;

    for (index = 0; index < size; ++index)
    {
        SpiaRegs.SPITXBUF = ((*pDataTx) & 0xFF)<<8;
        while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
        *(pDataRx) = (SpiaRegs.SPIRXBUF & 0xFF);
        pDataTx++;
        pDataRx++;
    }

}


/** \} */
/******************************************************** EOF *******************************************************/
