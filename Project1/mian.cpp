#include "action_matrix.h"
#include <iostream>

using namespace std;


int main(void) {
	action_matrix x(3, 3);
	x[0][0] = 1; x[0][1] = 2; x[0][2] = 3;
	x[1][0] = 3; x[1][1] = 2; x[1][2] = 1;
	x[2][0] = 2; x[2][1] = 8; x[2][2] = 5;

	cout << ~x << endl;
	cout << x*(~x) << endl;

	getchar();
	return 0;
}