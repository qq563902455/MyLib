/**
******************************************************************************
* @file    fastFloat.h
* @author  Tian Chang & Luo Xiaoyi
* @version V1.0
* @date    2017.5.9
* @brief   This file contains the headers of can.cpp
******************************************************************************
* @attention
*		该类用于加速浮点数运算
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FASTFLOAT_H
#define __FASTFLOAT_H
/* C&C++ ---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FAST_FLOAT_AMP  10000
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
/* Includes ------------------------------------------------------------------*/
/* Exported class ------------------------------------------------------------*/
class fastFloat
{
private:
	int32_t dataInt32;
public:
	//fastFloat(float val) :dataInt32(static_cast<int32_t>(val*FAST_FLOAT_AMP)) {}
	fastFloat(int val=0) :dataInt32(static_cast<int32_t>(val*FAST_FLOAT_AMP)) {}
	fastFloat(const fastFloat& x) :dataInt32(x.dataInt32) {};
	int32_t toInt32(void);
	float	toFloat(void);
	static void toFastFloat(float* data, uint32_t len = 1);
	~fastFloat() = default;
	inline void operator = (const fastFloat& y) { this->dataInt32 = y.dataInt32; }
	inline void operator = (float y) { this->dataInt32 = static_cast<int32_t>(y*FAST_FLOAT_AMP); }
	inline void operator = (int32_t y) { this->dataInt32 = static_cast<int32_t>(y*FAST_FLOAT_AMP); }
	friend fastFloat operator + (const fastFloat& x, const fastFloat& y);
	friend fastFloat operator - (const fastFloat& x, const fastFloat& y);
	friend fastFloat operator * (const fastFloat& x, const fastFloat& y);
	friend fastFloat operator / (const fastFloat& x, const fastFloat& y);
	friend uint8_t 	 operator > (const fastFloat& x, const fastFloat& y);
	friend uint8_t 	 operator < (const fastFloat& x, const fastFloat& y);

	template<typename type>
	inline friend fastFloat operator + (const fastFloat& x, const type& y)
	{
		fastFloat re;
		re.dataInt32 = x.dataInt32 + y*FAST_FLOAT_AMP;
		return re;
	}
	template<typename type>
	inline friend fastFloat operator + (const type& y, const fastFloat& x) { return (x + y); }
	template<typename type>
	inline friend fastFloat operator - (const fastFloat& x, const type& y) { return (x + (-y)); }
	template<typename type>
	inline friend fastFloat operator - (const type& y, const fastFloat& x)
	{
		fastFloat re;
		re.dataInt32 = -x.dataInt32 + y*FAST_FLOAT_AMP;
		return re;
	}

	template<typename type>
	inline friend fastFloat operator * (const fastFloat& x, const type& y)
	{
		fastFloat re;
		re.dataInt32 = x.dataInt32*y;
		return re;
	}
	template<typename type>
	inline friend fastFloat operator * (const type& y, const fastFloat& x) { return x*y; }

	template<typename type>
	inline friend fastFloat operator / (const fastFloat& x, const type& y)
	{
		fastFloat re;
		re.dataInt32 = x.dataInt32 / y;
		return re;
	}
	template<typename type>
	inline friend fastFloat operator / (const type& y, const fastFloat& x)
	{
		fastFloat re;
		re.dataInt32 = static_cast<int64_t>(y*FAST_FLOAT_AMP)*FAST_FLOAT_AMP / x.dataInt32;
		return re;
	}

	template<typename type>
	inline friend uint8_t operator > (const fastFloat& x, const type& y) { return (x.dataInt32>y); }
	template<typename type>
	inline friend uint8_t operator > (const type& y, const fastFloat& x) { return (x.dataInt32<y); }
	template<typename type>
	inline friend uint8_t operator < (const fastFloat& x, const type& y) { return (x.dataInt32<y); }
	template<typename type>
	inline friend uint8_t operator < (const type& y, const fastFloat& x) { return (x.dataInt32>y); }
};
#endif
#endif

/******************* (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
