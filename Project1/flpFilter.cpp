/**
******************************************************************************
* @file    action_math.cpp
* @author  lxy
* @version
* @date
* @brief
******************************************************************************
* @attention
*
*
*
*
******************************************************************************
*/
/* Includes -------------------------------------------------------------------*/
#include "flpFilter.h"
#include <cmath>



#include <iostream>
using namespace std;



/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
flpFilter::flpFilter(uint16_t n, float stepValue)
	:stepSize(stepValue),count(0),  a(0), b(0), variableStepSizeFlag(0)
{
	bParam = new action_matrix(1, n, MATRIX_ZERO);
	lastInput = new action_matrix(1, n, MATRIX_ZERO);

	for (uint16_t i = 0; i < n; i++)
	{
		if (i == 0) 
		{
			(*bParam)[0][i] = 1;
		}
		else
		{
			(*bParam)[0][i] = 0;
		}
	}
}
flpFilter::flpFilter(uint16_t n, float alpha, float beta)
	:count(0),a(alpha),b(beta),variableStepSizeFlag(1)
{
	bParam = new action_matrix(1, n, MATRIX_ZERO);
	lastInput = new action_matrix(1, n, MATRIX_ZERO);

	for (uint16_t i = 0; i < n; i++)
	{
		if (i == 0)
		{
			(*bParam)[0][i] = 1;
		}
		else
		{
			(*bParam)[0][i] = 0;
		}
	}
}
flpFilter::flpFilter(const flpFilter & m)
	:stepSize(m.stepSize), variableStepSizeFlag(m.variableStepSizeFlag)
{
	bParam = new action_matrix(1, m.bParam->get_column(), MATRIX_ZERO);
	lastInput = new action_matrix(1, m.lastInput->get_column(), MATRIX_ZERO);
	(*bParam) = (*m.bParam);
	(*lastInput) = (*m.lastInput);
}
flpFilter::~flpFilter()
{
	delete bParam;
	delete lastInput;
}
float flpFilter::out(float rawData)
{
	float outTemp, err;
	outTemp = static_cast<float>(((*bParam) * (!(*lastInput)))[0][0]);
	err = rawData - outTemp;
	cout << "lastInput: " << *lastInput;
	cout << "bParam: " << *bParam;
	cout << "err: " << err << endl;
	if (variableStepSizeFlag)
	{
		stepSize = b*(1 - exp(-a*err*err));
	}
	cout << "step: " << stepSize << endl;
	(*bParam) = (*bParam) + stepSize*err*(*lastInput);

	
	for (uint16_t i = lastInput->get_column() - 1; i > 0; i--)
	{
		(*lastInput)[0][i] = (*lastInput)[0][i - 1];
	}
	(*lastInput)[0][0] = rawData;

	
	
	return outTemp;
}


/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/


