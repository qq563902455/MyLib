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
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
#define DEBUG
#ifdef DEBUG
#include <iostream>
using namespace std;
#define _log_(str) cout<<str<<endl;
#else
#define _log_(str)	
#endif
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
static uint8_t na = 0;
static uint8_t nb = 0;
static uint8_t nd = 0;
static action_matrix *ab;
static action_matrix *y;
static action_matrix *prdata;
static float *in_st;
static uint32_t count_ident = 0;
static float out_last = 0;
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/**
* @brief  ���ڵݹ���С���˷���ϵͳģ�ͱ�ʶ
                 b1+b2*z^-1+b3*z^-2......+bnB*z^-(nB-1)
     G(z)=    -------------------------------------------- * z^-d
	           1+a1*z^-1+a2*z^-2...............+anA*z^-nA
* @param  nA: ���崫�ݺ�����ĸ����
          nB: ���崫�ݺ������ӽ���+1
		  d : ���崫�ݺ������ͺ�
* @retval none
*/
void input_Order_Model(uint8_t nA,uint8_t nB,uint8_t d, uint32_t num)
{
	/* ����ϵͳģ�͵Ĳ��� */
	na = nA;
	nb = nB;
	nd = d;
	count_ident = 0;
	out_last = 0;

	_log_("nA:" << (int)na);
	_log_("nB:" << (int)nb);
	_log_("nd:" << (int)d);

	/* ��ʼ���ݹ���С���˷���Ҫ�ľ��� */
	prdata = new action_matrix(num, na + nb, MATRIX_ZERO);
	ab = new action_matrix(na + nb, 1, MATRIX_ZERO);
	y = new action_matrix(num, 1, MATRIX_ZERO);


	/* Ϊϵͳ�Ĵ��ͺ�����ڴ棬���ڴ洢�ͺ�ʱ���ڵ�ϵͳ����������� */
	in_st = new float[d];
	for (uint8_t i = 0; i < d; i++)
	{
		in_st[0] = 0;
	}

}
/**
* @brief  �����ʶʹ�õ�����
* @param  in: ϵͳ����
          out: ϵͳ���
* @retval none
*/
void input_in_out_data(float in, float out)
{
	y->set_data(count_ident, 0, out);
	if (count_ident != 0)
	{
		/* ����ϵͳ�������� */
		for (uint8_t i = 0; i < nb - 1; i++)
		{
			prdata->set_data(count_ident, na + nb - 1 - i, prdata->get_data(count_ident - 1, na + nb - i - 2));
		}
		if (nd != 0)
		{
			prdata->set_data(count_ident, na, in_st[nd - 1]);
			memcpy(in_st + 1, in_st, (nd - 1) * 4);
			in_st[0] = in;
		}
		else
		{
			prdata->set_data(count_ident, na, in);
		}
		/* ����ϵͳ������� */
		for (uint8_t i = 0; i < na - 1; i++)
		{
			prdata->set_data(count_ident, na - i - 1, prdata->get_data(count_ident - 1, na - i - 2));
		}
		prdata->set_data(count_ident, 0, -out_last);
	}
	else
	{
		/* ����ϵͳ�������� */
		for (uint8_t i = 0; i < nb - 1; i++)
		{
			prdata->set_data(count_ident, na + nb - 1 - i,0);
		}
		if (nd != 0)
		{
			prdata->set_data(na, 0, in_st[nd - 1]);
			memcpy(in_st + 1, in_st, (nd - 1) * 4);
			in_st[0] = in;
		}
		else
		{
			prdata->set_data(count_ident, na, in);
		}
		/* ����ϵͳ������� */
		for (uint8_t i = 0; i < na - 1; i++)
		{
			prdata->set_data(count_ident, na - i - 1, 0);
		}
		prdata->set_data(count_ident, 0, -out_last);
	}
	out_last = out;
	count_ident++;
}
/**
  * @brief  ��ñ�ʶ�Ľ��
  * @param  none
  * @retval ab=[a1,a2,a3.....anA,b1,b2,b3......bnB]'
*/
float *getSystem()
{
	/* ����ʶ���ת�Ƶ������� */
	float *result = new float[(*ab).get_row()];
	(*ab) = (~(!(*prdata)*(*prdata)))*(!(*prdata))*(*y);

	for (uint32_t i = 0; i < (*ab).get_row(); i++)
	{
		result[i] = (float)(*ab).get_data(i, 0);
		_log_("re: "<<i<<": "<< result[i]);
	}


	/* �ͷž����Լ����ͺ���Ҫ������ */
    delete[] in_st;
	delete  ab;
	delete  prdata;
	delete  y;

	return result;
}
/* Exported class functions ---------------------------------------------------*/
/**
* @brief  ��ʼ��һ��ģ��
*                 num[0]*z^(LenN-1)+num[1]*z^(LenN-2)......+num[LenN-1]*z^(0)
*        G(z)=  --------------------------------------------------------------
*                 den[0]*z^(LenD-1)+den[1]*z^(LenD-2)......+den[LenD-1]*z^(0)
* @param  cd:   ��ʾģ������ɢ��������
                ContinueSys  ����ģ��
                DiscreteSys  ��ɢģ��
* @param 		num�� ���ݺ������������ָ��
* @param 	    den�� ���ݺ�����ĸ�����ָ��
* @param 		LenN: ���ݺ�����������ĳ���
* @param 		LenD: ���ݺ�����ĸ����ĳ���
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
* @brief   ��ʼ��һ��ģ�ͣ���ʼ����ϵͳ��ʶ��Ҫ����ʽ
                       b1+b2*z^-1+b3*z^-2......+bnB*z^-(nB-1)
           G(z)=    -------------------------------------------- * z^-d
                     1+a1*z^-1+a2*z^-2...............+anA*z^-nA
* @param  na: ���崫�ݺ�����ĸ����
          nb: ���崫�ݺ������ӽ���+1
          nd: ���崫�ݺ������ͺ�
* @retval none
*/
control_model::control_model(uint8_t cd, uint8_t nA, uint8_t nB, uint8_t nd, uint32_t num_ident)
{ 
	d_or_c = cd;
	if (nA - nB - nd + 1 >= 0)
	{
		len_num = nA - nd + 1;
		len_den = nA + 1;
	}
	else
	{
		len_num = nB + nd;
		len_den = nB + nd;
	}

	Num = new float[len_num];
	Den = new float[len_den];
	input_Order_Model(nA, nB, nd, num_ident);
}
/**
* @brief   �����������ͷ�ǰ������Ķ�̬�ڴ�
* @retval  none
*/
control_model::~control_model()
{
	delete[] Num;
	delete[] Den;
	delete[] out_simu;
	delete[] in_simu;
}
/**
* @brief   �õ�ģ�ͷ�������ĳ���
* @retval  len_num: ��������ĳ���
*/
uint8_t control_model::getLenNum() const
{
	return len_num;
}
/**
* @brief   �õ�ģ�ͷ�ĸ����ĳ���
* @retval  len_den: ��ĸ����ĳ���
*/
uint8_t control_model::getLenDen() const
{
	return len_den;
}
/**
* @brief   �õ�ģ�ͷ��������ָ��
* @retval  Num: ���������ָ��
*/
float*  control_model::getNumP()   const
{
	return Num;
}
/**
* @brief   �õ�ģ�ͷ�ĸ�����ָ��
* @retval  Num: ��ĸ�����ָ��
*/
float*  control_model::getDenP()   const
{
	return Den;
}
/**
* @brief   �õ�ģ�͵�����
* @retval  d_or_c: ģ�͵�����
                   ContinueSys�� ����ϵͳ
				   DiscreteSys�� ��ɢϵͳ           
*/
uint8_t control_model::getCD()     const
{
	return d_or_c;
}
/**
* @brief   ����ģ�ͱ�ʶʱ��Ҫ������
* @param   in:  ģ�͵���������
           out: ģ�͵��������
* @retval  none
*/
void control_model::model_iddata(float in, float out) const
{
	input_in_out_data(in, out);
}
/**
* @brief   ��֮ǰ��������ݣ�����ǰ�涨������ϵͳ����Ϊ����������ģ�;������
* @param   none
* @retval  none
*/
void control_model::model_ident(void)
{
	float *re;

	/* �õ�ģ�͵�na,nb,ndֵ */
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
		if (i < nb)
		{
			Num[i] = re[na + i];
		}
		else
		{
			Num[i] = 0;
		}
	}
}
float control_model::step(float in, uint8_t cmd)
{
	static uint8_t flag = 0;
	float out = 0;
	if (cmd == CLEARSTEP)
	{
		delete[] out_simu;
		delete[] in_simu;
		flag = 0;
	}
	else if (cmd == STEPING)
	{
		uint8_t temp_len = 0;
		if (len_den > len_num)
		{
			temp_len = len_den;
		}
		else
		{
			temp_len = len_num;
		}
		if (flag == 0)
		{
			out_simu = new float[temp_len - 1];
			in_simu = new float[temp_len];

			for (uint8_t i = 0; i < temp_len; i++)
			{
				if (i < temp_len - 1)
				{
					out_simu[i] = 0;
				}
				in_simu[i] = 0;
			}
			flag++;
		}

		memcpy(in_simu, in_simu + 1, 4 * (temp_len - 1));
		in_simu[temp_len - 1] = in;

		for (uint8_t i = 0; i < len_num; i++)
		{
				out += in_simu[len_num - 1 - i] * Num[i];	
		}

		if (len_den > 1)
		{
			for (uint8_t i = 1; i < len_den; i++)
			{
				out -= out_simu[len_den - i - 1] * Den[i];
			}
			memcpy(out_simu, out_simu + 1, 4 * (temp_len - 2));
			out_simu[len_den - 2] = out;
		}

		out /= Den[0];
	}
	return out;
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
