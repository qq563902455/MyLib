
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
#ifndef __FLPFILTER_H
#define __FLPFILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#include "action_matrix.h"
/* Exported class ----------------------------------------------------------- */
class flpFilter
{
private:
	action_matrix* bParam;
	action_matrix* lastInput;
	float stepSize;
	uint16_t count=0;

	float a;
	float b;
	uint8_t variableStepSizeFlag = 0;
public:
	flpFilter(uint16_t n, float stepValue);
	flpFilter(uint16_t n, float a, float b);
	flpFilter(const flpFilter& m);
	~flpFilter();
	float out(float rawData);
};
#endif

#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

