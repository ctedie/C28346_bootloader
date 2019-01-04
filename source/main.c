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
 *  \file		main.c
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "DSP2834x_Device.h"
#include "DSP28x_Project.h"

#include "main.h"
#include "FM25H20.h"
//#include "sci.h"
#include "frame.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
#define GPIO_OUTPUT     1
#define GPIO_INPUT      0

#define RUN_LED_DIR         GpioCtrlRegs.GPCDIR.bit.GPIO79
#define RUN_LED_DATA        GpioDataRegs.GPCDAT.bit.GPIO79
#define RUN_LED_ON()        (RUN_LED_DATA = 1)
#define RUN_LED_OFF()       (RUN_LED_DATA = 0)

#define FAULT_LED_DIR       GpioCtrlRegs.GPCDIR.bit.GPIO78
#define FAULT_LED_DATA      GpioDataRegs.GPCDAT.bit.GPIO78
#define FAULT_LED_ON()      (FAULT_LED_DATA = 1)
#define FAULT_LED_OFF()     (FAULT_LED_DATA = 0)

#define FORCE_LED_DIR       GpioCtrlRegs.GPCDIR.bit.GPIO77
#define FORCE_LED_DATA      GpioDataRegs.GPCDAT.bit.GPIO77
#define FORCE_LED_ON()      (FORCE_LED_DATA = 1)
#define FORCE_LED_OFF()     (FORCE_LED_DATA = 0)

/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/
static uint16_t m_data = 0;
static uint16_t m_FrameChannelNumber = 0xAA;
/* Private functions prototypes ------------------------------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------------------------------*/
static void LED_Config(void);

/* Public functions ------------------------------------------------------------------------------------------------*/

/**
 *********************************************************
 * \brief LED Configuration function
 *********************************************************/
static void LED_Config(void)
{
    EALLOW;
    RUN_LED_DIR = GPIO_OUTPUT;
    FAULT_LED_DIR = GPIO_OUTPUT;
    FORCE_LED_DIR = GPIO_OUTPUT;
    EDIS;

    RUN_LED_ON();
    FORCE_LED_ON();
    FAULT_LED_ON();
}

static uint16_t m_testDataTX[6] = {1, 2, 3, 4, 5, 6};
static uint16_t m_testDataRX[6];
static void rxcall(void *pData, uint16_t *pMsg, uint16_t size);


static uint16_t* alloc(void)
{
    uint16_t* msg;

    msg = (uint16_t*)malloc(50);
    return msg;
}

void liberer(void* pMsg)
{
    free(pMsg);
}

static uint16_t m_dataNotify = 0x0CED;
static void rxcall(void *pData, uint16_t *pMsg, uint16_t size)
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
void main(void)
{
//
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
   InitSysCtrl();

//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to it's default state.

   LED_Config();
//
// Step 3. Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
//
   DINT;

//
// Initialize PIE control registers to their default state.
// The default state is all PIE __interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
   InitPieCtrl();

//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
   IER = 0x0000;
   IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the __interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
   InitPieVectTable();

   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   IER = 0x100;                         // Enable CPU INT

   EINT;
//   ERTM;
//
// Step 4. User specific code:
//

   FM25H20_init();

//   sciConfig.baudrate = B460800;
//   sciConfig.dataSize = BIT_8;
//   sciConfig.stopBit = STOP_BIT_1;
//   sciConfig.parity = PARITY_NONE;
//   sciConfig.cbReception = &rxcall;

   m_FrameChannelNumber = FrameInit(SCI_A, B460800, BIT_8, PARITY_NONE, STOP_BIT_1, rxcall, &m_dataNotify, alloc, liberer);
   if (m_FrameChannelNumber == 0xFF)
   {
       while(1);
   }


    while(1)
    {

        RUN_LED_ON();
        DELAY_US(500000);
        RUN_LED_OFF();
        DELAY_US(500000);
        //        FM25H20_memWrite(0,  m_testDataTX,  6);
//        FM25H20_memRead(0,  m_testDataRX,  6);
//        Sci_Write(SCIA, "Bonjour\n", sizeof("Bonjour\n"));
    }

}
/** \} */
/******************************************************** EOF *******************************************************/
