/**
******************************************************************************
* @file    action_control.cpp
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
#include "action_math.h"
#include "action_matrix.h"
#include "math.h"
#include "action_control.h"
#include <cstring>
#include "stdint.h"

//#include <iostream>
//
//using namespace std;
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
static uint8_t na = 0;
static uint8_t nb = 0;
static uint8_t nd = 0;
static action_matrix *P;
static action_matrix *ab;
static action_matrix *K;
static action_matrix *data;
static float *in_st;
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/**
* @brief  基于递归最小二乘法的系统模型辨识
                 b1+b2*z^-1+b3*z^-2......+bnB*z^-(nB-1)
     G(z)=    -------------------------------------------- * z^-d
	           1+a1*z^-1+a2*z^-2...............+anA*z^-nA
* @param  nA: 脉冲传递函数分母阶数
          nB: 脉冲传递函数分子阶数+1
		  d : 脉冲传递函数纯滞后
* @retval none
*/
void input_Order_Model(uint8_t nA,uint8_t nB,uint8_t d)
{
	/* 输入系统模型的参数 */
	na = nA;
	nb = nB - 1;
	nd = d;

	/* 初始化递归最小二乘法需要的矩阵 */
	data = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	ab   = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	K    = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	P    = new action_matrix(na + nb + 1, na + nb + 1, MATRIX_I);

	/* 初始化使P矩阵足够大 */
	(*P) = (*P) * 1000000;

	/* 为系统的纯滞后分配内存，用于存储滞后时间内的系统输入输出数据 */
	in_st = new float[d];
	for (uint8_t i = 0; i < d; i++)
	{
		in_st[0] = 0;
	}

}
/**
* @brief  输入辨识使用的数据
* @param  in: 系统输入
          out: 系统输出
* @retval none
*/
void input_in_out_data(float in, float out)
{
	/* 更新系统输入数据 */
	for (uint8_t i = 0; i < nb - 1; i++)
	{
		data->set_data(na + i, 0, data->get_data(na + i + 1, 0));
	}
	if (nd != 0)
	{
		data->set_data(na, 0, in_st[nd - 1]);
		memcpy(in_st + 1, in_st, (nd - 1) * 4);
		in_st[0] = in;
	}
	else
	{
		data->set_data(na, 0, in);
	}

	/* 计算K */
	(*K)  = ((*P)*(*data)) * (~(1 + (!(*data))*(*P)*(*data)));

	/* 计算递归结果，即a,b的值 */
	(*ab) = (*ab) + (*K)*(out - (!(*data))*(*ab));
	//(*ab).PrintfItself();


	/* 更新P矩阵 */
	(*P) = (1 - (*K)*(!(*data)))*(*P);

    /* 更新系统输出数据 */
	for (uint8_t i = 0; i < na - 1; i++)
	{
		data->set_data(i, 0, data->get_data(i + 1, 0));
	}
	data->set_data(0, 0, -out);
}
/**
  * @brief  获得辨识的结果
  * @param  none
  * @retval ab=[a1,a2,a3.....anA,b1,b2,b3......bnB]'
*/
float *getSystem()
{
	/* 将辨识结果转移到数组中 */
	float *result = new float[(*ab).get_row()];
	for (uint8_t i = 0; i < (*ab).get_row(); i++)
	{
		result[i] = (*ab).get_data(i, 0);
	}

	/* 释放矩阵中的数据 */
	(*P).delete_data();
	(*ab).delete_data();
	(*K).delete_data();
	(*data).delete_data();

	/* 释放矩阵以及纯滞后需要的数据 */
    delete[] in_st;
    delete  P;
	delete  ab;
	delete  K;
	delete  data;

	return result;
}
/* Exported class functions ---------------------------------------------------*/
/**
* @brief  初始化一个模型
*                 num[0]*z^(LenN-1)+num[1]*z^(LenN-2)......+num[LenN-1]*z^(0)
*        G(z)=  --------------------------------------------------------------
*                 den[0]*z^(LenD-1)+den[1]*z^(LenD-2)......+den[LenD-1]*z^(0)
* @param  cd:   表示模型是离散还是连续
                ContinueSys  连续模型
                DiscreteSys  离散模型
* @param 		num： 传递函数分子数组的指针
* @param 	    den： 传递函数分母数组的指针
* @param 		LenN: 传递函数分子数组的长度
* @param 		LenD: 传递杉树分母数组的长度
* @retval none
*/
control_model::control_model(uint8_t cd, float* num, float* den, uint8_t LenN, uint8_t LenD)
{
	d_or_c = cd;
	len_num = LenN;
	len_den = LenD;
	Num = new float[LenN];
	Den = new float[LenD];

	for (uint8_t i = 0; i < LenN; i++)
	{
		Num[i] = num[i];
	}
	for (uint8_t i = 0; i < LenD; i++)
	{
		Den[i] = den[i];
	}
}

/**
* @brief   初始化一个模型，初始化成系统辨识需要的形式
                       b1+b2*z^-1+b3*z^-2......+bnB*z^-(nB-1)
           G(z)=    -------------------------------------------- * z^-d
                     1+a1*z^-1+a2*z^-2...............+anA*z^-nA
* @param  na: 脉冲传递函数分母阶数
          nb: 脉冲传递函数分子阶数+1
          nd: 脉冲传递函数纯滞后
* @retval none
*/
control_model::control_model(uint8_t cd, uint8_t nA, uint8_t nB, uint8_t nd)
{ 
	d_or_c = cd;
	if (nA - nB + 1 - nd>0)
	{
		len_num = nA - nB + 1 - nd + 1;
		len_den = nA + 1;
	}
	else
	{
		len_num = nB;
		len_den = -(-nB + 1 - nd) + 1;
	}

	Num = new float[len_num];
	Den = new float[len_den];
	input_Order_Model(nA, nB, nd);
}
/**
* @brief   析构函数，释放前面申请的动态内存
* @retval  none
*/
control_model::~control_model()
{
	delete[] Num;
	delete[] Den;
}
/**
* @brief   得到模型分子数组的长度
* @retval  len_num: 分子数组的长度
*/
uint8_t control_model::getLenNum() const
{
	return len_num;
}
/**
* @brief   得到模型分母数组的长度
* @retval  len_den: 分母数组的长度
*/
uint8_t control_model::getLenDen() const
{
	return len_den;
}
/**
* @brief   得到模型分子数组的指针
* @retval  Num: 分子数组的指针
*/
float*  control_model::getNumP()   const
{
	return Num;
}
/**
* @brief   得到模型分母数组的指针
* @retval  Num: 分母数组的指针
*/
float*  control_model::getDenP()   const
{
	return Den;
}
/**
* @brief   得到模型的类型
* @retval  d_or_c: 模型的类型
                   ContinueSys： 连续系统
				   DiscreteSys： 离散系统           
*/
uint8_t control_model::getCD()     const
{
	return d_or_c;
}
/**
* @brief   输入模型辨识时需要的数据
* @param   in:  模型的输入数据
           out: 模型的输出数据
* @retval  none
*/
void control_model::model_iddata(float in, float out) const
{
	input_in_out_data(in, out);
}
/**
* @brief   以之前输入的数据，还有前面定下来的系统阶数为基础，计算模型具体参数
* @param   none
* @retval  none
*/
void control_model::model_ident(void)
{
	float *re;

	/* 得到模型的na,nb,nd值 */
	re = getSystem();

	for (uint8_t i = 0; i < len_den; i++)
	{
		if (i < na + 1)
		{
			if (i == 0)
			{
				Den[0] = 1;
			}
			else
			{
				Den[i] = re[i - 1];
			}
		}
		else
		{
			Den[i] = 0;
		}
	}
	for (uint8_t i = 0; i < len_num; i++)
	{
		if (i < nb + 1)
		{
			Num[i] = re[na + i];
		}
		else
		{
			Num[i] = 0;
		}
	}
}

/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
