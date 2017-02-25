#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include"MatrComp/vecCompApi.h"
#include"NeuronLayers/s_convolayers.h"
#include"NeuronLayers/s_basiclayers.h"
using namespace std;
using std::vector;
using Vector = vector<float>;
void basiclayertest();
 
void main()
{  
	
	basiclayertest();
}

void basiclayertest()
{
	vector<Vector> input;
	SVC::initialMatrix(1, input, 30, 100);
	SVC::scale(input, 1000);
 
	convoLayer fcl(100, 5, 100), tcl(100, 5, 100); basicLayer fbl(1, 100), bl(1 ,100); actLayer fal("sigmoid"), al("sigmoid");
	vector<Vector> hidden,chidden ,flag; vector<Vector> hiout, chiout,output, error,herror,cerror,tmp;
	chidden = fcl.actNeuron(input);
	hidden = fbl.actNeuron(chidden);
	flag = fal.actNeuron(hidden);
	int m = 100;
	
	while (m-- > 0)
	{
		chiout = tcl.actNeuron(input);
		hiout = bl.actNeuron(chidden);
		output = al.actNeuron(hiout);
		error = SVC::PairWiseMinus(flag, output);
		cout << SVC::SUM(SVC::MAP(error, ::abs)) << endl;
		herror = al.updateWeight(error, hiout);
		cerror = bl.updateWeight(herror, chidden,0.1,0.0001);
		tmp = tcl.updateWeight(cerror, input, 0.1, 0.0001);
	}

}

 