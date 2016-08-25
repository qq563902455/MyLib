/**
******************************************************************************
* @file    action_control.h
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
#ifndef __ACTION_CONTROL_H
#define __ACTION_CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ContinueSys 0
#define DiscreteSys 1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void input_Order_Model(uint8_t nA, uint8_t nB, uint8_t d, uint32_t num);
void input_in_out_data(float in, float out);
float *getSystem();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* Exported class ------------------------------------------------------------*/

class control_model
{
public:
	control_model(uint8_t cd, float* Num, float* Den, uint8_t LenN, uint8_t  LenD);
	control_model(uint8_t cd, uint8_t nA, uint8_t nB, uint8_t nd,   uint32_t num_ident);
	uint8_t getLenNum() const;
	uint8_t getLenDen() const;
	float*  getNumP()   const;
	float*  getDenP()   const;
	uint8_t getCD()     const;
	void model_iddata(float in,float out) const;
	void model_ident(void);
	~control_model();
private:
	uint8_t len_num;
	uint8_t len_den;
	float* Num;
	float* Den;
	uint8_t d_or_c;
	
};

#endif


#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
