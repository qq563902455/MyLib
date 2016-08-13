#include "action_matrix.h"
#include "maa_config.h"
#include "action_AHRS.h"
#include <math.h>
#include <cstdint>

static action_matrix Q(4, 4, MATRIX_ZERO);
static action_matrix R_mag(3, 3, MATRIX_ZERO);
static action_matrix R_acc(3, 3, MATRIX_ZERO);
static action_matrix x(4, 1, MATRIX_ZERO);
static action_matrix P(4, 4, MATRIX_I);


void AHRS_Init()
{
	Q.set_data(0, 0, 0.01f);
	Q.set_data(1, 1, 0.01f);
	Q.set_data(2, 2, 0.01f);
	Q.set_data(3, 3, 0.01f);

	R_mag.set_data(0, 0, 1000.0f);
	R_mag.set_data(1, 1, 1000.0f);
	R_mag.set_data(2, 2, 10000.0f);

	R_acc.set_data(0, 0, 1.0f);
	R_acc.set_data(1, 1, 1.0f);
	R_acc.set_data(2, 2, 1.0f);

	P.set_data(0, 0, 1000000.0f);
	P.set_data(1, 0, 1000000.0f);
	P.set_data(2, 0, 1000000.0f);
	P.set_data(3, 0, 1000000.0f);

	x.set_data(0, 0, 1);
	x.set_data(1, 0, 0);
	x.set_data(2, 0, 0);
	x.set_data(3, 0, 0);
}
Quarternion AHRS_Update(three_axis gyr, three_axis accer, three_axis mag)
{
	static action_matrix x_pre(4, 1, MATRIX_ZERO);
	static action_matrix F(4, 4, MATRIX_ZERO);

	static action_matrix z(3, 1, MATRIX_ZERO);
	static action_matrix z_pre(3, 1, MATRIX_ZERO);
	static action_matrix H_mag(3, 4, MATRIX_ZERO);
	static action_matrix H_acc(3, 4, MATRIX_ZERO);

	static action_matrix K(4, 3, MATRIX_ZERO);
	static action_matrix P_pre(4, 4, MATRIX_ZERO);

	static action_matrix I(4, 4, MATRIX_I);

	float sum = 0;

	static action_matrix test(4, 1);

	Quarternion result;
	try
	{
		sum = mag.x*mag.x + mag.y*mag.y + mag.z*mag.z;
		sum = sqrt(sum);

		mag.x = mag.x / sum;
		mag.y = mag.y / sum;
		mag.z = mag.z / sum;

		sum = accer.x*accer.x + accer.y*accer.y + accer.z*accer.z;
		sum = sqrt(sum);

		accer.x = accer.x / sum;
		accer.y = accer.y / sum;
		accer.z = accer.z / sum;


		gyr.z = gyr.z*dT;
		gyr.x = gyr.x*dT;
		gyr.y = gyr.y*dT;

		/*  雅克比矩阵 */
		F.set_data(0, 0, 1);
		F.set_data(0, 1, -gyr.x);
		F.set_data(0, 2, -gyr.y);
		F.set_data(0, 3, -gyr.z);
		F.set_data(1, 0,  gyr.x);
		F.set_data(1, 1, 1);
		F.set_data(1, 2,  gyr.z);
		F.set_data(1, 3, -gyr.y);
		F.set_data(2, 0,  gyr.y);
		F.set_data(2, 1, -gyr.z);
		F.set_data(2, 2, 1);
		F.set_data(2, 3, gyr.x);
		F.set_data(3, 0, gyr.z);
		F.set_data(3, 1, gyr.y);
		F.set_data(3, 2, -gyr.x);
		F.set_data(3, 3, 1);

		/* 一阶龙格库塔法 */
		x_pre.set_data(0, 0, x.get_data(0, 0) - gyr.x*x.get_data(1, 0) - gyr.y*x.get_data(2, 0) - gyr.z*x.get_data(3, 0));
		x_pre.set_data(1, 0, x.get_data(1, 0) + gyr.x*x.get_data(0, 0) - gyr.y*x.get_data(3, 0) + gyr.z*x.get_data(2, 0));
		x_pre.set_data(2, 0, x.get_data(2, 0) + gyr.x*x.get_data(3, 0) + gyr.y*x.get_data(0, 0) - gyr.z*x.get_data(1, 0));
		x_pre.set_data(3, 0, x.get_data(3, 0) - gyr.x*x.get_data(2, 0) + gyr.y*x.get_data(1, 0) + gyr.z*x.get_data(0, 0));

		/* 预测误差方差阵 */
		P_pre = F*P*!F + Q;

#ifdef MAG_SENSOR_ENABLE	
		z.set_data(0, 0, mag.x);
		z.set_data(1, 0, mag.y);
		z.set_data(2, 0, mag.z);

		/* 四元数到方向余弦 */
		z_pre.set_data(0, 0, x.get_data(0, 0)*x.get_data(0, 0) + x.get_data(1, 0)*x.get_data(1, 0)
			- x.get_data(2, 0)*x.get_data(2, 0) - x.get_data(3, 0)*x.get_data(3, 0));
		z_pre.set_data(1, 0, 2 * x.get_data(1, 0)*x.get_data(2, 0) - 2 * x.get_data(3, 0)*x.get_data(0, 0));
		z_pre.set_data(2, 0, 2 * x.get_data(1, 0)*x.get_data(3, 0) + 2 * x.get_data(2, 0)*x.get_data(0, 0));

		/* 雅克比矩阵 */
		H_mag.set_data(0, 0, 2 * x.get_data(0, 0));
		H_mag.set_data(0, 1, 2 * x.get_data(1, 0));
		H_mag.set_data(0, 2, -2 * x.get_data(2, 0));
		H_mag.set_data(0, 3, -2 * x.get_data(3, 0));
		H_mag.set_data(1, 0, -2 * x.get_data(3, 0));
		H_mag.set_data(1, 1, 2 * x.get_data(2, 0));
		H_mag.set_data(1, 2, 2 * x.get_data(1, 0));
		H_mag.set_data(1, 3, -2 * x.get_data(0, 0));
		H_mag.set_data(2, 0, 2 * x.get_data(2, 0));
		H_mag.set_data(2, 1, 2 * x.get_data(3, 0));
		H_mag.set_data(2, 2, 2 * x.get_data(0, 0));
		H_mag.set_data(2, 3, 2 * x.get_data(1, 0));

		K = P_pre*(!H_mag)*(~(H_mag*P_pre*(!H_mag) + R_mag));

		x = x_pre + K*(z - z_pre);


		//cout << "mag" << endl;							
		//test= K*(z - z_pre);
		//test.PrintfItself();

		P = (I - K*H_mag)*P_pre*!(I - K*H_mag) + K*R_mag*!K;
		//P_mag.PrintfItself();

		sum = x.get_data(0, 0)*x.get_data(0, 0) +
			x.get_data(1, 0)*x.get_data(1, 0) +
			x.get_data(2, 0)*x.get_data(2, 0) +
			x.get_data(3, 0)*x.get_data(3, 0);

		sum = sqrt(sum);

		x.set_data(0, 0, x.get_data(0, 0) / sum);
		x.set_data(1, 0, x.get_data(1, 0) / sum);
		x.set_data(2, 0, x.get_data(2, 0) / sum);
		x.set_data(3, 0, x.get_data(3, 0) / sum);
#endif		
#ifdef ACC_SENSOR_ENABLE	
		x_pre = x;

		z.set_data(0, 0, accer.x);
		z.set_data(1, 0, accer.y);
		z.set_data(2, 0, accer.z);

		z_pre.set_data(0, 0, x.get_data(0, 0)*x.get_data(0, 0) + x.get_data(1, 0)*x.get_data(1, 0)
			- x.get_data(2, 0)*x.get_data(2, 0) - x.get_data(3, 0)*x.get_data(3, 0));
		z_pre.set_data(1, 0, 2 * x.get_data(1, 0)*x.get_data(2, 0) - 2 * x.get_data(3, 0)*x.get_data(0, 0));
		z_pre.set_data(2, 0, 2 * x.get_data(1, 0)*x.get_data(3, 0) + 2 * x.get_data(2, 0)*x.get_data(0, 0));


		/* 四元数到方向余弦 */
		z_pre.set_data(0, 0, 2 * x.get_data(0, 0)*x.get_data(2, 0) - 2 * x.get_data(1, 0)*x.get_data(3, 0));
		z_pre.set_data(1, 0, -2 * x.get_data(2, 0)*x.get_data(3, 0) - 2 * x.get_data(0, 0)*x.get_data(1, 0));
		z_pre.set_data(2, 0, x.get_data(1, 0)*x.get_data(1, 0) + x.get_data(2, 0)*x.get_data(2, 0)
			- x.get_data(0, 0)*x.get_data(0, 0) - x.get_data(3, 0)*x.get_data(3, 0));

		/* 雅克比矩阵 */
		H_acc.set_data(0, 0, 2 * x.get_data(2, 0));
		H_acc.set_data(0, 1, -2 * x.get_data(3, 0));
		H_acc.set_data(0, 2, 2 * x.get_data(0, 0));
		H_acc.set_data(0, 3, -2 * x.get_data(1, 0));
		H_acc.set_data(1, 0, -2 * x.get_data(1, 0));
		H_acc.set_data(1, 1, -2 * x.get_data(0, 0));
		H_acc.set_data(1, 2, -2 * x.get_data(3, 0));
		H_acc.set_data(1, 3, -2 * x.get_data(2, 0));
		H_acc.set_data(2, 0, -2 * x.get_data(0, 0));
		H_acc.set_data(2, 1, 2 * x.get_data(1, 0));
		H_acc.set_data(2, 2, 2 * x.get_data(2, 0));
		H_acc.set_data(2, 3, -2 * x.get_data(3, 0));

		K = P_pre*(!H_acc)*(~(H_acc*P_pre*(!H_acc) + R_acc));

		x = x_pre + K*(z - z_pre);

		//cout << "acc" << endl;
		//test = K*(z - z_pre);
		//test.PrintfItself();

		P = (I - K*H_acc)*P*!(I - K*H_acc) + K*R_acc*!K;

		sum = x.get_data(0, 0)*x.get_data(0, 0) +
			x.get_data(1, 0)*x.get_data(1, 0) +
			x.get_data(2, 0)*x.get_data(2, 0) +
			x.get_data(3, 0)*x.get_data(3, 0);

		sum = sqrt(sum);

		x.set_data(0, 0, x.get_data(0, 0) / sum);
		x.set_data(1, 0, x.get_data(1, 0) / sum);
		x.set_data(2, 0, x.get_data(2, 0) / sum);
		x.set_data(3, 0, x.get_data(3, 0) / sum);
#endif		
		result.q0 = x.get_data(0, 0);
		result.q1 = x.get_data(1, 0);
		result.q2 = x.get_data(2, 0);
		result.q3 = x.get_data(3, 0);
	}
	catch (int err)
	{
		switch (err)
		{
		case ERR_EQUAL:
			while (1);
		case ERR_MUL:
			while (1);
		case ERR_PLUS:
			while (1);
		case ERR_SUB:
			while (1);
		case ERR_INVERSE:
			while (1);
		}
	}
	return result;
}
