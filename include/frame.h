/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 d�c. 2018
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
 *  \file		frame.h
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/
#ifndef FRAME_H_
#define FRAME_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "sci.h"

/* Public Macro definition -----------------------------------------------------------------------------------------*/
/* Public Constant definition --------------------------------------------------------------------------------------*/
/* Public Type definition  -----------------------------------------------------------------------------------------*/
typedef void (*cbNotifyRx_t)(void *pData, uint16_t *pMsg, uint16_t size);
typedef uint16_t* (*cbAllocMsg_t)(void);
typedef void (*cbFreeMsg_t)(void* pMsg);

/* Public variables ------------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/
uint16_t SerialLinkFrameProtocoleInit(SciNumber_t link,
                                   SciSpeed_t baurate,
                                   SciDataSize_t bitSize,
                                   SciParity_t parity,
                                   SciStopBit_t stopBit,
                                   cbNotifyRx_t cbNotifyRx,
                                   void* pDataNotifyRx,
                                   cbAllocMsg_t cbAllocMsg,
                                   cbFreeMsg_t cbFreeMsg
                                   );

void SerialLinkFrameProtocole_Send(uint16_t channel, uint16_t *pMsg, uint16_t size);

#endif /* FRAME_H_ */

#ifdef __cplusplus
}
#endif

/** \} */
/** \} */
/******************************************************** EOF *******************************************************/
