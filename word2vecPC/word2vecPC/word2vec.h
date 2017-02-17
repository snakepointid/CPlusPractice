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
#include<omp.h>
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
	using SentenceP = std::shared_ptr<Sentence>;
	//members
	vector<SentenceP>sentences;
	std::map<string, int> tokens2indexs_;
	vector<string>   indexs2tokens_;
	vector<int>		 tokensCount_;
	vector<std::pair<string, int>> vocab_;
	vector<Vector>   inWeight_;
	vector<Vector>   inWeightNorm_;
	vector<Vector>   outWeight_;
	Vector           gradHolder_;
	int              total_words_=0;
	//speed up table
	vector<int>      unigram;
	vector<float>    sigTable;
	//controler	
	float alpha_;
	float min_alpha_;
	int negSamples_;
	int window_;
	int wordDim_;
	int min_count_;
	float subsample_;
	int spanNum_;
	float maxExp_;
	//construct funcs
	SW2V(float alpha = 0.1, float min_alpha = 0.001, int negSamples = 10, int window = 3, int wordDim = 100,
		int min_count = 1, float subsample = 0.001, int spanNum = 1000, float maxExp = 6.0) 
		:alpha_(alpha), min_alpha_(min_alpha), negSamples_(negSamples), window_(window), wordDim_(wordDim),
		min_count_(min_count), subsample_(subsample), spanNum_(spanNum), maxExp_(maxExp){};
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
			if (pr.second > min_count_) { index++; indexs2tokens_.push_back(move(pr.first)); tokensCount_.push_back(pr.second);}
			else { infreqWordCount += pr.second; }
			if(infreqWordCount>0){ indexs2tokens_.push_back("infreqWord"); tokensCount_.push_back(infreqWordCount); }
		}
		//shape those member
		n_words = indexs2tokens_.size();
		inWeight_.resize(n_words);
		outWeight_.resize(n_words);
		gradHolder_.resize(wordDim_);	
		//inital weights
		for (auto&w : inWeight_)
		{
			w.resize(wordDim_);
			for (auto&v : w)
			{
				v = (rng(eng) - 0.5) / wordDim_;
			}
		}
		for (auto&w : outWeight_)w.resize(wordDim_);
 
		//transfer the sentences
		for (auto&sentence : sentences)
		{
			if (sentence->tokens.empty())continue;
			for (auto&token : sentence->tokens)
			{
				sentence->indexs.push_back(tokens2indexs_[token]);
			}
		}	
		//negtive sample table
		unigram.resize(std::min((int)1e8, total_words_));
		int   unigramSize = unigram.size();
		float power = 0.75;
		float sum = std::accumulate(tokensCount.begin(), tokensCount.end(), 0, [&power](float x, const std::pair<string, int>& v) { return x + ::pow(v.second,power); });
		float d1 = ::pow(tokensCount_[0], power) / sum;
		int idx = 0;
		for (int a = 0; a < unigramSize; a++)
		{
			if (a / unigramSize > d1) { idx++; d1 += ::pow(tokensCount_[idx], power) / sum;}
			unigram[a] = idx;
			if (idx > n_words - 1)idx = n_words - 1;
		}
		printf("unigram size:%lu;with power sum:%.4f\n", unigram.size(), sum);
		//sigmoid value table
		sigTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::exp(i / spanNum_ * 2 - 1);
			sigTable[i] = f / (f + 1);;			
		}
		return 1;
	}
	void trainCorpus()
	{
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);
		static int currentWordCount = 0;
		static unsigned int trainedSentences = 0;
		static unsigned int TrainedSentences = 0;
		float alpha;
#pragma omp parallel for
		for (int i = 0; i<sentences.size(); i++)
		{
			int trainWords = 0;
			auto &sentence = sentences[i];
			if (sentence->indexs.size()<3)continue;
			if (currentWordCount > total_words_)currentWordCount = total_words_;
			alpha = std::max(min_alpha_, alpha_*(float)(1 - currentWordCount / total_words_));
			//subsample
			vector<int>tmpSentence;
			for (auto &idx : sentence->indexs)
			{
				if (subsample_ > 0)
				{
					float rnd = (::sqrt(tokensCount_[idx] / total_words_ / subsample_) + 1)*(subsample_*total_words_ / tokensCount_[idx]);
					if (rnd > rng(eng))tmpSentence.push_back(idx);
				}
				else
				{
					tmpSentence.push_back(idx);
				}			
			}
			if (tmpSentence.size() > 1) {
				trainedSentences += 1; trainWords = trainSentence(tmpSentence, alpha);
			}
#pragma omp atomic
			currentWordCount +=  trainWords;
		}		
		if (trainedSentences > 1000) { trainedSentences -= 1000; TrainedSentences += 1; }
		printf("total trained sentences: %luk|alpha: %.4f\n", TrainedSentences, alpha);

	}
	void testWordSimilar()
	{
		inWeightNorm_ = inWeight_;
		for (auto&w : inWeightNorm_)sUnit(w);
		string token;
		vector<std::pair<int, float>>dist;
		dist.reserve(indexs2tokens_.size());
		while(1)
		{
			printf("please enter the word you want to test('#quit' to quit):");
			std::cin >> token;
			if (token=="#quit")break;
			printf("the word you want to test is' %s '\n",token.c_str());
			lowerTokens(token);
			auto it = tokens2indexs_.find(token.c_str());
			if (it == tokens2indexs_.end()) { printf("word is not exist!!!\n"); continue; }
			Vector &targVec = inWeightNorm_[it->second];
		 
			for (int idx=0;idx<inWeightNorm_.size();idx++)
			{
				dist.push_back(std::move(std::make_pair(idx, sDot(targVec, inWeightNorm_[idx]))));
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
		fin << inWeight_.size() << " " << inWeight_[0].size() << "\n";
		for (int idx = 0; idx < indexs2tokens_.size(); idx++)
		{
			fin << indexs2tokens_[idx] << " ";
			for (auto&w : inWeight_[idx]) { fin << w << " "; }
			fin << "\n";
		}
		fin.close();
	}
	int loadWord2vec(string&fn)
	{
		inWeight_.clear();
		std::ifstream fin;
		string line;
		string token;
		fin.open(fn);
		if (!getline(fin, line)) { std::cout << "there have no data\n"; return -1; }
		int n_words = 0, wordDim = 0;	
		std::stringstream(line) >> n_words >> wordDim;
		inWeight_.resize(n_words);
		for (auto&w : inWeight_)w.resize(wordDim);
		int idx = 0;
		float wt;
		while (getline(fin, line))
		{
			std::stringstream ipt(line);
			ipt >> token;
			
			int widx = 0;
			while (ipt >> wt)
			{
				inWeight_[idx][widx++] = wt;
			}
			tokens2indexs_[token] = idx++;
			indexs2tokens_.push_back(move(token));
		}
		fin.close();
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
	int trainSentence(vector<int>&sentence, float alpha)
	{
		int sentLens = sentence.size();
		for (int targCur = 0; targCur < sentLens; targCur++)
		{
			int targTokenIdex = sentence[targCur];
			int reduced_window = rand() % window_;
			int lowCur = std::max(0, targCur - reduced_window - 1);
			int upCur = std::min(sentLens, targCur + reduced_window + 1);
			Vector&targWeight = inWeight_[targTokenIdex];
			for (; lowCur<upCur; lowCur++)
			{
				std::fill(gradHolder_.begin(), gradHolder_.end(), 0.0);
				if (lowCur == targCur)continue;
				int contTokenIdex = sentence[lowCur];
				Vector&contWeight = outWeight_[contTokenIdex];
				float f = sDot(targWeight, contWeight);
				if (f <= -maxExp_ || f >= maxExp_)
					continue;
				int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
				f = sigTable[fi];
				float g = (1 - f) * alpha;
				sSaxpy(contWeight, g, targWeight);
				sSaxpy(gradHolder_, g, contWeight);
				for (int neg = 0; neg < negSamples_; neg++)
				{
					int negTokenIdex = unigram[rand() % unigram.size()];
					if (negTokenIdex == targTokenIdex || negTokenIdex == contTokenIdex)continue;
					Vector&negWeight = outWeight_[negTokenIdex];
					float f = sDot(targWeight, negWeight);
					if (f <= -maxExp_ || f >= maxExp_)
						continue;
					int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
					f = sigTable[fi];
					float g = (0 - f) * alpha;
					sSaxpy(negWeight, g, targWeight);
					sSaxpy(gradHolder_, g, negWeight);
				}
				sSaxpy(targWeight, 1.0, gradHolder_);	 
			}
		}
		return sentLens;
	}
	
};

#endif /* snakeWord2Vec_h */
