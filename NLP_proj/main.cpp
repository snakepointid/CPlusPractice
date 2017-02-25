#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include"vecCompApi.h"
#include"s_basiclayers.h"
using namespace std;
using std::vector;
using Vector = vector<float>;
void basiclayertest();
void basiclayertest2();
void basiclayertest4();
void basiclayertest3();
void main()
{  
	
	basiclayertest4();
}
 

void basiclayertest()
{
	Vector input;
	SVC::initialMatrix(1, input, 100);
	vector<Vector> weights;
	SVC::initialMatrix(1, weights, 10, 100);
	Vector bias;
	SVC::initialMatrix(1, bias, 10);

	basicLayer bl(10, 100); Vector flag; Vector output; Vector error; basicLayer b(10, 100);
	bl.setWeights(weights, bias);
	flag.swap(bl.actNeuron(input));
	int m = 1000;
	while (m-- > 0)
	{
		output.swap(b.actNeuron(input));
		error.swap(SVC::PairWiseMinus(flag, output));
		cout << SVC::SUM(SVC::MAP(error, ::abs)) << endl;
		b.saveGradient(error, input);
		if (m % 10 == 0)b.updateWeight(0.01, 0.0001);
	}
}


void basiclayertest2()
{
	vector<Vector> input;
	SVC::initialMatrix(1, input, 500, 100);
	vector<Vector> weights;
	SVC::initialMatrix(1, weights, 10, 100);
	Vector bias;
	SVC::initialMatrix(1, bias, 10);

	basicLayer bl(10, 100); vector<Vector> flag; vector<Vector> output; vector<Vector> error; basicLayer b(10, 100);
	bl.setWeights(weights, bias);
	flag.swap(bl.actNeuron(input));
	int m = 1000;
	while (m-- > 0)
	{
		output.swap(b.actNeuron(input));
		error.swap(SVC::PairWiseMinus(flag, output));
		cout << SVC::SUM(SVC::MAP(error, ::abs)) << endl;
		b.saveGradient(error, input);
		if (m % 1 == 0)b.updateWeight(0.00001, 0.00001);
	}
}


void basiclayertest3()
{
	//initial matrix
	vector<Vector> input;
	SVC::initialMatrix(1, input, 5, 100);
	vector<Vector> weights1;
	SVC::initialMatrix(1, weights1, 20, 100);
	Vector bias1;
	SVC::initialMatrix(1, bias1, 20);
	vector<Vector> weights2;
	SVC::initialMatrix(1, weights2, 1, 20);
	Vector bias2;
	SVC::initialMatrix(1, bias2, 1);

	basicLayer a1(20, 100); basicLayer a2(1, 20);
	basicLayer b1(20, 100); basicLayer b2(1, 20);
	a1.setWeights(weights1, bias1);
	a2.setWeights(weights2, bias2);
	vector<Vector> flag; vector<Vector> output; vector<Vector> error,herror; vector<Vector>  hinput, hidden;
	hinput.swap(a1.actNeuron(input));
	flag.swap(a2.actNeuron(hinput));

	int m = 10;
	while (m-- > 0)
	{
		hidden.swap(b1.actNeuron(input));
		output.swap(b2.actNeuron(hidden));

		error.swap(SVC::PairWiseMinus(flag, output));
		cout << SVC::SUM(SVC::MAP(error, ::abs)) << endl;
		herror.swap(b2.updateWeight(error, hidden,0.002,0));
		b1.updateWeight(herror, input, 0.002, 0);
		 
	}
 
	
}


void basiclayertest4()
{
	//initial matrix
	vector<Vector> input;
	SVC::initialMatrix(1, input, 500, 100);
	vector<Vector> input1;
	SVC::initialMatrix(1, input1, 500, 100);
	actLayer ater("sigmoid"); 

	SVC::scale(input, 100);
 
	vector<Vector> flag= ater.actNeuron(input);
	vector<Vector> output, error,inputerror;
	 

	int m = 100;
	while (m-- > 0)
	{
		output.swap(ater.actNeuron(input1));
		error.swap(SVC::PairWiseMinus(flag, output));
		cout << SVC::SUM(SVC::MAP(error, ::abs)) << endl;
		inputerror.swap(ater.updateWeight(error, input1));
		SVC::saxpy(input1, 0.5, inputerror);
		 
		//cout << SVC::SUM(SVC::MAP(SVC::PairWiseMinus(input1, input), ::abs)) << endl;
	}


}