#include <fstream>  
#include <string>  
#include <iostream>  
#include "ArtificialNeuralNetwork.h"
#include "action_control.h"
#include "action_PID.h"

#include <time.h>

using namespace std;

float num[] = { 0.8f ,0 };
float den[] = { 0.7f ,-1.0f };
float act_val=0;
float ctr = 0;

int main(void)
{
	control_model gb(DiscreteSys, num, den, 2, 2);
	BP_ANN control(3, 5, 3, FUN_TANSIG, FUN_LOGSIG);
	BP_ANN model(2, 3, 1, FUN_TANSIG, FUN_TANSIG);
	BP_PID test(1, 1, 0);


	cout << "Kp:  " << test.Kp << endl;
	cout << "Ki:  " << test.Ki << endl;
	cout << "Kd:  " << test.Kd << endl;

	for (uint16_t j = 0; j < 1000; j++)
	{
		for (uint32_t i = 0; i < 100; i++)
		{
			if (j % 2 == 0)
			{
				ctr = test.BP_PID_out(control, model, 0.5, act_val);
			}
			else
				ctr = test.BP_PID_out(control, model, 1, act_val);
			act_val = gb.step(ctr, STEPING);
		}
	}
	

	for (uint32_t i = 0; i < 100; i++)
	{
		ctr = test.BP_PID_out(control, model, 0.5, act_val);

		act_val = gb.step(ctr, STEPING);

		cout << endl;
		cout << "U:  " << ctr << endl;
		cout << "Y:  " << act_val << endl;
		cout << "Kp:  " << test.Kp << endl;
		cout << "Ki:  " << test.Ki << endl;
		cout << "Kd:  " << test.Kd << endl;
	}


	getchar();
	return 0;
}