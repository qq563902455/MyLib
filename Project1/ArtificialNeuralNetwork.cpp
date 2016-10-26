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
#include <time.h>

/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
//#define DEBUG
#ifdef DEBUG
	#include <iostream>
	using namespace std;
	#define _log_(str) cout<<str<<endl
#else
	#define _log_(str)	
#endif

#define  PRINTF_ENABLE

#ifdef PRINTF_ENABLE
	#ifdef DEBUG
    #else
		#include <iostream>
		using namespace std;
	#endif
    #define _printf_(str) cout<<str<<endl
#else
    #define _printf_(x)
#endif // PRINTF_ENABLE


/* Private  macro -------------------------------------------------------------*/
/* 激活函数及其导数 */
#define LOGSIG(x)      ((float)1/(1+exp(-x)))
#define DIFF_LOGSIG(x) ((float)(exp(-x)/(1+exp(-x))/(1+exp(-x))))

#define TANSIG(x)      ((float)(2/(1+exp(-2*x))-1))
#define DIFF_TANSIG(x) ((float)(4*exp(-2*x)/(1+exp(-2*x))/(1+exp(-2*x))))

#define PURELIN(x)      (x)
#define DIFF_PURELIN(x) (1)

/* Private  variables ---------------------------------------------------------*/

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
BP_ANN::BP_ANN(uint8_t in, uint8_t hide, uint8_t out, float **data_l,   float **data_r,
	                                                  float *bL,        float *bR,
	                                                  uint8_t hide_fun, uint8_t out_fun)
{
	in_port = in;
	hide_layer = hide;
	out_port = out;

	b_L = new float[hide_layer];
	b_R = new float[out_port];

	b_L_last = new float[hide_layer];
	b_R_last = new float[out_port];

	hidelayer_fun = hide_fun;
	outport_fun = out_fun;

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
BP_ANN::BP_ANN(uint8_t in, uint8_t hide, uint8_t out, uint8_t hide_fun, uint8_t out_fun)
{
	srand((int)time(0));

	in_port = in;
	hide_layer = hide;
	out_port = out;

	hidelayer_fun = hide_fun;
	outport_fun = out_fun;

	b_L = new float[hide_layer];
	b_R = new float[out_port];

	b_L_last = new float[hide_layer];
	b_R_last = new float[out_port];

	for (uint16_t i = 0; i < hide_layer; i++)
	{
		b_L[i] = rand() % 200 / 100.0f - 1;
		b_L_last[i] = b_L[i];
	}
	for (uint16_t i = 0; i < out_port; i++)
	{
		b_R[i] = rand() % 200 / 100.0f - 1;
		b_R_last[i] = b_R[i];
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
			w_L[i][j] = rand() % 200 / 100.0f - 1;
			w_L_last[i][j] = w_L[i][j];
		}
	}
	for (uint8_t i = 0; i < hide; i++)
	{
		for (uint8_t j = 0; j < out; j++)
		{
			w_R[i][j] = rand() % 200 / 100.0f - 1;
			w_R_last[i][j] = w_R[i][j];
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

		switch (hidelayer_fun)
		{
		  case FUN_LOGSIG:
			  hide_out[i] = LOGSIG(hide_out[i]);
			  break;
		  case FUN_TANSIG:
			  hide_out[i] = TANSIG(hide_out[i]);
			  break;
		  case FUN_PURELIN:
			  hide_out[i] = PURELIN(hide_out[i]);
			  break;
		  default:
			  hide_out[i] = LOGSIG(hide_out[i]);
			  break;
		}
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
		switch (outport_fun)
		{
			case FUN_LOGSIG:
				outdata[i] = LOGSIG(outdata[i]);
				break;
			case FUN_TANSIG:
				outdata[i] = TANSIG(outdata[i]);
				break;
			case FUN_PURELIN:
				outdata[i] = PURELIN(outdata[i]);
				break;
			default:
				outdata[i] = TANSIG(outdata[i]);
				break;
		}
	}
	
	delete[] hide_out;
}
float BP_ANN::study(float *indata, const float *outdata)
{
	float err = 0;

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
		switch (hidelayer_fun)
		{
			case FUN_LOGSIG:
				hide_out[i] = LOGSIG(hide_v[i]);
				break;
			case FUN_TANSIG:
				hide_out[i] = TANSIG(hide_v[i]);
				break;
			case FUN_PURELIN:
				hide_out[i] = PURELIN(hide_v[i]);
				break;
			default:
				hide_out[i] = LOGSIG(hide_v[i]);
				break;
		}
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
		switch (outport_fun)
		{
			case FUN_LOGSIG:
				study_out[i] = LOGSIG(out_v[i]);
				break;
			case FUN_TANSIG:
				study_out[i] = TANSIG(out_v[i]);
				break;
			case FUN_PURELIN:
				study_out[i] = PURELIN(out_v[i]);
			default:
				study_out[i] = TANSIG(out_v[i]);
				break;
		}
		_log_("out_layer_v:" << study_out[i]);

		err += (outdata[i] - study_out[i])*(outdata[i] - study_out[i]);
	}

	/* 反向传播 */
	for (uint16_t i = 0; i < hide_layer; i++)
	{
		float temp = 0;
		float temp_w_L;
		float temp_b_L;
		for (uint16_t k = 0; k < out_port; k++)
		{
			switch (outport_fun)
			{
				case FUN_LOGSIG:
					temp += (float)((outdata[k] - study_out[k])*DIFF_LOGSIG(out_v[k])*w_R[i][k]);
					break;
				case FUN_TANSIG:
					temp += (float)((outdata[k] - study_out[k])*DIFF_TANSIG(out_v[k])*w_R[i][k]);
					break;
				case FUN_PURELIN:
					temp += (float)((outdata[k] - study_out[k])*DIFF_PURELIN(out_v[k])*w_R[i][k]);
					break;
				default:
					temp += (float)((outdata[k] - study_out[k])*DIFF_TANSIG(out_v[k])*w_R[i][k]);
					break;
			}
		}
		/* 改变隐藏层的输入链接权重 */
		for (uint16_t j = 0; j < in_port; j++)
		{

			temp_w_L = w_L[j][i];
			switch (hidelayer_fun)
			{
				case FUN_LOGSIG:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				case FUN_TANSIG:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_TANSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				case FUN_PURELIN:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_PURELIN(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				default:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
			}

			w_L_last[j][i] = temp_w_L;

			_log_("w_L:" << j << i << ':' << w_L[j][i]);
		}
		/* 改变隐藏层的阈值 */
		temp_b_L = b_L[i];
		switch (hidelayer_fun)
		{
			case FUN_LOGSIG:
				b_L[i] += (float)(a_study*DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			case FUN_TANSIG:
				b_L[i] += (float)(a_study*DIFF_TANSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			case FUN_PURELIN:
				b_L[i] += (float)(a_study*DIFF_PURELIN(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			default:
				b_L[i] += (float)(a_study*DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
		}

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

			switch (outport_fun)
			{
				case FUN_LOGSIG:
					w_R[j][i] += (float)(a_study*(outdata[i] - study_out[i])*hide_out[j] * DIFF_LOGSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				case FUN_TANSIG:
					w_R[j][i] += (float)(a_study*(outdata[i] - study_out[i])*hide_out[j] * DIFF_TANSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				case FUN_PURELIN:
					w_R[j][i] += (float)(a_study*(outdata[i] - study_out[i])*hide_out[j] * DIFF_PURELIN(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				default:
					w_R[j][i] += (float)(a_study*(outdata[i] - study_out[i])*hide_out[j] * DIFF_TANSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
			}

			w_R_last[j][i] = temp_w_R;

			_log_("w_R:" << j << i <<':' << w_R[j][i]);
		}
		/* 改变输出层的阈值 */
		temp_b_R = b_R[i];

		switch (outport_fun)
		{
			case FUN_LOGSIG:
				b_R[i] += (float)(a_study*(outdata[i] - study_out[i])*  DIFF_LOGSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			case FUN_TANSIG:
				b_R[i] += (float)(a_study*(outdata[i] - study_out[i])*  DIFF_TANSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			case FUN_PURELIN:
				b_R[i] += (float)(a_study*(outdata[i] - study_out[i])*  DIFF_PURELIN(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			default:
				b_R[i] += (float)(a_study*(outdata[i] - study_out[i])*  DIFF_TANSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
		}
		b_R_last[i] = temp_b_R;

		_log_("b_R:"<< i <<':'<< b_R[i]);
	}


	delete[] hide_out;
	delete[] hide_v;

	delete[] study_out;
	delete[] out_v;

	return err;
}

float BP_ANN::study(const float *indata, const float err,const float *diff)
{
	float Err = 0;

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
		hide_v[i] = 0.0f;
		for (uint8_t j = 0; j < in_port; j++)
		{
			hide_v[i] += indata[j] * w_L[j][i];
		}
		hide_v[i] += b_L[i];
		_log_("hide_layer_v:" << hide_v[i]);
		/* 计算隐藏层输出 */
		switch (hidelayer_fun)
		{
			case FUN_LOGSIG:
				hide_out[i] = LOGSIG(hide_v[i]);
				break;
			case FUN_TANSIG:
				hide_out[i] = TANSIG(hide_v[i]);
				break;
			case FUN_PURELIN:
				hide_out[i] = PURELIN(hide_v[i]);
				break;
			default:
				hide_out[i] = LOGSIG(hide_v[i]);
				break;
		}
		_log_("hide_layer_out" << hide_out[i]);
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
		switch (outport_fun)
		{
			case FUN_LOGSIG:
				study_out[i] = LOGSIG(out_v[i]);
				break;
			case FUN_TANSIG:
				study_out[i] = TANSIG(out_v[i]);
				break;
			case FUN_PURELIN:
				study_out[i] = PURELIN(out_v[i]);
			default:
				study_out[i] = TANSIG(out_v[i]);
				break;
		}
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
			switch (outport_fun)
			{
				case FUN_LOGSIG:
					temp += (float)((err * diff[k])*DIFF_LOGSIG(out_v[k])*w_R[i][k]);
					break;
				case FUN_TANSIG:
					temp += (float)((err * diff[k])*DIFF_TANSIG(out_v[k])*w_R[i][k]);
					break;
				case FUN_PURELIN:
					temp += (float)((err * diff[k])*DIFF_PURELIN(out_v[k])*w_R[i][k]);
					break;
				default:
					temp += (float)((err * diff[k])*DIFF_TANSIG(out_v[k])*w_R[i][k]);
					break;
			}
		}
		/* 改变隐藏层的输入链接权重 */
		for (uint16_t j = 0; j < in_port; j++)
		{

			temp_w_L = w_L[j][i];
			switch (hidelayer_fun)
			{
				case FUN_LOGSIG:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				case FUN_TANSIG:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_TANSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				case FUN_PURELIN:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_PURELIN(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
				default:
					w_L[j][i] += (float)(a_study*indata[j] * DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(w_L[j][i] - w_L_last[j][i]);
					break;
			}

			w_L_last[j][i] = temp_w_L;

			_log_("w_L:" << j << i << ':' << w_L[j][i]);
		}
		/* 改变隐藏层的阈值 */
		temp_b_L = b_L[i];
		switch (hidelayer_fun)
		{
			case FUN_LOGSIG:
				b_L[i] += (float)(a_study*DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			case FUN_TANSIG:
				b_L[i] += (float)(a_study*DIFF_TANSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			case FUN_PURELIN:
				b_L[i] += (float)(a_study*DIFF_PURELIN(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
			default:
				b_L[i] += (float)(a_study*DIFF_LOGSIG(hide_v[i])*temp) + a_keep*(b_L[i] - b_L_last[i]);
				break;
		}

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

			switch (outport_fun)
			{
				case FUN_LOGSIG:
					w_R[j][i] += (float)(a_study*(err * diff[i])*hide_out[j] * DIFF_LOGSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				case FUN_TANSIG:
					w_R[j][i] += (float)(a_study*(err * diff[i])*hide_out[j] * DIFF_TANSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				case FUN_PURELIN:
					w_R[j][i] += (float)(a_study*(err * diff[i])*hide_out[j] * DIFF_PURELIN(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
				default:
					w_R[j][i] += (float)(a_study*(err * diff[i])*hide_out[j] * DIFF_TANSIG(out_v[i])) + a_keep*(w_R[j][i] - w_R_last[j][i]);
					break;
			}

			w_R_last[j][i] = temp_w_R;

			_log_("w_R:" << j << i << ':' << w_R[j][i]);
		}
		/* 改变输出层的阈值 */
		temp_b_R = b_R[i];

		switch (outport_fun)
		{
			case FUN_LOGSIG:
				b_R[i] += (float)(a_study*(err * diff[i])*  DIFF_LOGSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			case FUN_TANSIG:
				b_R[i] += (float)(a_study*(err * diff[i])*  DIFF_TANSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			case FUN_PURELIN:
				b_R[i] += (float)(a_study*(err * diff[i])*  DIFF_PURELIN(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
			default:
				b_R[i] += (float)(a_study*(err * diff[i])*  DIFF_TANSIG(out_v[i])) + a_keep*(b_R[i] - b_R_last[i]);
				break;
		}
		b_R_last[i] = temp_b_R;

		_log_("b_R:" << i << ':' << b_R[i]);
	}


	delete[] hide_out;
	delete[] hide_v;

	delete[] study_out;
	delete[] out_v;

	return Err;
}


void BP_ANN::printf(void)
{
	for (uint16_t i = 0; i < in_port; i++)
	{
		for (uint16_t j = 0; j < hide_layer; j++)
		{
			_printf_("wl[" << i << "][" << j << "]: " << w_L[i][j]);
		}
	}

	for (uint16_t i = 0; i < hide_layer; i++)
	{
		_printf_("bl[" << i << "]: "<< b_L[i]);
	}

	for (uint16_t i = 0; i < hide_layer; i++)
	{
		for (uint16_t j = 0; j < out_port; j++)
		{
			_printf_("wr[" << i << "][" << j << "]: " << w_R[i][j]);
		}
	}

	for (uint16_t i = 0; i < out_port; i++)
	{
		_printf_("br[" << i << "]: " << b_R[i]);
	}
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/