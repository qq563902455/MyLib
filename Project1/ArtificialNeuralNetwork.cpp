/**
******************************************************************************
* @file    ArtificialNeuralNetwork.cpp
* @author  lxy ACTION
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
#include "ArtificialNeuralNetwork.h"
#include <math.h>
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
#define a_study 0.5  //学习步长
#define a_keep  0.05  //惯性量，动量
//#define DEBUG
#ifdef DEBUG
	#include <iostream>
	using namespace std;
	#define _log_(str) cout<<str<<endl;
#else
	#define _log_(str)	
#endif

/* Private  macro -------------------------------------------------------------*/
/* 激活函数及其导数 */
#define SIGMOID(x) ((float)1/(float)(1+exp(-x)))
#define DIFF_SIGMOID(x) ((float)(exp(-x)/(1+exp(-x))/(1+exp(-x))))
/* Private  variables ---------------------------------------------------------*/
static float **w_L_last;
static float **w_R_last;
static float *b_L_last;
static float *b_R_last;
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/* Exported class functions ---------------------------------------------------*/
/**
* @brief   初始化搭建神经网络
* @param   in:       神经网络的输入层节点数
           hide:     隐藏层节点数
		   out:      输出层节点数
		   data_l:   隐藏层节点输入权重
		   data_r:   输出层节点输入权重
		   bL:       隐藏层节点权重
		   bR:       输出层节点权重
* @retval  none
*/
BP_ANN::BP_ANN(uint8_t in,     uint8_t hide,    uint8_t out, 
	           float **data_l, float **data_r, 
	           float *bL,      float *bR)
{
	in_port = in;
	hide_layer = hide;
	out_port = out;

	b_L = new float[hide_layer];
	b_R = new float[out_port];

	b_L_last = new float[hide_layer];
	b_R_last = new float[out_port];

	for (uint16_t i = 0; i < hide_layer; i++)
	{
		b_L[i] = bL[i];
		b_L_last[i] = bL[i];
	}
	for (uint16_t i = 0; i < out_port; i++)
	{
		b_R[i] = bR[i];
		b_R_last[i] = bR[i];
	}

	w_L = new float *[in];
	w_L_last = new float *[in];
	for (uint8_t i = 0; i < in; i++)
	{
		w_L[i] = new float[hide];
		w_L_last[i] = new float[hide];
	}

	w_R = new float *[hide];
	w_R_last = new float *[hide];
	for (uint8_t i = 0; i < hide; i++)
	{
		w_R[i] = new float[out];
		w_R_last[i] = new float[out];
	}
	for (uint8_t i = 0; i < in; i++)
	{
		for (uint8_t j = 0; j < hide; j++)
		{
			w_L[i][j] = data_l[i][j];
			w_L_last[i][j] = data_l[i][j];
		}
	}
	for (uint8_t i = 0; i < hide; i++)
	{
		for (uint8_t j = 0; j < out; j++)
		{
			w_R[i][j] = data_r[i][j];
			w_R_last[i][j] = data_r[i][j];
		}
	}
}
/**
* @brief   初始化搭建神经网络
* @param   in:       神经网络的输入层节点数
		   hide:     隐藏层节点数
		   out:      输出层节点数
* @retval  none
*/
BP_ANN::BP_ANN(uint8_t in, uint8_t hide, uint8_t out)
{
	in_port = in;
	hide_layer = hide;
	out_port = out;

	b_L = new float[hide_layer];
	b_R = new float[out_port];

	b_L_last = new float[hide_layer];
	b_R_last = new float[out_port];

	for (uint16_t i = 0; i < hide_layer; i++)
	{
		b_L[i] = -0.1f;
		b_L_last[i] = -0.1f;
	}
	for (uint16_t i = 0; i < out_port; i++)
	{
		b_R[i] = -0.1f;
		b_R_last[i] = -0.1f;
	}

	w_L = new float *[in];
	w_L_last = new float *[in];
	for (uint8_t i = 0; i < in; i++)
	{
		w_L[i] = new float[hide];
		w_L_last[i] = new float[hide];
	}

	w_R = new float *[hide];
	w_R_last = new float *[hide];
	for (uint8_t i = 0; i < hide; i++)
	{
		w_R[i] = new float[out];
		w_R_last[i] = new float[out];
	}
	for (uint8_t i = 0; i < in; i++)
	{
		for (uint8_t j = 0; j < hide; j++)
		{
			w_L[i][j] = 0.1f;
			w_L_last[i][j] = 0.1f;
		}
	}
	for (uint8_t i = 0; i < hide; i++)
	{
		for (uint8_t j = 0; j < out; j++)
		{
			w_R[i][j] = 0.1f;
			w_R_last[i][j] = 0.1f;
		}
	}
}
/**
* @brief   释放神经网络
* @param   none
* @retval  none
*/
BP_ANN::~BP_ANN()
{
	for (uint8_t i = 0; i < in_port; i++)
	{
		delete[] w_L[i];
		delete[] w_L_last[i];
	}
	delete[] w_L;
	delete[] w_L_last;
	

	for (uint8_t i = 0; i < hide_layer; i++)
	{
		delete[] w_R[i];
		delete[] w_R_last[i];
	}
	delete[] w_R;
	delete[] w_R_last;

	delete[] b_L;
	delete[] b_R;

	delete[] b_L_last;
	delete[] b_R_last;
}
/**
* @brief   给一组输入得到神经网络的输出
* @param   indata:  神经网络的输入
* @retval  outdata: 神经网络的输出
*/
void BP_ANN::out(float *indata,float *outdata)
{
	float *hide_out;
	hide_out = new float[hide_layer];

	/* 计算隐藏层节点输出 */
	for (uint8_t i = 0; i < hide_layer; i++)
	{
		hide_out[i] = 0;
		for (uint8_t j = 0; j < in_port; j++)
		{
			hide_out[i] += indata[j]*w_L[j][i];
		}
		hide_out[i] += b_L[i];
	    hide_out[i] = SIGMOID(hide_out[i]);
	}	            

	/* 计算输出层节点输出 */
	for (uint8_t i = 0; i < out_port; i++)
	{
		outdata[i] = 0;
		for (uint8_t j = 0; j < hide_layer; j++)
		{
			outdata[i] += hide_out[j]*w_R[j][i];
		}
		outdata[i] += b_R[i];
		outdata[i] = SIGMOID(outdata[i]);
	}
	
	delete[] hide_out;
}
void BP_ANN::study(float *indata, const float *outdata)
{
	float *hide_out;  //正确输出（训练）
	float *study_out; //神经网络本身输出

	float *hide_v;    //隐藏层节点的V
	float *out_v;     //输出层节点的V

	hide_out = new float[hide_layer];
	hide_v = new float[hide_layer];

	study_out = new float[out_port];
	out_v = new float[out_port];

	for (uint8_t i = 0; i < hide_layer; i++)
	{
		/* 计算隐藏层V */
		hide_v[i] = 0.0;
		for (uint8_t j = 0; j < in_port; j++)
		{
			hide_v[i] += indata[j] * w_L[j][i];
		}
		hide_v[i] += b_L[i];
		_log_("hide_layer_v:"<<hide_v[i]);
		/* 计算隐藏层输出 */
		hide_out[i] = SIGMOID(hide_v[i]);
		_log_("hide_layer_out"<<hide_out[i]);
	}


	for (uint8_t i = 0; i < out_port; i++)
	{
		/* 计算输出层V */
		out_v[i] = 0;
		for (uint8_t j = 0; j < hide_layer; j++)
		{
			out_v[i] += hide_out[j] * w_R[j][i];
		}
		out_v[i] += b_R[i];
		_log_("out_layer_v:" << out_v[i]);
		/* 计算输出层输出 */
		study_out[i] = SIGMOID(out_v[i]);
		_log_("out_layer_v:" << study_out[i]);
	}

	/* 反向传播 */
	for (uint16_t i = 0; i < hide_layer; i++)
	{
		float temp = 0;
		float temp_w_L;
		float temp_b_L;
		for (uint16_t k = 0; k < out_port; k++)
		{
			temp += (float)((outdata[k] - study_out[k])*DIFF_SIGMOID(out_v[k])*w_R[i][k]);
		}
		/* 改变隐藏层的输入链接权重 */
		for (uint16_t j = 0; j < in_port; j++)
		{

			temp_w_L = w_L[j][i];
			w_L[j][i] += (float)(a_study*indata[j] * DIFF_SIGMOID(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
			w_L_last[j][i] = temp_w_L;

			_log_("w_L:" << j << i << ':' << w_L[j][i]);
		}
		/* 改变隐藏层的阈值 */
		temp_b_L = b_L[i];
		b_L[i] += (float)(a_study*DIFF_SIGMOID(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
		b_L_last[i] = temp_b_L;

		_log_("b_L:" << i << ':' << b_L[i]);
	}
	for (uint16_t i = 0; i < out_port; i++)
	{
		float temp_w_R;
		float temp_b_R;
		/* 改变输出层的输入链接权重 */
		for (uint16_t j = 0; j < hide_layer; j++)
		{
			temp_w_R = w_R[j][i];
			w_R[j][i] += (float)(a_study*(outdata[i] - study_out[i])*hide_out[j] * DIFF_SIGMOID(out_v[i]))+a_keep*(w_R[j][i]- w_R_last[j][i]);
			w_R_last[j][i] = temp_w_R;

			_log_("w_R:" << j << i <<':' << w_R[j][i]);
		}
		/* 改变输出层的阈值 */
		temp_b_R = b_R[i];
		b_R[i] += (float)(a_study*(outdata[i] - study_out[i])*  DIFF_SIGMOID(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
		b_R_last[i] = temp_b_R;

		_log_("b_R:"<< i <<':'<< b_R[i]);
	}


	delete[] hide_out;
	delete[] hide_v;

	delete[] study_out;
	delete[] out_v;
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/