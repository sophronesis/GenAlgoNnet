#include "metascene.h"
metascene::metascene()
{

}
metascene::~metascene()
{
}
int randnum(int min, int max)
{
	int num = min + (max + 1 - min)*rand() / RAND_MAX;
	return (num == max + 1) ? (max) : (num);
}
int* rand_arr(int num_agents)
{
	int* true_num = new int[num_agents];
	std::list<int> temp_list;
	std::list<int>::iterator it;

	for (int i = 0; i < num_agents; ++i)
		temp_list.push_back(i);

	int pos = 0;
	while (!temp_list.empty())
	{
		int rand_num = randnum(0, temp_list.size() - 1);
		it = temp_list.begin();
		while (rand_num > 0)
		{
			++it;
			rand_num--;
		}
		true_num[pos] = *it;
		temp_list.erase(it);
		++pos;
	}
	return true_num;
}
int binary_search_range(score_t* A, int key, int size)
{
	int left = 0, right = size - 1, mid;
	while (left <= right)
	{
		mid = left + (right - left) / 2;
		if (key < A[mid].fitness) right = mid - 1;
		else if (key > A[mid].fitness) left = mid + 1;
		else return mid;
	}
	return mid;
}
int fitness_comp(const void * a, const void * b)
{
	if (((score_t*)a)->fitness <  ((score_t*)b)->fitness) return -1;
	if (((score_t*)a)->fitness >((score_t*)b)->fitness) return 1;
	if (((score_t*)a)->fitness == ((score_t*)b)->fitness) return 0;
}
void metascene::init_log(int pop_size)
{
	log_file.open("data//log.txt");
	log_file << "'{\"N\":" << pop_size << ",";
	log_file << "\"I\":[";
}
void metascene::add_comma()
{
	log_file << ",'," << std::endl << "'";
}
void metascene::finalize_log()
{
	log_file << "}'";
	log_file.close();
}
void metascene::init(population& input)
{
	//init vars
	mut_rate = input.mut_rate;
	num_agents = input.population_size;
	in_population = &input;
	score = new score_t[num_agents];
	testing_num = 3;
	testing_index = new int[testing_num];
	scene_ended = 1;
	metascene_ended = 0;
	current_scene = -1;
	true_index = rand_arr(num_agents);
	for (int i = 0; i < num_agents; ++i)
	{
		score[i].index = i;
		score[i].fitness = 0;
	}
	//create random array of indexes
}
bool metascene::nextstep()
{
	if (metascene_ended) return 0;
	else 
		if (scene_ended)
		{
			//clear data from previous scene and save scores
			if (current_scene != -1)
			{
				std::cout << "[" << std::setw(2) << current_scene << "]|";
				int* testing_results = myscene.finalize();
				for (int i = 0; i < testing_num; ++i)
				{
					std::cout << " <" << std::setw(2) << testing_index[i] << "> <- " << std::setw(4) << testing_results[i] << "|";
					score[testing_index[i]].fitness += testing_results[i];
				}
				std::cout << std::endl;
				delete[] testing_results;
			}
			current_scene++;
			if (current_scene == num_agents) metascene_ended = 1;
			else
			{
				testing_index[0] = true_index[current_scene];
				testing_index[1] = true_index[(current_scene + 1) % num_agents];
				testing_index[2] = true_index[(current_scene + 3) % num_agents];
				myscene.maxX = in_population->options[0];
				myscene.maxY = in_population->options[1];
				myscene.init(*in_population, testing_index, testing_num);
				scene_ended = 0;
			}
		}
	else
		if (!myscene.nextstep())
			scene_ended = 1;
	return 1;
}
void metascene::finalize()
{
	//place for outputting results
	log_file << "[";
	for (int i = 0; i < num_agents;++i)
		if (i) log_file << "," << score[i].fitness; else log_file << score[i].fitness;

	//sorting by summary fitness
	qsort(score, num_agents, sizeof(score_t), fitness_comp);

	//median
	if (score[0].fitness != score[num_agents - 1].fitness) //if first and last have fitness
	{
		int mid_index = binary_search_range(score, (score[0].fitness + score[num_agents - 1].fitness) / 2, num_agents);
		if (num_agents - mid_index < 4) mid_index = num_agents - 4;
		else if (num_agents < 4) mid_index = 4;
		//int mid_index = num_agents / 2;

		for (int i = 0; i < num_agents; ++i)
		{
			if (i == mid_index) std::cout << "--- middle score range:" << (score[0].fitness + score[num_agents - 1].fitness) / 2 << std::endl;
			std::cout << std::setw(2) << score[i].index << " | " << std::setw(4) << score[i].fitness << std::endl;
		}

		gene* children = new gene[mid_index];
		int** parents = new int*[mid_index];
		for (int i = 0; i < mid_index; ++i)
			parents[i] = new int[2];

		//generate new species from fittest parents
		if (1)
			for (int i = 0; i < mid_index; ++i)
			{
				parents[i][0] = score[randnum(mid_index, num_agents - 1)].index;
				parents[i][1] = score[randnum(mid_index, num_agents - 1)].index;
				while (parents[i][0] == parents[i][1]) 
					parents[i][1] = score[randnum(mid_index, num_agents - 1)].index;
				children[i] = *gene_crossover(in_population->genes[parents[i][0]], in_population->genes[parents[i][1]]);
			}
		else
			for (int i = 0; i < mid_index; ++i)
			{
				int y = randnum(score[0].fitness - 2, score[num_agents - 1].fitness);
				int x = randnum(0, num_agents - 1);
				while (score[x].fitness < y)
				{
					y = randnum(score[0].fitness - 2, score[num_agents - 1].fitness);
					x = randnum(0, num_agents - 1);
				}
				parents[i][0] = x;
				while (parents[i][0] = x || score[x].fitness < y)
				{
					y = randnum(score[0].fitness - 2, score[num_agents - 1].fitness);
					x = randnum(0, num_agents - 1);
				}
				parents[i][1] = x;
				children[i] = *gene_crossover(in_population->genes[parents[i][0]], in_population->genes[parents[i][1]]);
			}

		//optimal parents
		score_t* optimal_placement = new score_t[mid_index];
		for (int i = 0; i < mid_index; ++i)
		{
			optimal_placement[i].index = i;
			optimal_placement[i].fitness = parents[i][0] + parents[i][1];
		}

		qsort(optimal_placement, mid_index, sizeof(score_t), fitness_comp);

		//replace with new species
		for (int i = 0; i < mid_index; ++i)
		{
			log_file << "," << score[i].index << "," << parents[i][0] << "," << parents[i][1];
			delete[] in_population->genes[score[i].index].data;
			in_population->genes[score[i].index].data = children[optimal_placement[i].index].data;
			gene_mutation(in_population->genes[score[i].index], mut_rate, -1, 1, 0);
		}
		for (int i = 0; i < mid_index; ++i)
			delete[] parents[i];
		delete[] parents;
		delete[] optimal_placement;
	}
	else
	{
		for (int i = 0; i < num_agents; ++i)
		{
			delete[] in_population->genes[score[i].index].data;
			in_population->genes[score[i].index] = gene_randomize(in_population->gene_size);
		}
	}


	log_file << "]";
	delete[] score;
	delete[] testing_index;
	delete[] true_index;
}



/*

//begin testing
for (int i = 0; i < num_agents; ++i)
{
//prepare agents
testing_index[0] = true_index[i];
testing_index[1] = true_index[(i + 1) % num_agents];
testing_index[2] = true_index[(i + 3) % num_agents];
myscene.init(input, testing_index, testing_num);
while (myscene.nextstep());
int* testing_results = myscene.finalize();
for (int i = 0; i < testing_num; ++i)
score[testing_index[i]].second += testing_results[i];
delete[] testing_index;
delete[] testing_results;
}

//sorting by summary fitness
qsort(score, num_agents, sizeof(std::pair<int, int>), pair_comp);

//median
int mid_fitness = (score[0].second + score[num_agents - 1].second) / 2;
int mid_index = binary_search_range(score, mid_fitness, num_agents);
if (num_agents - mid_index > 4) mid_index = num_agents - 4;
else if (num_agents < 4) mid_index = 4;

//generate new species from fittest parents
for (int i = 0; i < mid_index; ++i)
{
//init take random good parents
int parent_1 = randnum(mid_index, num_agents - 1);
int parent_2 = randnum(mid_index, num_agents - 1);
while (parent_1 == parent_2) parent_2 = randnum(mid_index, num_agents - 1);

//and mate them
gene* temp;
temp = gene_crossover(genes[parent_1], genes[parent_2]);
delete[] genes[score[i].first].data;
genes[score[i].first].data = temp->data;
gene_mutation(genes[score[i].first], mut_rate, -1, 1, 0);
}
*/

