/**
******************************************************************************
* @file    fastFloat.cpp
* @author  Tian Chang & Luo Xiaoyi
* @version V1.0
* @date    2016.10.27
* @brief   用于加速浮点数运算
******************************************************************************
* @attention none
******************************************************************************
*/
/* Includes -------------------------------------------------------------------*/
#include "fastFloat.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Private  const		  ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
int32_t fastFloat::toInt32(void)
{
	return dataInt32 / FAST_FLOAT_AMP;
}
float	fastFloat::toFloat(void)
{
	return static_cast<float>(dataInt32) / FAST_FLOAT_AMP;
}
fastFloat operator + (const fastFloat& x, const fastFloat& y)
{
	fastFloat re;
	re.dataInt32 = x.dataInt32 + y.dataInt32;
	return re;
}
fastFloat operator - (const fastFloat& x, const fastFloat& y)
{
	fastFloat re;
	re.dataInt32 = x.dataInt32 - y.dataInt32;
	return re;
}
fastFloat operator * (const fastFloat& x, const fastFloat& y)
{
	fastFloat re;
	re.dataInt32 = static_cast<int64_t>(x.dataInt32)*y.dataInt32 / FAST_FLOAT_AMP;
	return re;
}
fastFloat operator / (const fastFloat& x, const fastFloat& y)
{
	fastFloat re;
	re.dataInt32 = static_cast<int64_t>(x.dataInt32)*FAST_FLOAT_AMP / y.dataInt32;
	return re;
}
uint8_t 	operator > (const fastFloat& x, const fastFloat& y)
{
	return (x.dataInt32 > y.dataInt32);
}
uint8_t 	operator < (const fastFloat& x, const fastFloat& y)
{
	return (x.dataInt32 < y.dataInt32);
}
void fastFloat::toFastFloat(float* data, uint32_t len)
{
	for (uint32_t i = 0; i<len; i++)
	{
		*(reinterpret_cast<int32_t *>(data)) = static_cast<int32_t>((*data)*FAST_FLOAT_AMP);
	}
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/

