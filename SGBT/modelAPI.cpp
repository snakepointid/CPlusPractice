#include<chrono>
#include<vector>
#include<memory>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string>
#include "dataset.hpp"
#include "common.hpp"
#include"Bin.hpp"
#include "boost.hpp"
using namespace std;
 
int main()
{
//load datas
	
	string trainFileName = "train.csv";
	string testFileName = "test.csv";
	string targetName = "label";
	SGBT::dataset traindata;
	SGBT::dataset testdata;
	SGBT::boost boost(500,1);
	SGBT::bin binMapper(0.02);
	//load data
	auto cstart = std::chrono::high_resolution_clock::now();
	traindata.loadDataFromCsv(trainFileName, targetName);
	testdata.loadDataFromCsv(testFileName, targetName);
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load data: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	//preprocess data
	cstart = std::chrono::high_resolution_clock::now();
	binMapper.constructBins(traindata.rawData);
	binMapper.binMapper(traindata);
	cend = std::chrono::high_resolution_clock::now();
	printf("preprocess data: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	//train data
	cstart = std::chrono::high_resolution_clock::now();
	boost.train(traindata, binMapper.binLength, 10000);
	cend = std::chrono::high_resolution_clock::now();
	printf("train tree: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	//predict data
	cstart = std::chrono::high_resolution_clock::now();
	binMapper.binMapper(testdata);
	boost.predict(testdata.BinedData);
	cend = std::chrono::high_resolution_clock::now();
	printf("predict tree: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	string foutName = "out.csv";
	testdata.writeOutToCsv(foutName );
}
 