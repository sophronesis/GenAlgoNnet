#include "scene.h"
#include <algorithm>
#ifndef METASCENE_HH
#define METASCENE_HH
struct score_t
{
	int index;
	int fitness;
};
struct metascene
{
	//number of agents in population
	int num_agents;

	//scene
	scene myscene;

	//random indexes array
	int* true_index;

	//scores of all agents
	//first - number of agent
	//second - amount of score
	score_t *score;

	//genes array from population
	population* in_population;

	//loging file
	std::ofstream log_file;
	
	//mutation rate
	double mut_rate;

	metascene();
	~metascene();

	//initialize metascene
	void init(population&);

	//initialize log file
	void init_log(int);

	//add comma to log file
	void add_comma();

	//finalize log
	void finalize_log();

	//next step of metascene
	bool nextstep();

	//finalize metascene
	void finalize();

	//triggers, counters and index arrays
	bool scene_ended;
	bool metascene_ended;
	int current_scene;
	int testing_num;
	int* testing_index;

};
#endif
