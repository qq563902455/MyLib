#include "ArtificialNeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <sstream>  
using namespace std;



int main(void) {
	BP_ANN test(8, 7, 5, FUN_LOGSIG, FUN_LOGSIG);
	test.printf();
	cout << endl;
	test.modelReadfile("d:/abc.txt");
	test.printf();
	//test.modelTofile("d:/abc.txt");
	//test.printf();

	//ifstream inf("d:/abc.txt");
	//
	//char test[500];
	//stringstream is(test);
	//while (inf.getline(test,500))
	//{
	//	cout << test << endl;
	//	is.clear();
	//	is.str("");
	//	is << test;
	//	
	//	float s;
	//	while (is >> s) {
	//		cout<<s;
	//	}
	//	cout << endl;
	//}

	getchar();
	return 0;
}