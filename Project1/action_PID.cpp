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
	float out;

	err_sum += err;
	err_v = err - last_err;
	last_err = err;

	if (err_sum*Ki >= err_sum_max)
	{
		err_sum = err_sum_max / Ki;
	}
	if (err_sum*Ki <= -err_sum_max)
	{
		err_sum = -err_sum_max / Ki;
	}
	out = Kp*err + Ki*err_sum + Kd*err_v;

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
BP_PID::BP_PID(uint8_t nA, uint8_t nB, uint8_t nD)
{
	na = nA;
	nb = nB;
	nd = nD;


	data_st= new float[nA + nB + nD];


	for (uint16_t i = 0; i < nA + nB + nD; i++)
	{
		data_st[i] = 0;
	}

	controller_out = data_st;
	object_out = data_st + nB + nD;

}
BP_PID::~BP_PID()
{
	delete[] data_st;
}
float BP_PID::BP_PID_out(BP_ANN &control, BP_ANN &model, float set_val, float act_val)
{
	float err, temp_out_old, temp_out_new;
	float pid_in[3];
	float diff;
	float diff_in[3];
	float pid_out[3];

	/* �������ض���ģ�� */
	model.study(data_st, &act_val);

	/* ��÷��������Ϊ�������֮��ĵ�����ϵ��׼�� */
	model.out(data_st, &temp_out_old);

	/* ���·������룬Ϊ�������֮��ĵ�����ϵ��׼�� */
	if (fabs(controller_out[nb + nd - 1]) > 0.01f)
	{
		controller_out[nb + nd - 1] *= 1.01f;
		/* ��÷��������Ϊ�������֮��ĵ�����ϵ��׼�� */
		model.out(data_st, &temp_out_new);
		/* ���������������֮��ĵ�����ϵ */
		diff = (temp_out_new - temp_out_old) / (controller_out[nb + nd - 1] / 101);
	}
	else
	{
		controller_out[nb + nd - 1] += 0.001f;
		/* ��÷��������Ϊ�������֮��ĵ�����ϵ��׼�� */
		model.out(data_st, &temp_out_new);
		/* ���������������֮��ĵ�����ϵ */
		diff = (temp_out_new - temp_out_old) / 0.001f;
	}
		
	diff_in[0] = diff*last_err;
	diff_in[1] = diff*err_sum;
	diff_in[2] = diff*err_v;

	pid_in[0] = last_err;
	pid_in[1] = err_sum;
	pid_in[2] = err_v;

	/* ������ */
	err = set_val - act_val;
    /* ѵ�����ڵ���PID�������� */
	control.study(pid_in, err, diff_in);
	/* ����BP����������룬������������������������Ϣ�����ΪPID��ֵ */
	pid_in[0] = err;
	pid_in[1] = err + err_sum;
	pid_in[2] = err - last_err;

	/* ��������������PID��ֵ */
	control.out(pid_in, pid_out);
	Kp = pid_out[0];
	Ki = pid_out[1];
	Kd = pid_out[2];

	/* ��������������� */
	memcpy(controller_out, controller_out + 1, 4 * (nb + nd - 1));
	memcpy(object_out, object_out + 1, 4 * (na - 1));
	controller_out[nb + nd - 1] = this->out(err);
	object_out[na - 1] = act_val;

	return controller_out[nb + nd - 1];
}


/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
