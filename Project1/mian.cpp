#include <fstream>  
#include <string>  
#include <iostream>  
#include "ArtificialNeuralNetwork.h"
#include "action_control.h"
#include "action_PID.h"


using namespace std;

int main(void)
{
	//ifstream file("D:\\常用未整理\\代码类\\自适应控制\\test.csv");
	//float test;
	//
	//file >> test;
	//file >> test;
	//cout << test << endl;
	//file.close();

	//BP_ANN test(1, 5, 1);
	//float in;
	//float out;
	//for (uint32_t i = 0; i < 100; i++)
	//{
	//	in = 49 / 100.0f;
	//	test.out(&in, &out);
	//	cout << out << endl;
	//	for (uint32_t j = 0; j < 50; j++)
	//	{
	//		out = 0;
	//		in = j / 100.0f;
	//		test.study(&in, &out);
	//	}
	//	for (uint32_t j = 50; j < 100; j++)
	//	{
	//		out = 1;
	//		in = j / 100.0f;
	//		test.study(&in, &out);
	//	}
	//	
	//}
	//in = 49/100.0f;
	//test.out(&in, &out);
	//cout << out << endl;

	float num[3] = { 0.01,0.0083887,0.007234 };
	float den[3] = { 1,-0.7714,-0.2291};
	float out=0;

	control_model test(0, num, den, 3, 3);
	action_PID ctr;

	ctr.OptTuning(test, 100, 0.01, 1);

	cout << ctr.Kp << endl;
	cout << ctr.Ki << endl;



	getchar();

	return 0;
}