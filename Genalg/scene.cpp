#include "scene.h"
object::object()
{
	//white
	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
}
food::food()
{
	rad = 3;
	feedpower = 10;
	//green
	color[0] = 0;
	color[1] = 1;
	color[2] = 0;
}
big_food::big_food()
{
	rad = 6;
	feedpower = 30;
	//dark green
	color[0] = 0;
	color[1] = 0.5;
	color[2] = 0;
}
agent::agent()
{
	rad = 10;
	isdirected = 1;
	//yellow
	color[0] = 1;
	color[1] = 1;
	color[2] = 0;
};
scene::scene()
{

}
scene::~scene()
{
	obj_food.clear();
	obj_bfood.clear();
	obj_agent.clear();
}
inline double dist(object& temp1, object& temp2)
{
	return sqrt(pow(temp1.xpos - temp2.xpos, 2) + pow(temp1.ypos - temp2.ypos, 2));
}
inline double getcosineX(agent& current, object& target)
{
	return ((target.xpos - current.xpos)*cos(current.angle) + (target.ypos - current.ypos)*sin(current.angle)) / (sqrt(pow((target.xpos - current.xpos), 2) + pow((target.ypos - current.ypos), 2)));
}
void getinputvector(double* input, agent& current, object& target, double& maxdist)
{
	input[0] = (target.rad) / 20;
	input[1] = dist(current, target) / maxdist;
	input[2] = getcosineX(current, target);
	input[3] = target.color[0];
	input[4] = target.color[1];
	input[5] = target.color[2];
}
void scene::obj_anglerandom(object &temp)
{
	temp.angle = M_PI * (-1 + 2 * (((double)rand() / RAND_MAX)));
}
void scene::obj_posrandom(object &temp)
{
	temp.xpos = ((double)rand() / RAND_MAX)*maxX;
	temp.ypos = ((double)rand() / RAND_MAX)*maxY;
}
void scene::obj_checkeating(agent &temp)
{
	for (obj_food_it = obj_food.begin(); obj_food_it != obj_food.end(); ++obj_food_it)//for all food agents
	{
		if (dist(temp, *obj_food_it) < (temp.rad + obj_food_it->rad))//distance check
		{
			temp.fitness += obj_food_it->feedpower;
			obj_food.erase(obj_food_it);
			break;
		}
	}
	for (obj_bfood_it = obj_bfood.begin(); obj_bfood_it != obj_bfood.end(); ++obj_bfood_it)//for all big food agents
	{
		if (dist(temp, *obj_bfood_it) < (temp.rad + obj_bfood_it->rad))//distance check
		{
			temp.fitness += obj_bfood_it->feedpower;
			obj_bfood.erase(obj_bfood_it);
			break;
		}
	}
}
void scene::obj_move(object& temp, double dist)
{
	temp.xpos += dist*cos(temp.angle);
	temp.ypos += dist*sin(temp.angle);
}
void scene::obj_rotate(object &temp, double Xangle)
{
	temp.angle += Xangle;
}
void scene::init(population& input, int* agents_index, int agents_num)
{
	obj_food.clear();
	obj_bfood.clear();
	obj_agent.clear();

	started = 1;
	mut_rate = input.mut_rate;
	topology = input.topology;
	timer = input.options[4];
	num_segments = input.options[2];
	maxdist = sqrt(maxX*maxX + maxY*maxY);

	for (int i = 0; i < agents_num; ++i)//get num_of_agents random agents from population and init agents
	{
		agent temp;
		temp.gene_pos = agents_index[i];
		temp.self_gene = &input.genes[temp.gene_pos];

		//set colors//[-1...1] -> [0...1]
		for (int j = 0; j < 3; ++j)
			temp.color[j] = (1 + temp.self_gene->data[temp.self_gene->size - 4 + j]) / 2;

		//set radius//[-1...1] -> [0...10]
		temp.rad = (1 + temp.self_gene->data[temp.self_gene->size - 1]) * 5;

		//set speed	//[-1...1] -> [5...3]
		temp.speed = 4 - (temp.self_gene->data[temp.self_gene->size - 1]);

		obj_posrandom(temp);
		obj_anglerandom(temp);
		obj_agent.push_back(temp);
	}

	for (int i = 0; i < input.options[6]; ++i)//init food
	{
		food temp;
		temp.rad = input.options[8];
		temp.feedpower = input.options[10];
		obj_posrandom(temp);
		obj_food.push_back(temp);
	}

	for (int i = 0; i < input.options[7]; ++i)//init big food
	{
		big_food temp;
		temp.rad = input.options[9];
		temp.feedpower = input.options[11];
		obj_posrandom(temp);
		obj_bfood.push_back(temp);
	}
}
bool scene::nextstep()
{
	if (started && timer)//started and timer!=0
	{
		nnet mynnet;
		gene current_gene;
		gene temp_gene;
		gene nnet_gene;
		object* best_match;
		double best_score;
		double* input = new double[6];//size + distance + cosine + color x3
		double* output;
		//for all agents

		for (obj_agent_it = obj_agent.begin(); obj_agent_it != obj_agent.end(); ++obj_agent_it)
		{
			current_gene.data = obj_agent_it->self_gene->data;
			current_gene.size = obj_agent_it->self_gene->size;
			best_match = 0;
			best_score = 0;

			////upload genes to nnet1
			gene_seperate(current_gene, weightsnum(topology[0]), nnet_gene, temp_gene);
			mynnet.init(topology[0]);
			mynnet.importweights(nnet_gene);

			//	check all agent + nnet1 + nnet2 (dist to them + )
			for (obj_agent_it2 = obj_agent.begin(); obj_agent_it2 != obj_agent.end(); ++obj_agent_it2)//other agent
			{
				if (obj_agent_it2 == obj_agent_it) continue;//anti-self processing
				getinputvector(input, *obj_agent_it, *obj_agent_it2, maxdist);
				output = mynnet.fforward(input);
				if (*output > best_score)
				{
					best_score = *output;
					best_match = &(*obj_agent_it2);
				}
				delete[] output;
			}

			for (obj_food_it = obj_food.begin(); obj_food_it != obj_food.end(); ++obj_food_it)//food
			{
				//if (abs(getcosineX(*obj_agent_it, *obj_agent_it2)) < 0) continue;
				getinputvector(input, *obj_agent_it, *obj_food_it, maxdist);
				output = mynnet.fforward(input);
				if (*output > best_score)
				{
					best_score = *output;
					best_match = &(*obj_food_it);
				}
				delete[] output;
			}

			for (obj_bfood_it = obj_bfood.begin(); obj_bfood_it != obj_bfood.end(); ++obj_bfood_it)//big food
			{
				//if (abs(getcosineX(*obj_agent_it, *obj_agent_it2)) < 0) continue;
				getinputvector(input, *obj_agent_it, *obj_bfood_it, maxdist);
				output = mynnet.fforward(input);
				if (*output > best_score)
				{
					best_score = *output;
					best_match = &(*obj_bfood_it);
				}
				delete[] output;
			}

			delete[] mynnet.weights;//	mynnet.~nnet();
			//	get best object (best_match)

			//	upload genes to nnet2
			gene_seperate(temp_gene, weightsnum(topology[1]), nnet_gene, current_gene);
			mynnet.init(topology[1]);
			mynnet.importweights(nnet_gene);

			//	get angle+speed change
			getinputvector(input, *obj_agent_it, *best_match, maxdist);
			output = mynnet.fforward(input);

			//move agent + rotate 
			obj_rotate(*obj_agent_it, M_PI * (output[0] - 0.5));
			obj_move(*obj_agent_it, obj_agent_it->speed * ((output[1] / 4) + 0.25));
			//obj_checkborders(*obj_agent_it);

			//check for eating
			obj_checkeating(*obj_agent_it);

			delete[] output;
			delete[] mynnet.weights;
		}

		//if all basic food && all big food is eaten -> timer = 0;
		if (!(obj_food.size() || (obj_bfood.size()))) timer = 0;

		if (timer > 0) timer--;
		delete[] input;
		//delete[] output;
		return 1;
	}
	started = 0;
	return 0;
}
bool compare_fitness(const agent& first, const agent& second)
{
	return (first.fitness > second.fitness);
}
int* scene::finalize()
{
	started = 0; 
	int* results = new int[obj_agent.size()];
	int i = 0;
	for (obj_agent_it = obj_agent.begin(); obj_agent_it != obj_agent.end(); ++obj_agent_it)
	{
		results[i] = obj_agent_it->fitness;
		++i;
	}
	return results;
}
/*
void randomize(vector<int> &arr)
{
vector<int> visited(arr.size(), 0);
for (int i = 0; i < arr.size(); ++i)
{
int randpos = rand() % arr.size();
while (visited[randpos])
randpos = (randpos + 1) % arr.size();
if (visited[randpos] == 0)
{
visited[randpos]++;
arr[randpos] = i;
}

}

}
*/







