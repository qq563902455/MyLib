/**
  ******************************************************************************
  * @file    *.h
  * @author  Lxy Action
  * @version 
  * @date   
  * @brief   This file contains the headers of 
  ******************************************************************************
  * @attention
  *
  *
  * 
  * 
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
	#include "maa_config.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void AHRS_Init(void);
Quarternion AHRS_Update(three_axis gyr,three_axis acc,three_axis mag);	
#ifdef __cplusplus	
}
#endif
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
