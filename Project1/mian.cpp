#include <iostream>
#include <cstdint>
#include "action_matrix.h"
#include "action_control.h"
#include "action_PID.h"
using namespace std;

control_model g(0,3,1,0,10000);

int main(void)
{
	for (uint32_t i = 1; i < 10001; i++)
	{
		g.model_iddata(i, i+i-1+i/110.0f);
	}
	g.model_ident();

	for (uint8_t i = 0; i < 8; i++)
	{
		cout << g.getNumP()[i] << endl;
		cout << g.getDenP()[i] << endl;
	}

	while (1);
}