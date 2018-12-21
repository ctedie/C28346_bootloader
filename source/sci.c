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

/* Private functions prototypes ------------------------------------------------------------------------------------*/
static void generalTxIntHandler(uint16_t uartNb);
static void generalRxIntHandler(uint16_t uartNb);
void testCallback(uint16_t uartNb, void* pArg);
static void disableTxInterrupt(uint32_t uartNb);
static void enableTxInterrupt(uint32_t uartNb);
static void fifoInit(uint16_t sciNb);
interrupt void sciTxIntHandler0(void);
interrupt void sciTxIntHandler1(void);
interrupt void sciTxIntHandler2(void);
interrupt void sciRxIntHandler0(void);
interrupt void sciRxIntHandler1(void);
interrupt void sciRxIntHandler2(void);

static const pIntFunction m_IntTxFuncTable[] =
{
        sciTxIntHandler0,
        sciTxIntHandler1,
        sciTxIntHandler2
};

static const pIntFunction m_IntRxFuncTable[] =
{
        sciRxIntHandler0,
        sciRxIntHandler1,
        sciRxIntHandler2
};

static char receivedChar;

/* Private functions -----------------------------------------------------------------------------------------------*/
interrupt void sciTxIntHandler0(void)
{
    generalTxIntHandler(SCI_A);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
}

interrupt void sciTxIntHandler1(void)
{
    generalTxIntHandler(SCI_B);
}

interrupt void sciTxIntHandler2(void)
{
    generalTxIntHandler(SCI_C);
}

interrupt void sciRxIntHandler0(void)
{
    generalRxIntHandler((uint16_t)SCI_A);
    PieCtrlRegs.PIEACK.all |= M_INT9;
}

void sciRxIntHandler1(void)
{
    generalRxIntHandler(SCI_B);
}

interrupt void sciRxIntHandler2(void)
{
    generalRxIntHandler(SCI_C);
}


static void generalTxIntHandler(uint16_t uartNb)
{
//    if(intStatus & UART_INT_TX)
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
////TODO clear interrupt
//
}

static void generalRxIntHandler(uint16_t uartNb)
{
    uint16_t car = 0;

    if(m_sciList[uartNb].sciReg->SCIFFRX.bit.RXFFST > 0)
    {
        car = m_sciList[uartNb].sciReg->SCIRXBUF.bit.RXDT;
        if(m_sciList[uartNb].cbReception != NULL)
        {
            m_sciList[uartNb].cbReception(m_sciList[uartNb].pReceptionData, car);
        }
    }

    m_sciList[uartNb].sciReg->SCIFFRX.bit.RXFFOVRCLR = 1;
    m_sciList[uartNb].sciReg->SCIFFRX.bit.RXFFINTCLR = 1;




}

static void disableTxInterrupt(uint32_t uartNb)
{
    m_sciList[uartNb].sciReg->SCICTL2.bit.TXINTENA = 0;

}

static void enableTxInterrupt(uint32_t uartNb)
{
    m_sciList[uartNb].sciReg->SCICTL2.bit.TXINTENA = 1;
}

static uint16_t cptReceivedChar;
void testCallback(uint16_t uartNb, void* pArg)
{
    char* character = pArg;

}

static void fifoInit(uint16_t sciNb)
{

    sci_t *pSci = &m_sciList[sciNb];


    EALLOW;
    PieVectTable.SCIRXINTA = &sciRxIntHandler0;
    PieVectTable.SCITXINTA = &sciTxIntHandler0;
    EDIS;
    IER |= M_INT9;

    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
    PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2


    pSci->sciReg->SCICTL1.bit.SWRESET = 0;

    pSci->sciReg->SCIFFCT.all = 0;

    pSci->sciReg->SCIFFTX.bit.SCIFFENA = 1;
    pSci->sciReg->SCIFFTX.bit.TXFIFOXRESET = 0;
    pSci->sciReg->SCIFFRX.bit.RXFIFORESET = 0;

    pSci->sciReg->SCIFFTX.bit.TXFFIL = 0;
    pSci->sciReg->SCIFFTX.bit.TXFFIENA = 0;
    pSci->sciReg->SCIFFTX.bit.SCIRST = 1;

    pSci->sciReg->SCIFFRX.bit.RXFFIL = 1;
    pSci->sciReg->SCIFFRX.bit.RXFFIENA = 1;
    pSci->sciReg->SCIFFRX.bit.RXFFINTCLR = 1;

    pSci->sciReg->SCIFFTX.bit.TXFIFOXRESET = 1;
    pSci->sciReg->SCIFFRX.bit.RXFIFORESET = 1;

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


//    fifoInit(sciNb);

    EALLOW;
    PieVectTable.SCIRXINTA = &sciRxIntHandler0;
    PieVectTable.SCITXINTA = &sciTxIntHandler0;
    EDIS;
    IER |= M_INT9;

    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
    PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2


    pSci->sciReg->SCICTL1.bit.SWRESET = 0;

    pSci->sciReg->SCICTL1.bit.RXENA = 1;
    pSci->sciReg->SCICTL1.bit.TXENA = 1;
    pSci->sciReg->SCICTL2.bit.RXBKINTENA = 1;
    pSci->sciReg->SCICTL2.bit.TXINTENA = 0;

    pSci->sciReg->SCIFFTX.bit.TXFIFOXRESET = 0;
    pSci->sciReg->SCIFFTX.bit.SCIFFENA = 1;
    pSci->sciReg->SCIFFTX.bit.SCIRST = 1;
    pSci->sciReg->SCIFFTX.bit.TXFFIL = 0;
    pSci->sciReg->SCIFFTX.bit.TXFFIENA = 0;

    pSci->sciReg->SCIFFRX.bit.RXFIFORESET = 0;
    pSci->sciReg->SCIFFRX.bit.RXFFIL = 1;
    pSci->sciReg->SCIFFRX.bit.RXFFIENA = 1;

    pSci->sciReg->SCIFFCT.all = 0;


    pSci->sciReg->SCICTL1.bit.SWRESET = 1;
    pSci->sciReg->SCIFFTX.bit.TXFIFOXRESET = 1;
    pSci->sciReg->SCIFFTX.bit.TXFFINTCLR = 1;
    pSci->sciReg->SCIFFRX.bit.RXFIFORESET = 1;
    pSci->sciReg->SCIFFRX.bit.RXFFINTCLR = 1;


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
SciReturn_t Sci_Write(uint16_t sLink, uint16_t *pBuffer, const uint16_t size)
{
    SciReturn_t ret = SCI_SUCCESS;
    uint16_t i;


    if(m_sciList[sLink].initOK == false)
    {

        ret = SCI_NOT_INIT;
    }

    if(ret == SCI_SUCCESS)
    {
        for (i = 0; i < size; ++i)
        {
            //TODO Non blocking
            m_sciList[sLink].sciReg->SCITXBUF = *pBuffer;
            while(m_sciList[sLink].sciReg->SCICTL2.bit.TXEMPTY != 1);
            pBuffer++;
        }
    }

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
SciReturn_t Sci_Read(uint16_t sLink, uint16_t *pBuffer, const uint16_t size)
{
    SciReturn_t ret = SCI_SUCCESS;
    uint16_t i;


    if(m_sciList[sLink].initOK == false)
    {

        ret = SCI_NOT_INIT;
    }

    if(ret == SCI_SUCCESS)
    {
        for (i = 0; i < size; ++i)
        {
            while(m_sciList[sLink].sciReg->SCIRXST.bit.RXRDY != 1);
            *pBuffer =  m_sciList[sLink].sciReg->SCIRXBUF.bit.RXDT;
            pBuffer++;
        }
    }

    return ret;
}

/** \} */
/******************************************************** EOF *******************************************************/
