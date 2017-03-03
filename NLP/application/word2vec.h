//
//  snakeWord2Vec.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//
#include <vector>
#include <list>
#include <string>
#include<iostream>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <random>
#include <memory>
#include <fstream>

#include <sstream>
#include <map>
#include <chrono>
#include <stdio.h>
//#include <omp.h>
#include "../NeuronLayers/s_convolayers.h"
#include "../NeuronLayers/s_basiclayers.h"
using std::vector;
using std::string;
#ifndef snakeWord2Vec_h
#define snakeWord2Vec_h
struct SW2V
{
	struct Sentence
	{
		vector<string>tokens;
		vector<int>indexs;
	};
	using SentenceP =std::shared_ptr<Sentence>;
	//members
	vector<SentenceP>sentences;
	std::map<string, int> tokens2indexs_;
	vector<string>   indexs2tokens_;
	vector<int>		 tokensCount_;
	vector<std::pair<string, int>> vocab_;
	vector<Vector>   wordvec_;
	vector<Vector>   wordvecNorm_;
	int              total_words_ = 0;
 	vector<int> unigram_;
	//controler	
	float alpha_;
	float min_alpha_;
	int window_;
	int wordDim_;
	int hiddenDim_;
	int min_count_;
	float l1regular_;
//initial neuron layers
	
	//construct funcs
	SW2V(float alpha = 0.1, float min_alpha = 0.001,  float l1regular = 0.0001, int window = 5, int wordDim = 100,int hiddenDim=100,int min_count = 1)
		:alpha_(alpha), min_alpha_(min_alpha), window_(window), wordDim_(wordDim),hiddenDim_(hiddenDim),min_count_(min_count),  l1regular_(l1regular) {};
	//functions
	//processCorpus method which can get a vocabulary staticstic and transfer the sentences' tokens to index
	int processCorpus()
	{
		//last_token to hold the previews token
		string last_token;
		//tokensCount which temperary count the tokens numbers
		std::unordered_map<string, int>tokensCount;
		//count the token numbers
		for (auto &sentence : sentences)
		{
			if (sentence->tokens.empty())continue;
			for (auto &token : sentence->tokens)
			{
				total_words_ += 1;
				tokensCount[token] += 1;
				last_token = token;
			}
		}
		printf("the corpus have total %lu [words] with %lu distinct [tokens]\n", total_words_, tokensCount.size());
		if (total_words_ < 1)return -1;
		//initial members 1st
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);
		int n_words = tokensCount.size();
		vocab_.reserve(n_words);
		indexs2tokens_.reserve(n_words);
		tokensCount_.reserve(n_words);
		for (auto&pr : tokensCount)
		{
			vocab_.push_back(move(make_pair(pr.first, pr.second)));
		}
		auto comp = [](std::pair<string, int>&v1, std::pair<string, int>&v2) {return v1.second > v2.second; };
		std::sort(vocab_.begin(), vocab_.end(), comp);
		int index = 0;
		int infreqWordCount = 0;
		for (auto&pr : vocab_)
		{
			tokens2indexs_[pr.first] = index;
			if (pr.second > min_count_) { index++; indexs2tokens_.push_back(move(pr.first)); tokensCount_.push_back(pr.second); }
			else { infreqWordCount += pr.second; }
			if (infreqWordCount>0) { indexs2tokens_.push_back("infreqWord"); tokensCount_.push_back(infreqWordCount); }
		}
		//shape those member
		n_words = indexs2tokens_.size();
		wordvec_.resize(n_words);
		 
		//inital weights
		for (auto&w : wordvec_)
		{
			w.resize(wordDim_);
			for (auto&v : w)
			{
				v = (rng(eng) - 0.5) / wordDim_;
			}
		}
		//transfer the sentences
		for (auto&sentence : sentences)
		{
			if (sentence->tokens.empty())continue;
			for (auto&token : sentence->tokens)
			{
				sentence->indexs.push_back(tokens2indexs_[token]);
			}
		}
		//ser unigram
		unigram_.resize(total_words_);
		const float power = 0.75;
		float sum = 0.0;
		for(auto&v:tokensCount_){sum+=pow((float)v,power);}
		float d1 = ::pow((float)tokensCount_[0], power) / sum;
		int i = 0;
		for (int a=0; a<unigram_.size(); ++a) 
		{
			unigram_[a] = i;
			if (float(a) / unigram_.size() > d1) {
				++i; d1 += ::pow((float)tokensCount_[i], power) / sum;
			}
			if (i >= tokensCount_.size()) i = tokensCount_.size() - 1;
		}
		return 1;
	}
	void trainCorpus()
	{
	 std::cout<<"begin to train\n";
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);
		static int currentWordCount = 0;
		static  int trainedSentences = 0;
		static  int TrainedSentences = 0;
		float alpha;
//#pragma omp parallel for
		for (int i = 0; i<sentences.size();)
		{  
			auto &sentence = sentences[i];				
			int trainWords = sentence->indexs.size();
			if(trainWords>=window_){
				if (currentWordCount > total_words_)currentWordCount = total_words_;
				alpha = std::max(min_alpha_, alpha_*(1 - (float)currentWordCount / total_words_));
				trainedSentences += 1;
				int iplus = Ordertrain(sentence->indexs, alpha);
				currentWordCount += trainWords;
				i+=iplus;
			}else{i+=1;}	
//#pragma omp atomic
			
		}
		if (trainedSentences > 1000) { TrainedSentences += trainedSentences / 1000; trainedSentences = trainedSentences % 1000; }
		printf("total trained sentences: %luk\n", TrainedSentences);

	}
	void testWordSimilar()
	{
		wordvecNorm_ = wordvec_;
		for (auto&w : wordvecNorm_)SVC::norml2(w);
		string token;
		vector<std::pair<int, float>>dist;
		dist.reserve(indexs2tokens_.size());
		while (1)
		{
			printf("please enter the word you want to test('#quit' to quit):");
			std::cin >> token;
			if (token == "#quit")break;
			printf("the word you want to test is' %s '\n", token.c_str());
			lowerTokens(token);
			auto it = tokens2indexs_.find(token.c_str());
			if (it == tokens2indexs_.end()) { printf("word is not exist!!!\n"); continue; }
			Vector &targVec = wordvecNorm_[it->second];

			for (int idx = 0; idx<wordvecNorm_.size(); idx++)
			{
                dist.push_back(std::move(std::make_pair(idx, SVC::dot(targVec, wordvecNorm_[idx]))));
			}
			auto comp = [](std::pair<int, float>&v1, std::pair<int, float>&v2) {return v1.second > v2.second; };
			std::sort(dist.begin(), dist.end(), comp);
			for (int i = 0; i < 11; i++) {
				if (it->second == dist[i].first)continue;
				printf("%lu most similar: %s \n", i, indexs2tokens_[dist[i].first].c_str());
			}
			printf("---------------------------------------------------------------------------\n");
			dist.clear();
			token.clear();
		}

	}
	string& lowerTokens(string&token)
	{
		transform(token.begin(), token.end(), token.begin(), tolower);
		return token;
	}
	string& upperTokens(string&token)
	{
		transform(token.begin(), token.end(), token.begin(), toupper);
		return token;
	}
	void saveWord2vec(string&fn)
	{
		std::ofstream fin;
		fin.open(fn);
		fin << wordvec_.size() << " " << wordvec_[0].size() << "\n";
		for (int idx = 0; idx < indexs2tokens_.size(); idx++)
		{
			fin << indexs2tokens_[idx] << " ";
			for (auto&w : wordvec_[idx]) { fin << w << " "; }
			fin << "\n";
		}
		fin.close();
	}
	int loadWord2vec(string&fn)
	{
		wordvec_.clear();
		std::ifstream fin;
		string line;
		string token;
		fin.open(fn);
		if (!getline(fin, line)) { std::cout << "there have no data\n"; return -1; }
		int n_words = 0, wordDim = 0;
		std::stringstream(line) >> n_words >> wordDim;
		wordvec_.resize(n_words);
		for (auto&w : wordvec_)w.resize(wordDim);
		int idx = 0;
		float wt;
		while (getline(fin, line))
		{
			std::stringstream ipt(line);
			ipt >> token;

			int widx = 0;
			while (ipt >> wt)
			{
				wordvec_[idx][widx++] = wt;
			}
			tokens2indexs_[token] = idx++;
			indexs2tokens_.push_back(move(token));
		}
		fin.close();
		return 1;
	}

	void loadCorpus(string&fn)
	{
		std::ifstream fin;
		SentenceP sentence(new Sentence);
		string line;
		string token;
		fin.open(fn);
		while (std::getline(fin, line))
		{
			//if (sentences.size() > 1000)break;
			if (line.empty())continue;
			std::stringstream str(line);
			while (str >> token)
			{
				sentence->tokens.push_back(move(lowerTokens(token)));
				//sentence->tokens.push_back(move(token));
			}
			sentences.push_back(move(sentence));
			sentence.reset(new Sentence);
		}
		fin.close();
		printf("have loaded %lu sentences\n", sentences.size());
	}
void initHyperPara()
{
	printf("the default parameters' values are:\n------------------------------------\n");
	printf("#1 alpha     : %.4f---control the model learning rate \n", alpha_);
	printf("#2 min_alpha : %.4f---control the model smallest learning rate,because the learning rate decay in Adam \n", min_alpha_);
	printf("#3 l1regular : %.4f---the l1 regulation which used keep from overfit \n", l1regular_);
	printf("#4 window    : %lu---the context window size \n", window_);
	printf("#5 wordDim   : %lu---the word vec dimemsion \n", wordDim_);
	printf("#6 hiddenDim : %lu---the convolutional layer dimemsion \n", hiddenDim_);
	printf("#7 min_count : %lu---determine which word is infrequency word \n", min_count_);
 
	printf("choose the parameter number(eta. alpha is 1) you want to change(0 to quit):");
	int c=0;
	//std::cin >> c;
	while (c != 0)
	{
		switch (c)
		{
		case 0: break;
        case 1: printf("\nalpha     :"); std::cin >> alpha_; break;
		case 2: printf("\nmin_alpha :"); std::cin >> min_alpha_; break;
		case 3: printf("\nl1regular :"); std::cin >> l1regular_; break;
		case 4: printf("\nwindow    :"); std::cin >> window_; break;
		case 5: printf("\nwordDim   :"); std::cin >> wordDim_; break;
		case 6: printf("\nhiddenDim :"); std::cin >> hiddenDim_; break;
		case 7: printf("\nmin_count :"); std::cin >> min_count_; break;
		default:printf("no such parameter,please choose again"); break; 		
		}
		printf("choose the parameter you want to change(0 to quit):");
	//	cin >> c;
	}
}
private:
	int Ordertrain(vector<int>&sentence, float alpha)
	{
		 
        static convoLayer conlayer_(hiddenDim_, window_, wordDim_);
		static actLayer   actlayer1_("tanh");
        static basicLayer baslayer_(1, hiddenDim_);
        static actLayer   actlayer2_("sigmoid");
		static int cot=0,cott=1000;
		static float err1,err2;
		vector<Vector> convOUT1,actOUT1,mlpOUT1,OUT1,ERR1,mlpERR1,actERR1,convERR1,wordERR1;
		vector<Vector> convOUT2,actOUT2,mlpOUT2,OUT2,ERR2,mlpERR2,actERR2,convERR2,wordERR2;
		int sentLen = sentence.size();
		 
		vector<Vector*> trueSentP(sentLen);
		vector<Vector> trueSent(sentLen);
		vector<Vector*> fakeSentP(sentLen);
		vector<Vector> fakeSent(sentLen);  
		for(int i=0;i<sentLen;i++)
		{		
			trueSentP[i] = &wordvec_[sentence[i]];
			trueSent[i]  = wordvec_[sentence[i]];
			fakeSentP[i] = &wordvec_[sentence[sentLen-1-i]];
			fakeSent[i]  = wordvec_[sentence[sentLen-1-i]];
		}	
		
	//true sentence order
		//forward 
		convOUT1 = conlayer_.actNeuron(trueSent);
		actOUT1  = actlayer1_.actNeuron(convOUT1);
		mlpOUT1  = baslayer_.actNeuron(actOUT1);
		OUT1     = actlayer2_.actNeuron(mlpOUT1);
		//backward		  
		ERR1     = SVC::PairWiseMinus(1.0, OUT1);

		mlpERR1  = actlayer2_.updateWeight(ERR1, mlpOUT1);
		actERR1 = baslayer_.updateWeight(mlpERR1, actOUT1,alpha,l1regular_);
		convERR1 = actlayer1_.updateWeight(actERR1, convOUT1);
		wordERR1 = conlayer_.updateWeight(convERR1, trueSent,alpha,l1regular_);	 
		err1  +=  SVC::MEAN(SVC::MAP(ERR1, std::abs)) ;
		//UPDATE word vector
		SVC::saxpy(trueSentP,alpha,wordERR1);
		SVC::saxpy(trueSentP,-l1regular_);	
	//fake sentence order
		convOUT2 = conlayer_.actNeuron(fakeSent);
		actOUT2  = actlayer1_.actNeuron(convOUT2);
		mlpOUT2  = baslayer_.actNeuron(actOUT2);
		OUT2     = actlayer2_.actNeuron(mlpOUT2);
		//backward		  
		ERR2     = SVC::PairWiseMinus(0.0, OUT2);

		mlpERR2  = actlayer2_.updateWeight(ERR2, mlpOUT2);
		actERR2 = baslayer_.updateWeight(mlpERR2, actOUT2,alpha,l1regular_);
		convERR2 = actlayer1_.updateWeight(actERR2, convOUT2);
		wordERR2 = conlayer_.updateWeight(convERR2, fakeSent,alpha,l1regular_);	 
		err2    +=  SVC::MEAN(SVC::MAP(ERR2, std::abs)) ;
		//UPDATE word vector
		SVC::saxpy(fakeSentP,alpha,wordERR2);
		SVC::saxpy(fakeSentP,-l1regular_);
		cot++;
		if(cot%cott==0){
			cot=0;
			std::cout << err1/cott<<" "<<err2/cott<< std::endl;	
			err1=0.0;err2=0.0;
			vector<std::pair<int, float>>dist;
			int tokensize = indexs2tokens_.size();
			dist.reserve(tokensize);
			int tokenidx = rand() % tokensize;
			string token= indexs2tokens_[tokenidx];
			 
			Vector &targVec = wordvec_[tokenidx];
			for (int idx = 0; idx<wordvec_.size(); idx++)
			{
				dist.push_back(std::move(std::make_pair(idx, SVC::normdot(targVec, wordvec_[idx]))));
			}
	
			std::sort(dist.begin(), dist.end(), [](std::pair<int, float>&v1, std::pair<int, float>&v2) {return v1.second > v2.second; });
			
			for (int i = 0; i < 10; i++) {
				if (tokenidx == dist[i].first)continue;
				printf("%s %d most similar word: %s \n",token.c_str(), i, indexs2tokens_[dist[i].first].c_str());
			}
			printf("---------------------------------------------------------------------------\n");

		}	
 
		if((err1+err2)/cot/2<0.51){return 1;}
		return 0;		 
	}


};

#endif /* snakeWord2Vec_h */
