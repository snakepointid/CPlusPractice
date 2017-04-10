#include <chrono>
#include<iostream>
#include<vector>

using namespace std;

int main()
{
	//float alpha =1;
	//float min_alpha = 0.01;
	//	float l1regular_=0.0001;
	//int window = 5;
	//int wordDim = 100;
	//int hiddenDim = 20;
	//int min_count = 0;
	// 
	//string mode("train"),corpusFN("brown.txt"), WVloadFN("wordvec.txt");
	//printf("please choose your mode:\n'train'--for training corpus\n'test'--for test word vec similar\n");
	////cin >> mode;
	//if (mode == "train")
	//{
	//	 
	//	SW2V myModel(alpha, min_alpha,l1regular_, window , wordDim,hiddenDim,min_count );
	//	myModel.initHyperPara();
	//	printf("please enter your corpus file name:");
	//	//cin >> corpusFN;
	//	printf("\nplease enter your word vec save file name:");
	//	//cin >> WVloadFN;

	//	auto cstart = std::chrono::high_resolution_clock::now();
	//	myModel.loadCorpus(corpusFN);
	//	auto cend = std::chrono::high_resolution_clock::now();
	//	printf("load corpus: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	//	cstart = std::chrono::high_resolution_clock::now();
	//	myModel.processCorpus();
	//	cend = std::chrono::high_resolution_clock::now();
	//	printf("build vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	//	int loopt = 1;
	//	while (loopt>0)
	//	{
	//		cstart = std::chrono::high_resolution_clock::now();
	//		 
	//		for (int i = 0; i < loopt; i++)myModel.trainCorpus();
	//		cend = std::chrono::high_resolution_clock::now();
	//		printf("train vocab: %.4f seconds per loop\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0/ loopt);
	//		myModel.testWordSimilar();
	//		printf("do you want to contitue training, enter your loop times:(0 be quit)\n");
	//		cin >> loopt;
	//	}


	//	cstart = std::chrono::high_resolution_clock::now();
	//	myModel.saveWord2vec(WVloadFN);
	//	cend = std::chrono::high_resolution_clock::now();
	//	printf("saveWord2vec: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

	//}
	//if (mode == "test")
	//{
	//	SW2V myModel(alpha, min_alpha, l1regular_, window, wordDim,hiddenDim, min_count);
	//	printf("\nplease enter your word vec save file name:");
	//	cin >> WVloadFN;
	//	auto cstart = std::chrono::high_resolution_clock::now();
	//	myModel.loadWord2vec(WVloadFN);
	//	auto cend = std::chrono::high_resolution_clock::now();
	//	printf("loadWord2vec: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
	//	myModel.testWordSimilar();

	//}
	struct test {
		int age;
		vector<int> weight;
	 };
	test *a = new test{ 1 };
	std::cout << a->age << " " << a->weight.capacity();
	delete a;
}


