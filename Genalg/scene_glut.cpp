#include "scene_glut.h"
metascene* dflt = 0;
population* pop = 0;
std::ofstream flog; //log file
int  cure_gen = 0;  //current generation
int  max_gen = 2000;  //max generations
bool sce_strt = 0;	//is scene start
bool sce_done = 0;	//is scene done
bool sim_strt = 0;	//is simulation started
bool sim_done = 0;  //is simulation is done
int ScreenX;
int ScreenY;

//convert coords from (0...max) -> (-1...+1)
float cconv(float coord, float max)
{
	return (float)(-1 + 2 * (coord / max));
}

//render scene
inline void renderobject(object& temp1, scene& temp2, int segments = 35)
{
	glColor3f(temp1.color[0], temp1.color[1], temp1.color[2]);
	float Xangle = temp1.angle;
	float r = temp1.rad;
	if (!temp1.isdirected) Xangle = 0;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0 * M_PI * (float)i / (float)segments + Xangle;
		float xpos = cconv(temp1.xpos + (r * cosf(angle)), temp2.screenX);
		float ypos = -cconv(temp1.ypos + (r * sinf(angle)), temp2.screenY);
		glVertex2f(xpos, ypos);
	}
	if (temp1.isdirected) glVertex2f(cconv(temp1.xpos, temp2.screenX), -cconv(temp1.ypos, temp2.screenY));
	glEnd();
}

//render scene
void renderscene(scene& input)
{
	for (input.obj_agent_it = input.obj_agent.begin(); input.obj_agent_it != input.obj_agent.end(); ++input.obj_agent_it)//agents
		renderobject(*input.obj_agent_it, input, input.num_segments);

	for (input.obj_food_it = input.obj_food.begin(); input.obj_food_it != input.obj_food.end(); ++input.obj_food_it)//food
		renderobject(*input.obj_food_it, input, input.num_segments);

	for (input.obj_bfood_it = input.obj_bfood.begin(); input.obj_bfood_it != input.obj_bfood.end(); ++input.obj_bfood_it)//big food
		renderobject(*input.obj_bfood_it, input, input.num_segments);

	//borders
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);//white
	float xpos = cconv(input.maxX, input.screenX);
	float ypos = -cconv(input.maxY, input.screenY);;
	glVertex2f(-0.9999, 0.9999);
	glVertex2f(-0.9999, ypos);
	glVertex2f(xpos, ypos);
	glVertex2f(xpos, 0.9999);
	glEnd();
}

//reshaping
void reshape(int w, int h)
{
	ScreenX = w;
	ScreenY = h;
	dflt->myscene.screenX = ScreenX;
	dflt->myscene.screenY = ScreenY;
	glViewport(0, 0, w, h);
}

//render window
void render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	renderscene(dflt->myscene);
	glutSwapBuffers();
}

void idle()
{
	//Sleep(10);

	if (sim_strt)
	{
		if (sce_done)
		{
			//finalize
			render();
			//flog << "\"" << cure_gen << "\":";
			dflt->finalize();
			if (cure_gen < max_gen - 1) dflt->add_comma();
			//if (cure_gen % 20 == 19)flog << "'+" << std::endl << "'";
			sce_done = 0;
			sce_strt = 0;
			if (cure_gen + 1 == max_gen) sim_done = 1;
		}
		else if (sce_strt)
		{
			//next step
			render();
			if (!dflt->nextstep())
				sce_done = 1;
		}
		else
		{
			//init
			cure_gen++;
			sce_strt = 1;
			dflt->myscene.screenX = ScreenX;
			dflt->myscene.screenY = ScreenY;
			dflt->myscene.maxX = pop->options[0];
			dflt->myscene.maxY = pop->options[1];
			dflt->init(*pop);
			std::cout << "Generation : " << std::setw(3) << cure_gen << std::endl;
		}
	}
	else
	{
		cure_gen = -1;
		sce_strt = 0;
		sce_done = 0;
		max_gen = pop->options[3];
		sim_strt = 1;
		dflt->init_log(pop->population_size);
	}
	if (sim_done)
	{
		//exit + save population
		dflt->finalize_log();
		pop->writetofile(1, "output.popl");
		exit(0);
	}


	/*
	
	if (!sim_strt)
	{
		dflt->init(*pop);
		sce_done = 0;
		max_gen = pop->options[3];
		sim_strt = 1;
	}
	else 
		
		
	if (sim_done)
	{
		pop->writetofile(1, "output");
		exit(0);
	}
	else
		if (dflt->nextstep())
			sce_done = 1;
	*/

	/*
	if (sim_strt)
	{
		if (sce_done)
		{
			render();
			//flog << "\"" << cure_gen << "\":";
			dflt->finalize(flog);
			if (cure_gen <  max_gen - 1) flog << ",";
			if (cure_gen % 20 == 19)flog << "'+" << std::endl << "'";
			sce_done = 0;
			sce_strt = 0;
			if (cure_gen - 1 == max_gen) sim_done = 1;
		}
		else if (sce_strt)
		{
			render();
			if (!dflt->nextstep())
				sce_done = 1;
		}
		else
		{
			cure_gen++;
			sce_strt = 1;
			dflt->screenX = ScreenX;
			dflt->screenY = ScreenY;
			dflt->maxX = pop->options[0];
			dflt->maxY = pop->options[1];
			dflt->init(*pop);
			std::cout << "Generation : " << std::setw(3) << cure_gen << std::endl;
		}
	}
	else
	{
		cure_gen = -1;
		sce_strt = 0;
		sce_done = 0;
		max_gen = pop->options[3];
		sim_strt = 1;
		flog.open("data/log.txt");
		flog << "'{\"N\":" << pop->population_size << ",";
		flog << "\"I\":[";
	}
	if (sim_done)
	{
		flog << "]}'";
		flog.close();
		pop->writetofile(1, "output");
		exit(0);
	}
	*/
}

void startscene(int argc, char ** argv, population& popul)
{
	dflt = new metascene;
	pop = &popul;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);//some magic numbers
	glutInitWindowSize(pop->options[0], pop->options[1]);
	glutCreateWindow("Genetic algorithms over neural networks beta");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();
}