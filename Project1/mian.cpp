#include "action_matrix.h"
#include "flpFilter.h"
#include <iostream>

using namespace std;

float inData[10] = { 0.1f,1.1f,2.1f,3.1f,4.1f,
					5.1f,6.1f,7.1f,8.1f,9.1f };

int main(void) {
	flpFilter test(3,1,0.1);
	
	for (uint16_t i = 0; i < 500; i++)
	{
		float re;
		if (i < 10) {
			 re = test.out(inData[i]);
		}
		else
		{
			re = test.out(10);
		}
		cout << "filter out: "<< re << endl;
	}


	getchar();
	return 0;
}