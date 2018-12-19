/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 déc. 2018
 *  \addtogroup	TODO
 *  \{
 *  \addtogroup	TODO
 *  \{
 *  
 *  \brief 
 *  
 *  \details 
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		sci.h
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/
#ifndef SCI_H_
#define SCI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Public Macro definition -----------------------------------------------------------------------------------------*/
/* Public Constant definition --------------------------------------------------------------------------------------*/
/* Public Type definition  -----------------------------------------------------------------------------------------*/
typedef void (*SciCallback)(uint16_t sLink, void* pArg);
typedef void (*SciReceiveCallback)(void *pData, uint16_t car);
typedef bool (*SciTransmitCallback)(void *pData, uint16_t *car);
typedef void (*SciEndOfTransmitionCallback)(void *pData);

typedef enum
{
    SCI_SUCCESS,
    SCI_ERROR,
    SCI_BAD_CONFIG,
    SCI_NOT_INIT,
    SCI_TX_ERROR,
    SCI_BAD_ID
}SciReturn_t;

typedef enum
{
    SCI1 = 0,
    SCI2,
    SCI3,

    NB_SERIAL
}SciNumber_t;

typedef enum
{
    B9600 = 9600,
    B19200 = 19200,
    B115200 = 115200,
    B460800 = 460800
}SciSpeed_t;

typedef enum
{
    BIT_7 = 6,
    BIT_8 = 7
}SciDataSize_t;

typedef enum
{
    PARITY_NONE,
    PARITY_EVEN,
    PARITY_ODD
}SciParity_t;

typedef enum
{
    STOP_BIT_1 = 0,
    STOP_BIT_2 = 1
}SciStopBit_t;


typedef struct
{
    SciSpeed_t baudrate;
    SciDataSize_t dataSize;
    SciParity_t parity;
    SciStopBit_t stopBit;
    SciReceiveCallback cbReception;
    void* pReceptionData;
    SciTransmitCallback cbTransmission;
    void* pTransmitionData;
    SciEndOfTransmitionCallback cbEndOfTransmition;
    void* pEndOfTransmitionArg;
    bool initOK;
}SciConfig_t;

/* Public variables ------------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/
uint16_t Sci_Init(uint16_t sLink, SciConfig_t *pConfig);
//uint16_t Sci_Read(uint16_t sLink, uint16_t *pBuffer, const uint16_t size);
//uint16_t Sci_Write(uint16_t sLink, uint16_t *pBuffer, uint16_t size);
//SciReturn_t Sci_StartTX(uint16_t sLink);

void sciIntHandler0(void);
void sciIntHandler1(void);
void sciIntHandler2(void);

#endif /* SCI_H_ */

#ifdef __cplusplus
}
#endif

/** \} */
/** \} */
/******************************************************** EOF *******************************************************/
