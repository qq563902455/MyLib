#include <iostream>
#include <cstdint>
#include "action_matrix.h"
#include "action_control.h"
#include "action_PID.h"
using namespace std;

float num[] = { 0.00625725f,0.0166207f};
float den[] = { 1.0f,-0.792979f,-0.207224f };

control_model g(0, num, den, 2, 3);
int main(void)
{
	action_PID pi;
	pi.out_max = 500;

	/*  这个函数你就直接按下面给就OK了 */
	pi.OptTuning(g,0.001f, 100);

	cout << "Kp " << pi.Kp << endl;
	cout << "Ki " << pi.Ki << endl;

	while (1);
}