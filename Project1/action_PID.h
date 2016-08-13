/**
******************************************************************************
* @file    action_PID.h
* @author  Lxy Action
* @version
* @date
* @brief   This file contains the headers of C++ FILE
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
#ifndef __ACTION_PID_H
#define __ACTION_PID_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "action_control.h"
/* Exported enum -------------------------------------------------------------*/
/* Exported class ------------------------------------------------------------*/
class action_PID
{
public:
	float Kp = 0;
	float Ki = 0;
	float Kd = 0;
	float out(float err);
	void autoTuning(control_model &g, float wc, float rwc, float T);
private:
	float err_sum = 0;
	float last_err = 0;
	float err_sum_max = 50;
};
/* Exported overload ------------------------------------------------------- */
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

