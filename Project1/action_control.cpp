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
* @brief  ���ڵݹ���С���˷���ϵͳģ�ͱ�ʶ
                 b1+b2*z^-1+b3*z^-2......+bnB*z^-(nB-1)
     G(z)=    -------------------------------------------- * z^-d
	           1+a1*z^-1+a2*z^-2...............+anA*z^-nA
* @param  nA: ���崫�ݺ�����ĸ����
          nB: ���崫�ݺ������ӽ���+1
		  d : ���崫�ݺ������ͺ�
* @retval none
*/
void input_Order_Model(uint8_t nA,uint8_t nB,uint8_t d)
{
	/* ����ϵͳģ�͵Ĳ��� */
	na = nA;
	nb = nB - 1;
	nd = d;

	/* ��ʼ���ݹ���С���˷���Ҫ�ľ��� */
	data = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	ab   = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	K    = new action_matrix(na + nb + 1, 1, MATRIX_ZERO);
	P    = new action_matrix(na + nb + 1, na + nb + 1, MATRIX_I);

	/* ��ʼ��ʹP�����㹻�� */
	(*P) = (*P) * 1000000;

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
	/* ����ϵͳ�������� */
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

	/* ����K */
	(*K)  = ((*P)*(*data)) * (~(1 + (!(*data))*(*P)*(*data)));

	/* ����ݹ�������a,b��ֵ */
	(*ab) = (*ab) + (*K)*(out - (!(*data))*(*ab));
	//(*ab).PrintfItself();


	/* ����P���� */
	(*P) = (1 - (*K)*(!(*data)))*(*P);

    /* ����ϵͳ������� */
	for (uint8_t i = 0; i < na - 1; i++)
	{
		data->set_data(i, 0, data->get_data(i + 1, 0));
	}
	data->set_data(0, 0, -out);
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
	for (uint8_t i = 0; i < (*ab).get_row(); i++)
	{
		result[i] = (*ab).get_data(i, 0);
	}

	/* �ͷž����е����� */
	(*P).delete_data();
	(*ab).delete_data();
	(*K).delete_data();
	(*data).delete_data();

	/* �ͷž����Լ����ͺ���Ҫ������ */
    delete[] in_st;
    delete  P;
	delete  ab;
	delete  K;
	delete  data;

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
* @param 		LenD: ����ɼ����ĸ����ĳ���
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
* @brief   �����������ͷ�ǰ������Ķ�̬�ڴ�
* @retval  none
*/
control_model::~control_model()
{
	delete[] Num;
	delete[] Den;
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
