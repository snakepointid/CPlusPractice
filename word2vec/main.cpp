#include <chrono>
#include<iostream>
#include<vector>
#include <stdio.h>
#include"word2vec.h"
using namespace std;
 
void initHyperPara(float &alpha, float & min_alpha, int &negSamples, int &window, int &wordDim, int &min_count, float &subsample, int &spanNum, float & maxExp);
int main()
{
	float alpha = 0.1;
	float min_alpha = 0.001;
	int negSamples = 10;
	int window = 3;
	int wordDim = 100;
	int min_count = 1;
	float subsample = 0.001;
	int spanNum = 1000;
	float maxExp = 6.0;
	string mode,corpusFN, WVloadFN;
	printf("please choose your mode:\n'train'--for training corpus\n'test'--for test word vec similar\n");
	cin >> mode;
	if (mode == "train")
	{
		initHyperPara(alpha, min_alpha, negSamples, window, wordDim, min_count, subsample, spanNum, maxExp);
		SW2V myModel(alpha, min_alpha, negSamples, window , wordDim,min_count, subsample,spanNum, maxExp);
		printf("please enter your corpus file name:");
		cin >> corpusFN;
		printf("\nplease enter your word vec save file name:");
		cin >> WVloadFN;

		auto cstart = std::chrono::high_resolution_clock::now();
		myModel.loadCorpus(corpusFN);
		auto cend = std::chrono::high_resolution_clock::now();
		printf("load corpus: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

		cstart = std::chrono::high_resolution_clock::now();
		myModel.processCorpus();
		cend = std::chrono::high_resolution_clock::now();
		printf("build vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
		int loopt = 100;
		while (loopt>0)
		{
			cstart = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < loopt; i++)myModel.trainCorpus();
			cend = std::chrono::high_resolution_clock::now();
			printf("train vocab: %.4f seconds per loop\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0/ loopt);
			myModel.testWordSimilar();
			printf("do you want to contitue training, enter your loop times:(0 be quit)\n");
			cin >> loopt;
		}


		cstart = std::chrono::high_resolution_clock::now();
		myModel.saveWord2vec(WVloadFN);
		cend = std::chrono::high_resolution_clock::now();
		printf("saveWord2vec: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	}
	if (mode == "test")
	{
		SW2V myModel(alpha, min_alpha, negSamples, window, wordDim, min_count, subsample, spanNum, maxExp);
		printf("\nplease enter your word vec save file name:");
		cin >> WVloadFN;
		auto cstart = std::chrono::high_resolution_clock::now();
		myModel.loadWord2vec(WVloadFN);
		auto cend = std::chrono::high_resolution_clock::now();
		printf("loadWord2vec: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
		myModel.testWordSimilar();

	}
}
 
void initHyperPara(float & alpha, float & min_alpha, int & negSamples, int & window, int & wordDim, int & min_count, float & subsample, int & spanNum, float & maxExp)
{
	printf("the default parameters' values are:\n------------------------------------\n");
	printf("#1 alpha     : %.4f---control the model learning rate \n", alpha);
	printf("#2 min_alpha : %.4f---control the model smallest learning rate,because the learning rate decay in Adam \n", min_alpha);
	printf("#3 negSamples: %lu---the negtive sampling numbers \n", negSamples);
	printf("#4 window    : %lu---the context window size \n", window);
	printf("#5 wordDim   : %lu---the word vec dimemsion \n", wordDim);
	printf("#6 min_count : %lu---determine which word is infrequency word \n", min_count);
	printf("#7 subsample : %.4f---control the subsampling rate \n", subsample);
	printf("#8 spanNum   : %lu---the sigmoid projection table size,recommend keep default \n", spanNum);
	printf("#9 maxExp    : %.4f---the sigmoid projection table max and min zone,recommend keep default \n", maxExp);
	printf("choose the parameter number(eta. alpha is 1) you want to change(0 to quit):");
	int c;
	cin >> c;
	while (c != 0)
	{
		switch (c)
		{
		case 0: break;
		case 1: printf("\nalpha   :"); cin >> alpha; break;
		case 2: printf("\nmin_alpha   :"); cin >> min_alpha; break;
		case 3: printf("\nnegSamples   :"); cin >> negSamples; break;
		case 4: printf("\nwindow   :"); cin >> window; break;
		case 5: printf("\nwordDim   :"); cin >> wordDim; break;
		case 6: printf("\nmin_count   :"); cin >> min_count; break;
		case 7: printf("\nsubsample   :"); cin >> subsample; break;
		case 8: printf("\nspanNum   :"); cin >> spanNum; break;
		case 9: printf("\nmaxExp   :"); cin >> maxExp; break;
		default:printf("no such parameter,please choose again"); break; 		
		}
		printf("choose the parameter you want to change(0 to quit):");
		cin >> c;
	}
}
