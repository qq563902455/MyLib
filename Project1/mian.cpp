#include <iostream>
#include <cstdint>
#include "action_matrix.h"
#include "action_control.h"
#include "action_PID.h"
using namespace std;

control_model g(0,1,1,1);
action_PID pi;
int main(void)
{
	for (int i = 0; i < 10000; i++)
	{
		g.model_iddata(1, i);
	}
	g.model_ident();

	pi.autoTuning(g, 1, 60, 0.005f);

	cout << pi.Kp << endl;
	cout << pi.Ki << endl;

	while (1);
}