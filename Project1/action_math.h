
/**
******************************************************************************
* @file    action_math.h
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
#ifndef __ACTION_MATH_H
#define __ACTION_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif

	/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
	/* Exported types ------------------------------------------------------------*/
	/* Exported constants --------------------------------------------------------*/
	/* Exported macro ------------------------------------------------------------*/
	/* Exported functions ------------------------------------------------------- */
	void BasicFitting_th(uint8_t num);
	void ordata_input(float val_x, float val_y, float val_y1, float val_y2);
	float **BasicFittingCal(void);
#ifdef __cplusplus
}
#endif
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
