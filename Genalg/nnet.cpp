#include "nnet.h"
nnet::nnet()
{

}
nnet::nnet(int* data)
{
	init(data);
}
void nnet::init(int* data)
{
	//copy topology
	topology = data;

	//adjust weights
	weights = new matrix[topology[0] - 1];

	for (int i = 0; i < topology[0] - 1; ++i)
		weights[i].resize(topology[i + 2], topology[i + 1] + 1);//+1 for bias neuron
}
void nnet::randomize()
{
	for (int i = 0; i < topology[0] - 1; ++i)
		weights[i].randomize();
}
nnet::~nnet()
{
	//delete[] weights;
}
inline double sigmoide(double input)
{
	return (1 / (1 + pow(M_E, -input)));
}
void sigmoide(double* input, int size)
{
	for (int i = 0; i < size; ++i)
		input[i] = sigmoide(input[i]);
}
double* addbias(double* vector, int num)
{
	double* result = new double[num + 1];
	for (int i = 0; i < num; ++i)
		result[i] = vector[i];
	result[num] = 1;
	delete[] vector;
	return result;
}
inline double* fforwardone(double* vector, matrix* weights)//feed forward's one step
{
	//init result vector
	double* result = new double[weights->xsize];

	//main cycle
	for (int i = 0; i < weights->xsize; ++i)
	{
		result[i] = 0;
		for (int j = 0; j < weights->ysize; ++j)
			result[i] += vector[j] * weights->val[i][j];//sum up all multiplications 
	}

	delete[] vector;

	return result;
}
double* nnet::fforward(double* input)
{
	//init vector
	double* result = new double[topology[1]+1];

	//copy data
	for (int i = 0; i < topology[1]; ++i)
		result[i] = input[i];
	//add bias neuron
	result[topology[1]] = 1;

	//main cycle
	for (int i = 0; i < topology[0] - 1; ++i)
	{
		//sum up multiplications of vector by matrix elements
		result = fforwardone(result, &weights[i]);

		//sigmoide over current vector
		sigmoide(result, topology[i + 2]);

		//add bias neuron
		if (i < topology[0] - 2) result = addbias(result, topology[i + 2]);
	}

	return result;
}
gene nnet::exportweights()
{
	//init array with size for all weights
	gene result;
	result.size = weightsnum(topology);
	result.data = new double[weightsnum(topology)];
	int current = 0;
	for (int k = 0; k < topology[0] - 1; ++k)//for all weights matrixs
		for (int j = 0; j < weights[k].ysize; ++j)//for all ysize
			for (int i = 0; i < weights[k].xsize; ++i, ++current)//for all xsize
				result.data[current] = weights[k].val[i][j];
	return result;
}
int weightsnum(int* data)
{
	int result = 0;
	for (int i = 0; i < data[0] - 1; ++i)
		result += (data[i + 1] + 1) * (data[i + 2]);
	return result;
}
void nnet::importweights(gene input)
{
	if (weightsnum(topology) == input.size)
	{
		//importing
		int current = 0;
		for (int k = 0; k < topology[0] - 1; ++k)//for all weights matrixs
			for (int j = 0; j < weights[k].ysize; ++j)//for all ysize
				for (int i = 0; i < weights[k].xsize; ++i, ++current)//for all xsize
					weights[k].val[i][j] = input.data[current];
	}
	else
		std::cout << "Error in importweights method of nnet class!" << std::endl;
}
