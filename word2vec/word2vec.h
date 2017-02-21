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
#include<map>
#include <chrono>
#include <stdio.h>
//#include<omp.h>
#include"vecComp.h"
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
	Vector           outWeight_;
	Vector           hiddenvec_;
	vector<Vector >  hiddenWeight_;
	int              total_words_ = 0;
	//speed up table
	Vector  sigTable;
	Vector tanhTable;
	//controler	
	float alpha_;
	float min_alpha_;
	int window_;
	int wordDim_;
	int hiddenDim_;
	int min_count_;
	int spanNum_;
	float maxExp_;
	float l1regular_;

	//construct funcs
	SW2V(float alpha = 0.1, float min_alpha = 0.001, int negSamples = 10, int window = 3, int wordDim = 100,
		int min_count = 1, float subsample = 0.001, int spanNum = 1000, float maxExp = 6.0, float l1regular = 0.0001)
		:alpha_(alpha), min_alpha_(min_alpha), window_(window), wordDim_(wordDim),
		min_count_(min_count), spanNum_(spanNum), maxExp_(maxExp), l1regular_(l1regular) {};
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
		hiddenWeight_.resize(hiddenDim_);
		outWeight_.resize(hiddenDim_);
		hiddenvec_.resize(hiddenDim_);
		//inital weights
		for (auto&w : wordvec_)
		{
			w.resize(wordDim_);
			for (auto&v : w)
			{
				v = (rng(eng) - 0.5) / wordDim_;
			}
		}
		for (auto&w : outWeight_)w = (rng(eng) - 0.5) / hiddenDim_;
		for (auto&hiddenvec : hiddenWeight_)
		{
			hiddenvec.resize(window_*wordDim_);
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

		//sigmoid value table
		sigTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::exp((i / spanNum_ * 2 - 1)*maxExp_);
			sigTable[i] = f / (f + 1);;
		}

		tanhTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::tanh((i / spanNum_ * 2 - 1)*maxExp_);
			tanhTable[i] = f;
		}
		return 1;
	}
	void trainCorpus()
	{
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);
		static int currentWordCount = 0;
		static  int trainedSentences = 0;
		static  int TrainedSentences = 0;
		float alpha;
#pragma omp parallel for
		for (int i = 0; i<sentences.size(); i++)
		{
			int trainWords = 0;
			auto &sentence = sentences[i];
			if (sentence->indexs.size()<3)continue;
			if (currentWordCount > total_words_)currentWordCount = total_words_;
			alpha = std::max(min_alpha_, alpha_*(float)(1 - currentWordCount / total_words_));
			trainedSentences += 1;
			trainWords = Ordertrain(sentence->indexs, alpha);
#pragma omp atomic
			currentWordCount += trainWords;
		}
		if (trainedSentences > 1000) { TrainedSentences += trainedSentences / 1000; trainedSentences = trainedSentences % 1000; }
		printf("total trained sentences: %luk\n", TrainedSentences);

	}
	void testWordSimilar()
	{
		wordvecNorm_ = wordvec_;
		for (auto&w : wordvecNorm_)sUnit(w);
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
				dist.push_back(std::move(std::make_pair(idx, sDot(targVec, wordvecNorm_[idx]))));
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
private:
	int Ordertrain(vector<int>&sentence, float alpha)
	{
		vector<int>rightVec;
		vector<int>fakeVec;
		rightVec.resize(window_);
		fakeVec.resize(window_);
		int sentLens = sentence.size();
		for (int iniCur = 0; iniCur < sentLens; iniCur++)
		{
			if (iniCur + window_>sentLens)break;
			int hidx = 0;
			for (auto&hiddenweigt : hiddenWeight_)
			{
				for (int windCur = 0; windCur<window_; windCur++)
				{
					int rightIdx = sentence[iniCur + windCur];
					int fakeIdx = sentence[iniCur + window_ - 1 - windCur];
					rightVec[windCur] = rightIdx;
					fakeVec[windCur] = fakeIdx;
					float f = sDot(rightVec, hiddenweigt);
					if (f < -maxExp_) { f = -1; }
					else if (f>maxExp_) { f = 1; }
					else {
						int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
						f = tanhTable[fi];
					}
					hiddenvec_[hidx++] = f;
				}
			}
			float f = sDot(outWeight_, hiddenvec_);
		}
		return sentLens;
	}


};

#endif /* snakeWord2Vec_h */
