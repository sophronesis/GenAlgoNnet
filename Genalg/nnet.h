#include "genes.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifndef NNET_HH
#define NNET_HH
struct nnet
{
	//inputdim = topology[1];
	//ouputdim = topology[topology[0]];
	//weights num = topology[0]-1;

	//nnet layers data
	int* topology=0;

	//weights matrixes
	matrix* weights=0;

	//init nnet (inputing topology)
	nnet();
	nnet(int* data);
	void init(int* data);

	//destroy nnet
	~nnet();
	
	//randomize weights
	void randomize();

	//feed forward data vector
	//uses extra memory
	double* fforward(double* input);

	//export weights' data
	//uses extra memory
	gene exportweights();

	//import weights' data
	void importweights(gene input);
};

//return weights number (from topology array)
int weightsnum(int* data);

#endif