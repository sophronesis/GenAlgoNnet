#define _CRT_SECURE_NO_WARNINGS
#include "population.h"
#include <iostream>
#include <fstream>
population::population()
{
	//genes.size = 0;
}
population::~population()
{
	for (int i = 0; i < population_size; ++i)//for all agents
		delete[] genes[i].data;
	delete[] genes;
}
void population::init(int** intopology, int* inoptions, double inmut_rate)
{
	//calculating num of genes in agents
	for (int i = 0; i < nnetnum; ++i)//add neural networks
		gene_size += weightsnum(intopology[i]);
	//adding other data
	gene_size += inoptions[12];

	options = inoptions;
	population_size = inoptions[5];
	topology = intopology;
	mut_rate = inmut_rate;

	//add random genes to all agents
	genes = new gene[population_size];
	for (int i = 0; i < population_size; ++i)//for all agents
	{
		gene agent;
		agent = gene_randomize(gene_size);
		genes[i] = agent;
	}
}
void population::readfromfile(bool mode, char * name)
{
	/*
	<population_size> <gene size> <nnet_num> <other_data>

	[i=1..<nnet_num>]
	<nnet_i_topology>

	[i=1..<nnet_num>]
	[j=1..<population_size>]
	<nnet_i_weights>
	<other_data_genome>
	*/
	std::string path;
	std::ifstream finp;
	char file[20];
	path += "data\\";

	if (mode)
		strcpy(file, name);
	else
		std::cin >> file;
	path = (path + file);
	finp.open(path);
	if (finp)//reading
	{
		int temp;

		//first line
		finp >> population_size >> gene_size >> nnetnum >> gene_other;
		topology = new int*[nnetnum];
		
		//nnet topologies
		for (int i = 0; i < nnetnum; ++i)
		{
			//reading topology
			finp >> temp;//topology size
			topology[i] = new int[temp + 1];
			topology[i][0] = temp;
			for (int j = 1; j < topology[i][0] + 1; ++j)
				finp >> topology[i][j];
		}

		//reading genes
		genes = new gene[population_size];
		for (int i = 0; i < population_size; ++i)
		{
			genes[i].size = gene_size;
			genes[i].data = new double[gene_size];
			for (int j = 0; j < gene_size; ++j)
				finp >> genes[i].data[j];
		}
		finp.close();
	}
	else
	{
		std::cout << std::endl << "File not found" << std::endl;
		exit(1);
	}
		
}
void  population::writetofile(bool mode, char * name)
{
	/*
	<population_size> <gene size> <nnet_num> <other_data>

	[i=1..<nnet_num>]
	<nnet_i_topology>

	[i=1..<nnet_num>]
	[j=1..<population_size>]
	<nnet_i_weights>
	<other_data_genome>
	*/
	std::string path;
	std::ofstream fout;
	char file[20];
	path += "data\\";

	if (mode)
		strcpy(file, name);
	else
		std::cin >> file;
	path = (path + file) ;
	fout.open(path);

	if (fout)//reading
	{
		//first line
		fout << population_size << " " << gene_size << " " << nnetnum << " " << gene_other << std::endl;

		//nnet topologies
		for (int i = 0; i < nnetnum; ++i)
		{
			for (int j = 0; j < topology[i][0] + 1; ++j)
				fout << topology[i][j] << " ";
			fout << std::endl;
		}
		fout << std::endl;

		//genes
		for (int i = 0; i < population_size; ++i)
		{
			for (int j = 0; j < gene_size; ++j)
				fout << genes[i].data[j] << std::endl;
			fout << std::endl;
		}
		fout.close();
	}
	else
		std::cout << std::endl << "Can't write population data to file" << std::endl;
}
