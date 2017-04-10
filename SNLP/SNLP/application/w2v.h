#ifndef W2V_H_
#define W2V_H_
#include<memory>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<sstream>
#include"../common/matrInit.h"
#include"../common/saxpy.h"
#include"../common/meta.h"
#include"../common/strProcess.h"
namespace SNLP {
	struct SW2V
	{
	private:
		struct Sentence
		{
			vector<string>tokens_;
			vector<int>indexs_;
		};
		struct Word
		{
			string token_;
			int index_;
			int freqCount_;
		};
		using SentenceP = std::unique_ptr<Sentence>;
		using WordP     = std::unique_ptr<Word>;
		//members
		vector<SentenceP>     sentences_;
		vector<WordP>         wordlist_;
		std::map<string, int> tokens2index_;
		//wordvec
		vector<Fvector>  wordvec_;
		vector<Fvector>  negwordvec_;
		//vector<Fvector>  wordvecHold_;
		//vector<Fvector>  negwordvecHold_;
		int  total_distinct_words_ = 0;
		int  total_text_words_     = 0;
		int lastIndex_;
		Ivector unigram_;
		//controler	
		float alpha_;
		int minibatch_;
		int window_;
		int wordDim_;
		int min_count_;
		int _initPara();
		int _batchTrain(Ivector&sentence);
	public:
		//construct funcs
		SW2V(float alpha = 0.1, float l1regular = 0.0001, int window = 5, int wordDim = 100, int minibatch = 100, int min_count = 1)
			:alpha_(alpha) , window_(window), wordDim_(wordDim), minibatch_(minibatch), min_count_(min_count){};
		//functions
		//processCorpus method which can get a vocabulary staticstic and transfer the sentences' tokens to index
		int build_vocabulary();
		int trainWordVec();
		int getSimilarWords();
		int saveWordVec(string&fn);
		int loadWordVec(string&fn);
		int loadRawText(string&fn);	
		
	};
}
#endif // !W2V_H_