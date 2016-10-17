#include <fstream>  
#include <string>  
#include <iostream>  
#include "ArtificialNeuralNetwork.h"
#include "action_control.h"
#include "action_PID.h"

#include <time.h>

using namespace std;


int main(void)
{
	BP_ANN gb_study(2, 10, 1, FUN_TANSIG, FUN_TANSIG);
	static float out[1] = { 0 };
	static float in[2] = { 0,0 };

	srand((int)time(0));

	gb_study.a_study = 0.01;
	gb_study.a_keep = 0.05;

	while (1)
	{
		float err = 0;
		for (uint32_t i = 0; i < 100000; i++)
		{
			in[0] = rand() % 200 / 100.0f - 1;
			in[1] = rand() % 200 / 100.0f - 1;
			out[0] = (in[0] * in[0] + in[1] * in[1]) / 2.0f;
			gb_study.study(in, out);
		}
		

		
		for (uint32_t i = 0; i < 1000; i++)
		{
			in[0] = rand() % 200 / 100.0f - 1;
			in[1] = rand() % 200 / 100.0f - 1;
			out[0] = 0.0f;
			gb_study.out(in, out);

			if ((in[0]*in[0]+ in[1]*in[1]) <= 0.01)
				continue;

			err += fabs((in[0] * in[0] + in[1] * in[1]) / 2.0 - out[0])/ (in[0] * in[0] + in[1] * in[1]);
		}
		cout << "err: " <<err << endl;
		if (err < 10)
			break;

	}
	gb_study.printf();
	for (;;)
	{
		//in[0] = rand() % 200 / 100.0f - 1;
		//in[1] = rand() % 200 / 100.0f - 1;

		cin >> in[0];
		cin >> in[1];

		out[0] = 0.0f;
		gb_study.out(in, out);

		cout << "in0: " << in[0] << endl;
		cout << "in1: " << in[1] << endl;
		cout << "out: " << out[0] << endl;
		cout << "actval: " << (in[0] * in[0] + in[1] * in[1]) / 2.0f << endl;
		cout << endl;
	}
	getchar();
	return 0;
}