/**
******************************************************************************
* @file    action_PID.cpp
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
#include "action_math.h"
#include "action_matrix.h"
#include "math.h"
#include "action_control.h"
#include <cstring>
#include "stdint.h"
#include "action_PID.h"
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
/* Exported class functions ---------------------------------------------------*/
/**
* @brief  �õ����������
* @param  err:  ������ֵ��ʵ��ֵ�Ĳ�
* @retval none
*/
float action_PID::out(float err)
{
	float err_vell=0;
	float out;

	err_sum += err;
	err_vell = err - last_err;
	last_err = err;

	if (err_sum*Ki >= err_sum_max)
	{
		err_sum = err_sum_max / Ki;
	}
	if (err_sum*Ki <= -err_sum_max)
	{
		err_sum = -err_sum_max / Ki;
	}
	out = Kp*err + Ki*err_sum + Kd*err_vell;

	if (out > out_max)
	{
		out = out_max;
	}
	else if (out < -1 * out_max)
	{
		out = -1 * out_max;
	}

	return out;
}
/**
* @brief  PI������������
* @param  g:   ���ض���
          wc:  ��Խ��ֹƵ��
		  rwc: ��λԣ��
		  T:   �������ڣ���λs
* @retval none
*/
void action_PID::autoTuning(control_model &g, float wc, float rwc,float T)
{
	double wwc;                   //��Ӧw�任��Ĵ�Խ��ֹƵ��
	double z_r, z_v, A_z, A_ph;   //����Z��w�����ʵ�����鲿��ģ������λ��

	wwc = 2 / T*tan(wc*T / 2);   //���㴩Խ��ֹƵ��

	z_r = (1 - T*T / 4 * wwc*wwc) / (1 + T*T / 4 * wwc*wwc);  //��������Z��ʵ��
	z_v = T*wwc / (1 + T*T / 4 * wwc*wwc);                    //��������Z���鲿
	A_z = sqrt(z_r*z_r + z_v*z_v);                            //��������Z��ģ��
	A_ph = atan2(z_v, z_r);                                   //��������Z����λ��

	double R_t = 0, V_t = 0, R_b = 0, V_b = 0;  //���ض�����ӵ�ʵ�����鲿�����ض����ĸ��ʵ�����鲿
	double temp_A,temp_ph;
	double A_g,ph_g;

	/*  ����ʵ���鲿����  */
	for (uint16_t i = 1; i <= g.getLenNum(); i++)
	{
		temp_A = pow(A_z, g.getLenNum() - i);
		temp_ph = A_ph*(g.getLenNum() - i);
		R_t += g.getNumP()[i - 1] * temp_A*cos(temp_ph);
		V_t += g.getNumP()[i - 1] * temp_A*sin(temp_ph);
	}
	/*  ��ĸʵ���鲿����  */
	for (uint16_t i = 1; i <= g.getLenDen(); i++)
	{
		temp_A = pow(A_z, g.getLenDen() - i);
		temp_ph = A_ph*(g.getLenDen() - i);
		R_b += g.getDenP()[i - 1] * temp_A*cos(temp_ph);
		V_b += g.getDenP()[i - 1] * temp_A*sin(temp_ph);
	}
	ph_g = atan2(V_t, R_t) - atan2(V_b, R_b);                   //���㱻�ض������λ��
	A_g = sqrt(V_t*V_t + R_t *R_t) / sqrt(V_b*V_b + R_b*R_b);   //���㱻�ض����ģ��

	/* PI�Ŀ�����ģ������λ�ǣ��Լ�PI������PI�����ֵ�ļ��� */
	double ph_pi,Rate_pi;

	temp_A = sqrt(pow((T*wwc / (1 + T*T / 4 * wwc*wwc)), 2) + pow((T*T / 2 * wwc*wwc / (1 + T*T / 4 * wwc*wwc)), 2));
	temp_ph = atan2((T*wwc / (1 + T*T / 4 * wwc*wwc)), -(T*T / 2 * wwc*wwc / (1 + T*T / 4 * wwc*wwc)));

	temp_A = T / temp_A;
	temp_ph = -temp_ph;

	temp_A = temp_A*A_z;
	temp_ph += A_ph;


	rwc = rwc / 180.0f*3.1415926f;                                                  //��������λԣ�ȴӽǶȱ�ɻ���
	ph_pi = rwc - 3.141592 - ph_g;                                                  //����PI��������Ҫ����λ��
	Rate_pi = tan(ph_pi) / (temp_A*sin(temp_ph) - temp_A*tan(ph_pi)*cos(temp_ph));  //����P��I�ı���

	Ki = (float)(sin(ph_pi) / A_g / temp_A / sin(temp_ph));  //�õ�����Ki
	Kp = (float)(Ki / Rate_pi);                              //�õ�����Kp
	Kd = 0.0f;

	Ki *= T;
}
/**
  * @brief  ��ÿ������ķ�����
  * @param  g:   ���ض���
            in:  ��������
			cmd: STEPING:    ��������
			     CLEARSTEP:  ����������ʷ��Ϣ
  * @retval out: ��Ӧ��������ķ������
  */
float action_PID::getfeedback(control_model &g, float in, uint8_t cmd)
{
	static float out = 0;
	float pid_out;
	if (cmd == STEPING)
	{
		pid_out = this->out(in - out);
		out = g.step(pid_out, STEPING);
	}
	else if (cmd == CLEARSTEP)
	{
		out = 0;
		err_sum = 0;
		last_err = 0;
		g.step(0, CLEARSTEP);
	}
	return out;
}
void action_PID::OptTuning(control_model &g, float wc, float T, float test_in)
{
	float result[1000];
	for (uint16_t i = 0; i < 1000; i++)
	{
		this->autoTuning(g, wc, 10 + 170.0f / 1000.0f*i, T);
		result[i] = 0;
		for (uint32_t j = 0; j < 1000; j++)
		{
			result[i] += (float)fabs(test_in - this->getfeedback(g, test_in, STEPING));

			if (this->Kp < 0 || this->Ki < 0)
			{
				result[i] = -1;
				break;
			}
		}
		this->getfeedback(g, test_in, CLEARSTEP);
	}

	for (uint16_t i = 0; i < 1000; i++)
	{
		uint16_t count = 0;
		for (uint16_t j = 0; j < 1000; j++)
		{
			if (fabs(result[i] + 1) < 0.01)
			{
				break;
			}
			if (result[i] <= result[j]|| fabs(result[j] + 1) < 0.01)
			{
				count++;
			}
		}
		if (count >= 1000)
		{
			this->autoTuning(g, wc, 10 + 170.0f / 1000.0f*i, T);
			break;
		}
			
	}
}
void action_PID::OptTuning(control_model &g, float T, float test_in)
{
	float result[1000];
	for (uint16_t i = 0; i < 1000; i++)
	{
		this->OptTuning(g, 0.5f+i*0.5f, T, test_in);
		result[i] = 0;
		for (uint32_t j = 0; j < 1000; j++)
		{
			result[i] += (float)fabs(test_in - this->getfeedback(g, test_in, STEPING));
		}
		this->getfeedback(g, test_in, CLEARSTEP);
	}
	for (uint16_t i = 0; i < 1000; i++)
	{
		uint16_t count = 0;
		for (uint16_t j = 0; j < 1000; j++)
		{
			if (result[i] <= result[j])
			{
				count++;
			}
		}
		if (count >= 1000)
		{
			this->OptTuning(g, 0.5f + i*0.5f, T, test_in);
			break;
		}

	}
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
