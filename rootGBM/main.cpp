#include<chrono>
#include<vector>
#include<memory>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string>
#include "include/Bin.h"
#include "include/boost.h"
using namespace rootGBM;
int main() {
	string trainFileName = "train.csv";
	string testFileName = "test.csv";
	string targetName = "label";
	std::unique_ptr<dataset> traindata(new dataset(1));
	std::unique_ptr<dataset> testdata(new dataset(1));
	std::unique_ptr<boost> boost(new boost(5000,0.1,8));
	std::unique_ptr<bin> bin(new bin(100));
 
	//load data
	auto cstart = std::chrono::high_resolution_clock::now();
	traindata->loadDataFromCsv(trainFileName, targetName,10000,10);
	testdata->loadDataFromCsv(testFileName, targetName, 2000, 10);
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load data: %.4f seconds,total %lu samples,%lu features\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0, traindata->observe_num, traindata->feature_num);
	//preprocess data
	cstart = std::chrono::high_resolution_clock::now();
	bin->constructBins(traindata.get());
	bin->binMapper(traindata.get());
	cend = std::chrono::high_resolution_clock::now();
	printf("preprocess data: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	//train data
	cstart = std::chrono::high_resolution_clock::now();
	boost->train(traindata.get(), bin.get(),1);
	cend = std::chrono::high_resolution_clock::now();
	printf("train tree: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	//predict data
	cstart = std::chrono::high_resolution_clock::now();
	bin->binMapper(testdata.get());
	boost->predict(testdata.get());
	cend = std::chrono::high_resolution_clock::now();
	printf("predict tree: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	string foutName = "out.csv";
	testdata->writeOutToCsv(foutName);
}