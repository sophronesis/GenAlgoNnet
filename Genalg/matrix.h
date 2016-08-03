#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#ifndef MATRIX_HH
#define MATRIX_HH
struct matrix
{
	//data
	int xsize = -1;
	int ysize = -1;
	double** val;

	//create matrix
	matrix();
	matrix(int, int);

	//destroy matrix
	~matrix();

	//resize matrix
	void resize(int xsize, int ysize);

	//randomize weights
	void randomize(double min = -1, double max = 1);
};
#endif