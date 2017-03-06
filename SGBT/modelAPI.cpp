#include<chrono>
#include<vector>
#include<memory>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string>
#include "dataset.hpp"
#include "common.hpp"
#include "boost.hpp"
using namespace std;
 
int main()
{
//load datas
	auto cstart = std::chrono::high_resolution_clock::now();
	string datasetFileName = "train.csv";
	string targetName = "label";
	SGBT::dataset dataset;
	dataset.loadDataFromCsv(datasetFileName, targetName);
	dataset.getSpan();
	dataset.binMapper();
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load data: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	//train data
	
	cstart = std::chrono::high_resolution_clock::now();
	SGBT::boost boost(500, 1, 3, 30);
	boost.train(dataset, 10);
	cend = std::chrono::high_resolution_clock::now();
	printf("train tree: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
 
}
 