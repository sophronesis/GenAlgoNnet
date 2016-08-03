#include "scene_glut.h"
#include "metascene.h"
#include <fstream>
struct settings_t
{
	int* options = 0;
	int** topology = 0; 
	double mut_rate = 0;
};
void initrand()
{
	//init pseudorandom generator
	srand((unsigned int)time(0));
}
settings_t& readsettings()
{
	/*
	0	:screen wight
	1	:screen height
	2 	:object render segments
	3	:generations needed
	4	:time per scene
	5	:population size
	6	:small food num
	7	:big food num
	8	:small food size
	9	:big food size
	10	:small food value
	11	:big food value
	x	:inner topology
	x.1	:topology for each nnet
	*/
	settings_t* result = new settings_t;

	result->options = new int[13];
	result->topology = new int*[2];
	std::ifstream finp;
	finp.open("data\\settings.cfg");
	//if file is missing
	if (!finp)
	{
		std::ofstream fout;
		fout.open("data\\settings.cfg");
		fout << 640 << std::endl
			<< 480 << std::endl
			<< 35 << std::endl
			<< 10 << std::endl
			<< 1500 << std::endl
			<< 20 << std::endl
			<< 30 << std::endl
			<< 5 << std::endl
			<< 3 << std::endl
			<< 6 << std::endl
			<< 10 << std::endl
			<< 30 << std::endl
			<< 3 << " " << 6 << " " << 5 << " " << 1 << std::endl
			<< 3 << " " << 6 << " " << 5 << " " << 2 << std::endl
			<< 0.05 << std::endl;
		fout.close();
		finp.open("data\\settings.cfg");
	}

	for (int i = 0; i < 12; ++i)
		finp >> result->options[i];

	for (int i = 0; i < 2; ++i)
	{
		int n;
		finp >> n;
		result->topology[i] = new int[n + 1];
		result->topology[i][0] = n;

		for (int j = 0; j < n; ++j)
			finp >> result->topology[i][j + 1];
	}
	if (result->topology[0][1] != 6 || result->topology[1][1] != 6 || result->topology[0][result->topology[0][0]] != 1 || result->topology[1][result->topology[1][0]] != 2)
	{
		std::cout << "Invalid settings file: nnet topology isn't supported." << std::endl;
		std::cin.get();
		exit(0);
	}
	result->options[12] = 4;
	finp >> result->mut_rate;
	return *result;
}
int main(int argc, char ** argv)
{
	initrand();
	settings_t settings = readsettings();
	population test_pop;
	test_pop.init(settings.topology, settings.options, settings.mut_rate);
	char n;
	char file[20];
	std::cout << "usage: r (for random initital population)\n       l [path/to/file] (load pretrained population)" << std::endl;
	std::cin >> n >> file;
	if (n == 'l') test_pop.readfromfile(true,file);
	startscene(argc, argv, test_pop);
	system("pause");
	return 0;
}
//test matrixes
/*
std::ostream &outputmatrix(std::ostream &output, matrix* MatrixToOut)
{

for (int j = 0; j < MatrixToOut->ysize; ++j)
{
for (int i = 0; i < MatrixToOut->xsize; ++i)
output << std::setw(6) << MatrixToOut->val[i][j] << " |";
output << std::endl;
}
output << std::endl;

return output;
}
void main()
{
matrix abc;
std::cout.precision(2);
abc.resize(9,10);
abc.randomize();
outputmatrix(std::cout, &abc);
system("pause");
}
*/