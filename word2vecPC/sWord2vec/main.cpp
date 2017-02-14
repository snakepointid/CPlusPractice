#include <chrono>
#include<iostream>
#include <stdio.h>
#include"word2vec.h"
using namespace std;
int main()
{
	SW2V myModel;
	auto cstart = std::chrono::high_resolution_clock::now();
	myModel.loadCorpus();
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	cstart = std::chrono::high_resolution_clock::now();
	myModel.processCorpus();
	cend = std::chrono::high_resolution_clock::now();
	printf("build vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	cstart = std::chrono::high_resolution_clock::now();
	myModel.trainCorpus();
	cend = std::chrono::high_resolution_clock::now();
	printf("train vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
 
}
