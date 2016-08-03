#include "population.h"
#include "fstream"
//#define SCREEN_X 640//1300
//#define SCREEN_Y 480//700
#ifndef SCENE_HH
#define SCENE_HH
struct object
{
	//show line of direction on circle
	bool isdirected=0;

	// position on map 
	double xpos = 0;
	double ypos = 0;

	//cicle radius
	double rad = 0;

	//angle 
	double angle = 0;

	//color
	double color[3];

	//constuctor
	object();

}; 
struct food : public object
{
	int feedpower;
	food();
};
struct big_food : public object
{
	int feedpower;
	big_food();
};
struct agent : public object
{
	gene* self_gene;
	double speed;
	int fitness = 0;
	int gene_pos = 0;
	agent();
}; 
struct scene
{
	//scene resolution
	int maxX;
	int maxY;

	//current screen resolution
	int screenX;
	int screenY;

	//scene initialized
	bool started;

	//timer
	int timer;

	//number of segments in circles
	int num_segments;

	//lists
	std::list<food> obj_food;
	std::list<big_food> obj_bfood;
	std::list<agent> obj_agent;
	std::list<food>::iterator obj_food_it;
	std::list<big_food>::iterator obj_bfood_it;
	std::list<agent>::iterator obj_agent_it;
	std::list<agent>::iterator obj_agent_it2;

	//agents topology
	int** topology;

	scene();
	~scene();

	//init scene(done)
	void init(population& input,int* agents_index,int agents_num);

	//simulate next step(done)
	//returns true if possible to do one more step
	bool nextstep();

	//finalize scene
	int* finalize();

	//maximal distance
	double maxdist;

	//mutation rate
	double mut_rate;

	//check agents can eat food(done)
	void obj_checkeating(agent&);

	//randomize position(done)
	void obj_posrandom(object&);

	//randomize angle(done)
	void obj_anglerandom(object&);

	//move object(done)
	void obj_move(object&, double dist = 0);

	//rotate object(done)
	void obj_rotate(object&, double angle = 0);
};
#endif