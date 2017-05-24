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
#ifndef __KALMANFILTER_H
#define __KALMANFILTER_H

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include "action_matrix.h"
/* Exported enum -------------------------------------------------------------*/
/* Exported class ------------------------------------------------------------*/
class kalmanFilter
{
protected:
	action_matrix Aa;
	action_matrix H;
	action_matrix L;
	action_matrix Q;
	action_matrix R;
	action_matrix P;
	action_matrix X;
	action_matrix K;
public:
	kalmanFilter(action_matrix& Atemp, action_matrix& Htemp, action_matrix& Ltemp, 
				 action_matrix& Qtemp, action_matrix& Rtemp, action_matrix& Ptemp,action_matrix& Xtemp);
	virtual action_matrix estimateState(action_matrix& Z,action_matrix& U);
};
class fadingKalmanFilter: public kalmanFilter
{
protected:
	const uint32_t len;
	action_matrix* r;
public:
	fadingKalmanFilter(action_matrix& Atemp, action_matrix& Htemp, action_matrix& Ltemp, 
										 action_matrix& Qtemp, action_matrix& Rtemp, action_matrix& Ptemp,
										 action_matrix& Xtemp,	     uint32_t len);
	~fadingKalmanFilter();
	virtual action_matrix estimateState(action_matrix& Z,action_matrix& U);
};

/* Exported overload ------------------------------------------------------- */
#endif
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/


