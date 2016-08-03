#include "genes.h"

gene::~gene()
{
}

//merge genes
gene gene_merge(gene first_gene, gene second_gene)
{
	gene result;
	result.size = first_gene.size + second_gene.size;
	result.data = new double[result.size];
	for (int i = 0; i < first_gene.size; ++i)
		result.data[i] = first_gene.data[i];
	for (int i = 0; i < second_gene.size; ++i)
		result.data[i + first_gene.size] = second_gene.data[i];
	return result;
}

//seperate genes
void gene_seperate(gene& input, int num, gene& first_part, gene& second_part)
{
	first_part.size = num;
	second_part.size = input.size - num;
	first_part.data = input.data;
	second_part.data = &input.data[first_part.size];
}

//randomize gene
gene gene_randomize(int num, double min, double max)
{
	gene result;
	result.size = num;
	result.data = new double[result.size];
	for (int i = 0; i < result.size; ++i)
		result.data[i] = min + (max - min)*((float)rand() / RAND_MAX);
	return result;
}

//crossover of genes
gene* gene_crossover(gene first_gene, gene second_gene, int mode)
{
	int num = first_gene.size;
	gene* result = new gene;
	result->size = num;
	result->data = new double[num];
	switch (mode)
	{
	case 0:
	{
		//init breakpoint (random [0 ... num - 1])
		int brkpoint = (int)(((float)rand() / RAND_MAX)*(num - 1));

		//selects offspring to select (first/second)
		bool offspring = (((float)rand() / RAND_MAX) > 0.5) ? 1 : 0;

		//copy genes to result vector
		for (int i = 0; i < num; ++i)
			if ((offspring) ^ (i > brkpoint))
				result->data[i] = first_gene.data[i];
			else
				result->data[i] = second_gene.data[i];
	}
	break;
	case 1:
	{
		//init breakpoint (random [0 ... num - 1])
		//point 1 < point 2
		int brkpoint1 = (int)(((float)rand() / RAND_MAX)*(num - 1));
		int brkpoint2 = (int)(((float)rand() / RAND_MAX)*(num - 1));
		if (brkpoint1 > brkpoint2)//swap
		{
			int temp;
			temp = brkpoint1;
			brkpoint1 = brkpoint2;
			brkpoint2 = temp;
		}

		//selects offspring to select (first/second)
		bool offspring = (((float)rand() / RAND_MAX) > 0.5) ? 1 : 0;

		//copy genes to result vector
		for (int i = 0; i < num; ++i)
			if ((offspring) ^ ((i >= brkpoint1) && (i <= brkpoint2)))
				result->data[i] = first_gene.data[i];
			else
				result->data[i] = second_gene.data[i];
	}
	break;
	case 2:
	{
		//copy genes to result vector with randomly selecting genes from both parents
		for (int i = 0; i < num; ++i)
			if ((((float)rand() / RAND_MAX) > 0.5) ? 1 : 0)
				result->data[i] = first_gene.data[i];
			else
				result->data[i] = second_gene.data[i];
	}
	break;
	}

	return result;
}

//mutation
void gene_mutation(gene& input, double mutate_probability, double min, double max, bool mode)
{ 
	if (!mode)
	{
		for (int i = 0; i < input.size; ++i)
			if (((float)rand() / RAND_MAX) < mutate_probability)
				input.data[i] = min + (max - min)*((float)rand() / RAND_MAX);
	}
	else
	{
		for (int i = 0; i < input.size; ++i)
			if (((float)rand() / RAND_MAX) < mutate_probability)
				input.data[i] += min + (max - min)*((float)rand() / RAND_MAX);
	}
}


