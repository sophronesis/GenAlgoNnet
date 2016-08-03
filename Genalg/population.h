#include <list>
#include <iterator>
#include "nnet.h"
#ifndef POPULATION_HH
#define POPULATION_HH
struct population
{
	//list of agents
	gene* genes;

	//agent's net topology
	int** topology;

	//options for scene
	int* options;

	//mutation rate
	double mut_rate;

	//gene of agents size
	int gene_size = 0;

	//number of nnets in agent
	int nnetnum = 2;

	//number of other stuff in nnets
	int gene_other = 4;

	//population size
	int population_size;

	//generate population(done)
	population();

	//delete population
	~population();

	//init population (add some number of agents with certain topology)(done)
	void init(int** topology, int* options, double mut_rate);

	//get population from file(done)
	//uses extra memory
	void readfromfile(bool mode = 0, char * name = "");

	//export popuation to file(done)
	void  writetofile(bool mode = 0, char * name = "");

};
#endif