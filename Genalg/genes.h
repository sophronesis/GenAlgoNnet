#include "matrix.h"
#ifndef GENES_HH
#define GENES_HH
struct gene//genes list
{
	~gene();
	int size = -1;
	double* data = 0;
};

//merge genes(done)
//uses extra memory
gene gene_merge(gene first_gene, gene second_gene);

//seperate genes(done)
void gene_seperate(gene& input, int num, gene& first_part, gene& second_part);

//randomize genes(done)
//uses extra memory
gene gene_randomize(int num, double min = -1, double max = 1);

//crossover of genes(done)
//uses extra memory
gene* gene_crossover(gene first_gene, gene second_gene, int mode = 1);

//mutation of gene(done)
void gene_mutation(gene& input, double mutate_probability, double min = -1, double max = 1, bool mode = 0);


#endif