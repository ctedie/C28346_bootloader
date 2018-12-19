/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 déc. 2018
 *  \addtogroup	TODO
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		sci.c
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

#include "DSP2834x_Device.h"
#include "DSP2834x_Examples.h"

#include "DSP2834x_Sci.h"
#include "sci.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
typedef struct
{
    volatile struct SCI_REGS *sciReg;

    SciReceiveCallback cbReception;
    void* pReceptionData;
    SciTransmitCallback cbTransmission;
    void* pTransmitionData;
    SciEndOfTransmitionCallback cbEndOfTransmition;
    void* pEndOfTransmitionArg;
    bool initOK;

    uint32_t _intFlags;
}sci_t;

/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/
sci_t m_sciList[NB_SERIAL] =
{
       {
                .sciReg = &SciaRegs,
                .initOK = false
        }
};


//static const uint32_t m_UARTPeriph[NB_SERIAL] =
//{
//        SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1, SYSCTL_PERIPH_UART2
//};

//static const uint32_t m_UARTInt[NB_SERIAL] =
//{
//    INT_UART0, INT_UART1, INT_UART2
//};

typedef void (*pIntFunction)(void);
static const pIntFunction m_IntFuncTable[] =
{
        sciIntHandler0,
        sciIntHandler1,
        sciIntHandler2
};

/* Private functions prototypes ------------------------------------------------------------------------------------*/
static void generalIntHandler(uint32_t uartNb);
void testCallback(uint16_t uartNb, void* pArg);
static void disableTxInterrupt(uint32_t uartNb);
static void enableTxInterrupt(uint32_t uartNb);

static char receivedChar;

/* Private functions -----------------------------------------------------------------------------------------------*/
void sciIntHandler0(void)
{
    generalIntHandler(SCI1);
}

void sciIntHandler1(void)
{
    generalIntHandler(SCI2);
}

void sciIntHandler2(void)
{
    generalIntHandler(SCI3);
}


static void generalIntHandler(uint32_t uartNb)
{
//    uint32_t intStatus;
//    uint16_t car;
//    uint16_t txCar;
//
//    intStatus = UARTIntStatus(m_sciList[uartNb].uartBase, true);
//    UARTIntClear(m_sciList[uartNb].uartBase, intStatus);
//
//    if(intStatus & (UART_INT_RX | UART_INT_RT))
//    {
//        if(m_sciList[uartNb].cbReception != NULL)
//        {
//            if(UARTCharsAvail(m_sciList[uartNb].uartBase))
//            {
//                car = (uint16_t)UARTCharGetNonBlocking(m_sciList[uartNb].uartBase);
//                m_sciList[uartNb].cbReception(m_sciList[uartNb].pReceptionData, car);
//            }
//        }
//    }
//    else if(intStatus & UART_INT_TX)
//    {
//        if(m_sciList[uartNb].cbTransmission != NULL)
//        {
//            while(UARTSpaceAvail(m_sciList[uartNb].uartBase))
//            {
//                //Send the first characters
//                if(m_sciList[uartNb].cbTransmission(m_sciList[uartNb].pTransmitionData, &car))
//                {
//                    UARTCharPutNonBlocking(m_sciList[uartNb].uartBase, car);
//
//                }
//                else
//                {
//                    break;
//                }
//            }
//
//        }
//
//    }
//    else
//    {
////      UARTCharGet(m_sciList[uartNb].uartNb);
//    }
//
//
//
}

static void disableTxInterrupt(uint32_t uartNb)
{
//    m_sciList[uartNb]._intFlags &= ~UART_INT_TX;
//    UARTIntDisable(m_sciList[uartNb].uartBase, UART_INT_TX);
}

static void enableTxInterrupt(uint32_t uartNb)
{
//    m_sciList[uartNb]._intFlags |= UART_INT_TX;
//    UARTIntEnable(m_sciList[uartNb].uartBase, UART_INT_TX);
}

static uint16_t cptReceivedChar;
void testCallback(uint16_t uartNb, void* pArg)
{
    char* character = pArg;

}

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
SciReturn_t Sci_Init(SciNumber_t sciNb, SciConfig_t *pConfig)
{
    SciReturn_t ret = SCI_SUCCESS;
    uint16_t baudrate;

    uint32_t config = 0;
    uint32_t intFlags = 0;
    uint32_t fifoLevelsFlags = 0;

    sci_t *pSci = NULL;

    if(pConfig == NULL)
    {
        //Error
        return SCI_BAD_CONFIG;
    }

    InitSciaGpio();

    pSci = &m_sciList[sciNb];

    pSci->sciReg->SCICTL1.bit.SWRESET = 1;

    m_sciList[sciNb].cbReception = pConfig->cbReception;
    m_sciList[sciNb].pReceptionData = pConfig->pReceptionData;
    m_sciList[sciNb].cbTransmission = pConfig->cbTransmission;
    m_sciList[sciNb].pTransmitionData = pConfig->pTransmitionData;
    m_sciList[sciNb].cbEndOfTransmition = pConfig->cbEndOfTransmition;
    m_sciList[sciNb].pEndOfTransmitionArg = pConfig->pEndOfTransmitionArg;


    pSci->sciReg->SCICCR.bit.SCICHAR = pConfig->dataSize;

    switch (pConfig->parity)
    {
        case PARITY_NONE:
            pSci->sciReg->SCICCR.bit.PARITYENA = 0;
            break;
        case PARITY_EVEN:
            pSci->sciReg->SCICCR.bit.PARITYENA = 1;
            pSci->sciReg->SCICCR.bit.PARITY = 1;
            break;
        case PARITY_ODD:
            pSci->sciReg->SCICCR.bit.PARITYENA = 1;
            pSci->sciReg->SCICCR.bit.PARITY = 0;
            break;
        default:
            return SCI_BAD_CONFIG;
    }

    pSci->sciReg->SCICCR.bit.STOPBITS = pConfig->stopBit;


    //SysCtlPeripheralEnable(m_UARTPeriph[sciNb]);

    baudrate = (300000000 / (((uint32_t)(SysCtrlRegs.LOSPCP.bit.LSPCLK << 1) * (uint32_t)pConfig->baudrate)*8)) - 1;
    pSci->sciReg->SCIHBAUD = (baudrate & 0xFF00) >> 8;
    pSci->sciReg->SCILBAUD = (baudrate & 0xFF);


//    if(m_sciList[sciNb].cbReception != NULL)
//    {
//        m_sciList[sciNb]._intFlags = (UART_INT_RX | UART_INT_RT);
//    }

    pSci->sciReg->SCICTL1.bit.RXENA = 1;
    pSci->sciReg->SCICTL1.bit.TXENA = 1;

    m_sciList[sciNb].initOK = true;

    return ret;
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
//SciReturn_t sci_StartTX(uint16_t sLink)
//{
//    uint16_t car;
//
//    if(sLink > 4)
//    {
//        return SERIAL_LINK_BAD_ID;
//    }
//
//    if(m_sciList[sLink].initOK == false)
//    {
//        return SERIAL_LINK_NOT_INIT;
//    }
//
//    while(UARTSpaceAvail(m_sciList[sLink].uartBase))
//    {
//        //Send the first characters
//        if(m_sciList[sLink].cbTransmission(m_sciList[sLink].pTransmitionData, &car))
//        {
//            UARTCharPutNonBlocking(m_sciList[sLink].uartBase, car);
//
//        }
//        else
//        {
//            break;
//        }
//    }
//
//    enableTxInterrupt(sLink);
//    return SERIAL_LINK_SUCCESS;
//
//}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
//SciReturn_t sci_Write(uint16_t sLink, uint16_t *pBuffer, const uint16_t size)
//{
//    uint16_t ret = SERIAL_LINK_SUCCESS;
//    uint16_t i;
//
//
//    if(m_sciList[sLink].initOK == false)
//    {
//
//        ret = SERIAL_LINK_NOT_INIT;
//    }
//
//    if(ret == SERIAL_LINK_SUCCESS)
//    {
//        for (i = 0; i < size; ++i)
//        {
//            //TODO Non blocking
//        }
//    }
//
//    return ret;
//}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
//SciReturn_t sci_Read(uint16_t sLink, uint16_t *pBuffer, const uint16_t size)
//{
//    uint16_t ret = SERIAL_LINK_SUCCESS;
//    uint16_t i;
//
//
//    if(m_sciList[sLink].initOK == false)
//    {
//
//        ret = SERIAL_LINK_NOT_INIT;
//    }
//
//    if(ret == SERIAL_LINK_SUCCESS)
//    {
//        for (i = 0; i < size; ++i)
//        {
//
//        }
//    }
//
//    return ret;
//}

/** \} */
/******************************************************** EOF *******************************************************/
