#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include<chrono>
using namespace std;
void selectMode(string&mode);
void readDataFromFile(string&fn, vector<map<int, double>>&data);
void readDataFromFile(string&fn, map<int, double>&weights);
void readDataFromFile(string&fn, vector<map<int, double>>&data, map<int, double>&weights, random_device&rd);
void assTest(vector<map<int, double>>&data, map<int, double>&weights);
void trainModel(vector<map<int, double>>&data, map<int, double>&weights,string&fn, mt19937&g);
double vecnorm(map<int, double>&w1, map<int, double>&w2);
void setHyperPara();
double classify(map<int, double>&feature, map<int, double>&weights);
double sgmoid(double logit);
vector<string>splits(const string&s, char dim, vector<string>&elements);
vector<string>splits(const string&s, char dim);
//set hyper parameters
static double eta = 0.001;
static double l1 = 0.0001;
static double eps = 0.0005;
static unsigned int maxiter = 50000;
int main()
{

	//filename to save data
	string finame1 = "train.dat";
	string finame2 = "weight.dat";
	string foname = "weight.dat";
	//set random set
	random_device rd;
	mt19937 g(rd());
	 //select the training model
	string mode;
	selectMode(mode);
	//choose the model
	if (mode == "trw")
	{
		//read rawdata from files and initial random weights
		vector<map<int, double>>traindata;
		map<int, double>weights;
		setHyperPara();
		cout << "        please enter your traindata file's name: ";
		cin >> finame1;
		cout << "\nplease enter your trained weights file's name: ";
		cin >> finame2;
		cout << "\n            file name to save trained weights: ";
		cin >> foname;
		readDataFromFile(finame1, traindata);
		readDataFromFile(finame2, weights);
		trainModel(traindata, weights, foname, g);
		 
	}
	else if (mode == "tr")
	{
		//read rawdata from files and initial random weights
		vector<map<int, double>>traindata;
		map<int, double>weights;
		setHyperPara();
		cout << "please enter your traindata file's name: ";
		//cin >> finame1;
		cout << "\n     file name to save trained weights: ";
		//cin >> foname;
		readDataFromFile(finame1, traindata, weights,rd);
		trainModel(traindata, weights, foname, g);
	}
	else if (mode == "te")
	{
		//read testdata and trained weights from files
		vector<map<int, double>>testdata;
		map<int, double>weights;
		cout << "        please enter your traindata file's name: ";
		cin >> finame1;
		cout << "\nplease enter your trained weights file's name: ";
		cin >> finame2;
		readDataFromFile(finame1, testdata);
		readDataFromFile(finame2, weights);
		assTest(testdata, weights);
	}
    return(1);
}

void selectMode(string&mode)
{
	cout << "please choose your training mode:\n";
	cout << "'trw':train data with trained weights\n";
	cout << "'tr' :only train data with random initial weights\n";
	cout << "'te' :test data\n";
	cin >> mode;
}

void readDataFromFile(string & fn, vector<map<int, double>>& data)
{
	ifstream fin;
	string line;
	vector<string>tokens;
	vector<string>feat_val;
	map<int, double>example;
	fin.open(fn);
	while (getline(fin, line))
	{
		if(line.empty()|line[0]!='#'|line[0] != ' ')continue;			
		tokens = splits(line, ' ');
		//get labels
		example[0] = (atoi(tokens[0].c_str()) == 1) ? 1 : 0;
		for (int i = 1; i < tokens.size(); i++)
		{
			feat_val = splits(tokens[i], ':');
			if (feat_val.size() == 2)example[atoi(feat_val[0].c_str())] = atof(feat_val[1].c_str());
		}
				//get the key and value pairs									 				 
		data.push_back(example);
		example.clear();		
	}
	fin.close();
}

void readDataFromFile(string & fn, map<int, double>& weights)
{
	ifstream fin;
	string line;
	vector<string>feat_val;
	fin.open(fn);
	while (getline(fin, line))
	{
		//get the key and value pairs
		feat_val = splits(line, ':');
		if (feat_val.size() == 2)			 
			weights[atoi(feat_val[0].c_str())] = atof(feat_val[1].c_str());					 
	}
	fin.close();
}

void readDataFromFile(string & fn, vector<map<int, double>>& data, map<int, double>& weights,random_device&rd)
{
	ifstream fin;
	string line;
	vector<string>tokens;
	vector<string>feat_val;
	fin.open(fn);
	while (getline(fin, line))
	{
		if (line.length())
			if (line[0] != '#'&&line[0] != ' ')
			{
				map<int, double>example;
				tokens = splits(line, ' ');
				//get labels
				example[0] = (atoi(tokens[0].c_str()) == 1) ? 1 : 0;
				for (int i = 1; i < tokens.size(); i++)
				{
					//get the key and value pairs
					feat_val = splits(tokens[i], ':');
					if (feat_val.size() == 2)
					{
						example[atoi(feat_val[0].c_str())] = atof(feat_val[1].c_str());
						weights[atoi(feat_val[0].c_str())] = -1.0 + 2.0*(double)rd() / rd.max();

					}
				}						
				data.push_back(example);
			}
	}
	fin.close();
}

void assTest(vector<map<int, double>>& data, map<int, double>& weights)
{
	int correct = 0;
	for (int i = 0; i < data.size(); i++)
	{
		 
		int label = data[i][0];
		int predy = (classify(data[i], weights) > 0.5) ? 1 : 0;
		if ((predy==0&&label==1)|| (predy == 1 && (label == 0 || label == -1)))
			correct++;
	}
	printf("you have %i correctness\n", correct);
}
 

void trainModel(vector<map<int, double>>& data, map<int, double>& weights, string & fn, mt19937&g)
{
	cout << "now begain to train the model\n";
	cout << "training samples: " << data.size() << endl;
	cout << "feature numbers:  " << weights.size() << endl;
	double norm = 1.0;
	int n = 0;
	double mu = 0.0;
	double sumLoss = 0.0;
	map<int, double>sample;
	map<int, double>total_l1;
	vector<int>index(data.size());
	iota(index.begin(), index.end(), 0);
	for (auto it = weights.begin(); it != weights.end(); it++)
	{
		total_l1[it->first] = 0.0;
	}
    auto cstart = std::chrono::high_resolution_clock::now();
	while (norm>eps)
	{
		shuffle(index.begin(), index.end(), g);
		map<int, double>oldweights(weights);
		for (int i = 0; i < data.size(); i++)
		{
			mu += l1*eta;
			sample = data[index[i]];
			int label = sample[0];
			double predy = classify(sample, weights);
			double error = predy - label;
			sumLoss += abs(error);
			for (auto it = sample.begin(); it != sample.end(); it++)
			{
				if (it->first != 0)
				{
					weights[it->first] += eta*(label-predy)*it->second;
					double z = weights[it->first];
					if (weights[it->first] > 0.0) {
						weights[it->first] = max(0.0, (double)(weights[it->first] - (mu + total_l1[it->first])));
					}
					else if (weights[it->first] < 0.0) {
						weights[it->first] = min(0.0, (double)(weights[it->first] + (mu - total_l1[it->first])));
					}
					total_l1[it->first] += (weights[it->first] - z);
				}			
			}
		}
        norm = vecnorm(weights, oldweights);
		if(n==0||n%100==0)
        {
            auto cend = std::chrono::high_resolution_clock::now();
            printf("the norm is %1.4f,the sum loss is %1.4f,in iteration:%i,cost %.4f seconds\n", norm, sumLoss/ data.size(),n,std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
            cstart = std::chrono::high_resolution_clock::now();
        }
			
		sumLoss = 0.0;
		if(++n>maxiter)
			break;
	}
	ofstream fout;
	fout.open(fn.c_str());
	for (auto it = weights.begin(); it != weights.end(); it++)
		fout << it->first << ":" << it->second << endl;
	fout.close();
	cout << "\nsaved the weights";
}

double vecnorm(map<int, double>& w1, map<int, double>& w2)
{
	double norm = 0.0;
	double diff;
	for (auto it = w1.begin(); it != w1.end(); it++)
	{
		diff = w1[it->first] - w2[it->first];
		norm += diff*diff;
	}
	return sqrt(norm);

}

void setHyperPara()
{
	cout << "       the default learning rate is :" << eta << endl;
	cout << "the default l1 regulation lambda is :" << l1 << endl;
	cout << "the default max iteration times  is :" << maxiter << endl;
	cout << "do you want change the hyperparameters: 'y'or'n'\n";
	char c;
	cin >> c;
	if (tolower(c) == 'y')
	{
		cout << "\n       the new learning rate is :";
		cin >> eta;
		cout << "\nthe new l1 regulation lambda is :";
		cin >> l1;
		cout << "\nthe new max iteration times  is :";
		cin >> maxiter;
	}
}

double classify(map<int, double> &feature, map<int, double> &weights)
{
	double logit = 0.0;
	for (auto it = feature.begin(); it != feature.end(); it++)
		if (it->first != 0)
			logit += feature[it->first] * weights[it->first];
	return sgmoid(logit);
}

double sgmoid(double logit)
{
	static double overflow = 20.0;
	if (logit > overflow)logit = overflow;
	if (logit <-overflow)logit = -overflow;
	return 1.0 / (1.0 + exp(-logit));
}

 

vector<string> splits(const string & s, char dim, vector<string>& elements)
{
	stringstream ss(s);
	string item;
	while (1)
	{
		getline(ss,item,dim);
		if(item.empty())break;
		elements.push_back(move(item));
	}		
	return elements;
}

vector<string> splits(const string & s, char dim)
{
	vector<string> elements;	
	return splits(s, dim, elements);
}
