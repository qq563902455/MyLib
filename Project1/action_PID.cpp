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
* @brief  得到控制器输出
* @param  err:  误差，期望值与实际值的差
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
* @brief  PI控制器自整定
* @param  g:   被控对象
          wc:  穿越截止频率
		  rwc: 相位裕度
		  T:   控制周期，单位s
* @retval none
*/
void action_PID::autoTuning(control_model &g, float wc, float rwc,float T)
{
	double wwc;                   //对应w变换后的穿越截止频率
	double z_r, z_v, A_z, A_ph;   //算子Z在w域里的实部，虚部，模长，相位角

	wwc = 2 / T*tan(wc*T / 2);   //计算穿越截止频率

	z_r = (1 - T*T / 4 * wwc*wwc) / (1 + T*T / 4 * wwc*wwc);  //计算算子Z的实部
	z_v = T*wwc / (1 + T*T / 4 * wwc*wwc);                    //计算算子Z的虚部
	A_z = sqrt(z_r*z_r + z_v*z_v);                            //计算算子Z的模长
	A_ph = atan2(z_v, z_r);                                   //计算算子Z的相位角

	double R_t = 0, V_t = 0, R_b = 0, V_b = 0;  //被控对象分子的实部，虚部，被控对象分母的实部，虚部
	double temp_A,temp_ph;
	double A_g,ph_g;

	/*  分子实部虚部计算  */
	for (uint16_t i = 1; i <= g.getLenNum(); i++)
	{
		temp_A = pow(A_z, g.getLenNum() - i);
		temp_ph = A_ph*(g.getLenNum() - i);
		R_t += g.getNumP()[i - 1] * temp_A*cos(temp_ph);
		V_t += g.getNumP()[i - 1] * temp_A*sin(temp_ph);
	}
	/*  分母实部虚部计算  */
	for (uint16_t i = 1; i <= g.getLenDen(); i++)
	{
		temp_A = pow(A_z, g.getLenDen() - i);
		temp_ph = A_ph*(g.getLenDen() - i);
		R_b += g.getDenP()[i - 1] * temp_A*cos(temp_ph);
		V_b += g.getDenP()[i - 1] * temp_A*sin(temp_ph);
	}
	ph_g = atan2(V_t, R_t) - atan2(V_b, R_b);                   //计算被控对象的相位角
	A_g = sqrt(V_t*V_t + R_t *R_t) / sqrt(V_b*V_b + R_b*R_b);   //计算被控对象的模长

	/* PI的控制器模长，相位角，以及PI比例，PI具体的值的计算 */
	double ph_pi,Rate_pi;

	temp_A = sqrt(pow((T*wwc / (1 + T*T / 4 * wwc*wwc)), 2) + pow((T*T / 2 * wwc*wwc / (1 + T*T / 4 * wwc*wwc)), 2));
	temp_ph = atan2((T*wwc / (1 + T*T / 4 * wwc*wwc)), -(T*T / 2 * wwc*wwc / (1 + T*T / 4 * wwc*wwc)));

	temp_A = T / temp_A;
	temp_ph = -temp_ph;

	temp_A = temp_A*A_z;
	temp_ph += A_ph;


	rwc = rwc / 180.0f*3.1415926f;                                                  //将给定相位裕度从角度变成弧度
	ph_pi = rwc - 3.141592 - ph_g;                                                  //计算PI控制器需要的相位角
	Rate_pi = tan(ph_pi) / (temp_A*sin(temp_ph) - temp_A*tan(ph_pi)*cos(temp_ph));  //计算P与I的比例

	Ki = (float)(sin(ph_pi) / A_g / temp_A / sin(temp_ph));  //得到最后的Ki
	Kp = (float)(Ki / Rate_pi);                              //得到最后的Kp
	Kd = 0.0f;

	Ki *= T;
}
/**
  * @brief  获得控制器的仿真结果
  * @param  g:   被控对象
            in:  仿真输入
			cmd: STEPING:    正常仿真
			     CLEARSTEP:  清楚仿真的历史信息
  * @retval out: 对应仿真输入的仿真输出
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

	/* 调整被控对象模型 */
	model.study(data_st, &act_val);

	/* 获得仿真输出，为求入输出之间的导数关系做准备 */
	model.out(data_st, &temp_out_old);

	/* 更新仿真输入，为求入输出之间的导数关系做准备 */
	if (fabs(controller_out[nb + nd - 1]) > 0.01f)
	{
		controller_out[nb + nd - 1] *= 1.01f;
		/* 获得仿真输出，为求入输出之间的导数关系做准备 */
		model.out(data_st, &temp_out_new);
		/* 计算仿真的输入输出之间的导数关系 */
		diff = (temp_out_new - temp_out_old) / (controller_out[nb + nd - 1] / 101);
	}
	else
	{
		controller_out[nb + nd - 1] += 0.001f;
		/* 获得仿真输出，为求入输出之间的导数关系做准备 */
		model.out(data_st, &temp_out_new);
		/* 计算仿真的输入输出之间的导数关系 */
		diff = (temp_out_new - temp_out_old) / 0.001f;
	}
		
	diff_in[0] = diff*last_err;
	diff_in[1] = diff*err_sum;
	diff_in[2] = diff*err_v;

	pid_in[0] = last_err;
	pid_in[1] = err_sum;
	pid_in[2] = err_v;

	/* 误差计算 */
	err = set_val - act_val;
    /* 训练用于调整PID的神经网络 */
	control.study(pid_in, err, diff_in);
	/* 更新BP神经网络的输入，该神经网络的输入是误差的相关信息，输出为PID的值 */
	pid_in[0] = err;
	pid_in[1] = err + err_sum;
	pid_in[2] = err - last_err;

	/* 根据误差情况更新PID的值 */
	control.out(pid_in, pid_out);
	Kp = pid_out[0];
	Ki = pid_out[1];
	Kd = pid_out[2];

	/* 更新输入输出数据 */
	memcpy(controller_out, controller_out + 1, 4 * (nb + nd - 1));
	memcpy(object_out, object_out + 1, 4 * (na - 1));
	controller_out[nb + nd - 1] = this->out(err);
	object_out[na - 1] = act_val;

	return controller_out[nb + nd - 1];
}


/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
