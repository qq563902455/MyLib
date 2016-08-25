#include <iostream>
#include <cstdint>
#include "action_matrix.h"
#include "action_control.h"
#include "action_PID.h"
using namespace std;

control_model g(0,2,3,0,10000); //后面的10000代表要输入10000个数据
action_PID pi;
int main(void)
{
	for (int i = 1; i <= 10000; i++)
	{
		g.model_iddata(i+1/(float)i, i+i-1);
	}
	g.model_ident();


	for (uint8_t i = 0; i < 8; i++)
	{
		cout << g.getNumP()[i] << endl;
		cout << g.getDenP()[i] << endl;
		cout << endl;
	}

	pi.autoTuning(g, 1, 60, 0.0005f);

	cout << pi.Kp << endl;
	cout << pi.Ki << endl;

	while (1);
}