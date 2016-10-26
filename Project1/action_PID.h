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
#include "ArtificialNeuralNetwork.h"
#include "action_control.h"
/* Exported enum -------------------------------------------------------------*/
/* Exported class ------------------------------------------------------------*/
class action_PID
{
public:
	float Kp = 0;
	float Ki = 0;
	float Kd = 0;
	float err_sum_max = 9999999999.0f;
	float out_max = 100.0f;
	float err_sum = 0.0f;
	float err_v = 0.0f;
	float last_err = 0.0f;


	float out(float err);
	void  autoTuning(control_model &g, float wc, float rwc, float T);
	float getfeedback(control_model &g, float in, uint8_t cmd);
	void  OptTuning(control_model &g, float wc, float T, float test_in);
	void  OptTuning(control_model &g, float T, float test_in);
};


class BP_PID :public action_PID
{
private:
	float *controller_out;
	float *object_out;
	float *data_st;

	uint16_t na = 0;
	uint16_t nb = 0;
	uint16_t nd = 0;

public:
	BP_PID(uint8_t nA, uint8_t nB, uint8_t nD);
	float BP_PID_out(BP_ANN &control, BP_ANN &model, float set_val, float act_val);
	~BP_PID();
};



/* Exported overload ------------------------------------------------------- */
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

