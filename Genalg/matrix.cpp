#include "matrix.h"
matrix::matrix()
{

}
matrix::matrix(int x, int y)
{
	val = new double*[x];
	for (int i = 0; i < x; ++i)
		val[i] = new double[y];
	xsize = x;
	ysize = y;
	for (int i = 0; i < x; ++i)
		for (int j = 0; j < y; ++j)
			val[i][j] = 0;
}
//todo -> bugs
matrix::~matrix()
{
	//if ((xsize != -1) && (ysize != -1))
		for (int i = 0; i < xsize; ++i)
			delete[] val[i];
	delete[] val;
}
void matrix::resize(int x, int y)
{
	if ((xsize != -1) && (ysize != -1))
		this->~matrix();

	val = new double*[x];
	for (int i = 0; i < x; ++i)
		val[i] = new double[y];
	xsize = x;
	ysize = y;
	for (int i = 0; i < x; ++i)
		for (int j = 0; j < y; ++j)
			val[i][j] = 0;
}
void matrix::randomize(double min, double max)
{
	for (int i = 0; i < xsize; ++i)
		for (int j = 0; j < ysize; ++j)
			val[i][j] = min + (max - min)*((float)rand() / RAND_MAX);
}