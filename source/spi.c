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
 * \brief 
 *
 * \param [in]  
 * \param [out]  
 *
 * \return
 *********************************************************/
int16_t spi_init(void)
{
    InitSpiaGpio();

    /* Put SPI in reset */
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;

    /* set speed */
    SpiaRegs.SPIBRR = (300000000 / ((SysCtrlRegs.LOSPCP.bit.LSPCLK << 1) * 1000000)) - 1;

    SpiaRegs.SPICCR.bit.SPICHAR = 15; //8 bits

    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; //Master

    /* SPI Mode 0 */
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

    SpiaRegs.SPICTL.bit.TALK = 1;

    /* Release from reset */
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

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
int16_t spi_write(uint16_t val)
{
    SpiaRegs.SPIDAT = val;
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
void spi_read(void)
{

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
void spi_readwrite(void)
{
}


/** \} */
/******************************************************** EOF *******************************************************/