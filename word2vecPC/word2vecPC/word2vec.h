﻿//
//  snakeWord2Vec.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//
#include <vector>
#include <list>
#include <string>
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
	vector<Vector>   inWeight;
	vector<Vector>   outWeight;
	Vector           gradHolder;
	//speed up table
	vector<int>unigram;
	vector<float>sigTable;
	int spanNum = 1000;
	float maxExp = 6.0;
	//controler
	bool phrase_ = true;
	int min_token_count_ = 0;
	float alpha_ = 0.025;
	float min_alpha_ = 0.0001;
	float subsample_ = 0;

	int negSamples_ = 5;
	int contextWindow = 5;
	int total_words;
	int wordDim = 100;
	int processCorpus()
	{
		auto cstart = std::chrono::high_resolution_clock::now();
		string last_token;
		std::unordered_map<string, int>tokensCount;
		//count the token numbers
		for (auto &sentence : sentences)
		{
			if (sentence->tokens.empty())continue;
			for (auto &token : sentence->tokens)
			{
				tokensCount[token] += 1;
				if (phrase_)
				{
					if (!last_token.empty()) { tokensCount[last_token + "_" + token] += 1; }
				}
				last_token = token;
			}
		}
		total_words = std::accumulate(tokensCount.begin(), tokensCount.begin(), 0, [](int x, const std::pair<string, int>&v) {return x + v.second; });
		//select phrase base simple statistic
		 
		if (phrase_)
		{
			std::unordered_map<string, int>phraseCount;
			for (auto &sentence : sentences)
			{
				vector<string>phraseTokens;
				if (sentence->tokens.empty())continue;
				for (auto &token : sentence->tokens)
				{
					if (!last_token.empty())
					{
						string phrase = last_token + "_" + token;
						int pa = tokensCount[last_token], pb = tokensCount[token], pab = tokensCount[phrase];
						float score = 0.0;
						if (pab > min_token_count_)
						{
							score = (pab - min_token_count_) / ((float)pa*pb)*total_words;
							if (score > 100)
							{
								phraseCount[phrase] += 1;
								token.clear();
								phraseTokens.push_back(move(phrase));
							}
							else if (pa>min_token_count_)
							{
								phraseCount[last_token] += 1;
								phraseTokens.push_back(move(last_token));
							}
						}
					}
					last_token = token;
					if (!last_token.empty() && tokensCount[last_token]>min_token_count_)
					{
						phraseCount[last_token] += 1;
						phraseTokens.push_back(move(last_token));
					}
				}
				int len = phraseTokens.size();
				sentence->tokens.swap(phraseTokens);
				sentence->indexs.reserve(len);
			}
			tokensCount.swap(phraseCount);
		}
		auto cend = std::chrono::high_resolution_clock::now();
		printf("process tokens: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);

		total_words = std::accumulate(tokensCount.begin(), tokensCount.end(), 0, [](int x, const std::pair<string, int>& v) { return x + v.second; });
		std::cout << "total words:" << total_words << std::endl;
		if (total_words < 1)return -1;
		int n_words = tokensCount.size();
		cstart = std::chrono::high_resolution_clock::now();
		//initial weights
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);
		inWeight.resize(n_words);
		outWeight.resize(n_words);
		indexs2tokens_.reserve(n_words);
		tokensCount_.reserve(n_words);
		vocab_.reserve(n_words);
		gradHolder.resize(wordDim);
		for (auto&w : inWeight)
		{
			w.resize(wordDim);
			for (auto&v : w)
			{
				v = (rng(eng) - 0.5) / wordDim;
			}
		}
		for (auto&w : outWeight)
		{
			w.resize(wordDim);
		}
		cend = std::chrono::high_resolution_clock::now();
		printf("initial weights: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
		cstart = std::chrono::high_resolution_clock::now();
		//initial members
		for (auto&pr : tokensCount)
		{
			vocab_.push_back(move(make_pair(pr.first, pr.second)));
		}
		auto comp = [](std::pair<string, int>&v1, std::pair<string, int>&v2) {return v1.second > v2.second; };
		std::sort(vocab_.begin(), vocab_.end(), comp);
		int index = 0;
		for (auto&pr : vocab_)
		{
			tokens2indexs_[pr.first] = index++;
			indexs2tokens_.push_back(move(pr.first));
			tokensCount_.push_back(pr.second);
		}
		for (auto&sentence : sentences)
		{
			if (sentence->tokens.size()<3)continue;
			for (auto&token : sentence->tokens)
			{
				sentence->indexs.push_back(tokens2indexs_[token]);
			}
		}
		cend = std::chrono::high_resolution_clock::now();
		printf("initial members: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
		//negtive sample table
		cstart = std::chrono::high_resolution_clock::now();
		unigram.resize(std::min((int)1e8, total_words));
		int uniSize = unigram.size();
		float power = 0.75;
		float sum = std::accumulate(tokensCount.begin(), tokensCount.end(), 0, [&power](float x, const std::pair<string, int>& v) { return x + ::pow(v.second,power); });
		float d1 = ::pow(tokensCount_[0], power) / sum;
		int idx = 0;
		for (int a = 0; a < uniSize; a++)
		{
			if (a / uniSize > d1) { idx++; d1 += ::pow(tokensCount_[idx], power) / sum;}
			unigram[a] = idx;
			if (idx > n_words - 1)idx = n_words - 1;
		}
		printf("unigram size:%lu;with sum:%.4f\n", unigram.size(), sum);
		cend = std::chrono::high_resolution_clock::now();
		printf("table speed up: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);
		//sigmoid value table
		sigTable.resize(spanNum);
		for (int i = 0; i < spanNum; i++)
		{
			float f = ::exp(i / spanNum * 2 - 1);
			sigTable[i] = f / (f + 1);;			
		}
		std::cout << "haha to the final\n";
		return 1;
	}
	void trainCorpus()
	{
		std::default_random_engine eng(::time(NULL));
		std::uniform_real_distribution<float> rng(0.0, 1.0);

		int currentWordCount = 0;
		int trainedSentences = 0;
#pragma omp parallel for
		for (int i = 0; i<sentences.size(); i++)
		{
			int trainWords = 0;
			auto &sentence = sentences[i];
			if (sentence->indexs.empty())continue;
			
			float alpha = std::max(min_alpha_, alpha_*(1 - currentWordCount / total_words));
			//subsample
			vector<int>tmpSentence;
			for (auto &idx : sentence->indexs)
			{
				float rnd = (::sqrt(tokensCount_[idx] / total_words / subsample_) + 1)*(subsample_*total_words / tokensCount_[idx]);
				if (rnd > rng(eng)| subsample_==0)tmpSentence.push_back(idx);
			};
			if (tmpSentence.size() > 1) {
				trainedSentences += 1; trainWords = trainSentence(tmpSentence, alpha);
			}
#pragma omp atomic
			currentWordCount += trainWords;
		}
		std::cout << "prepare trained sentences:" << trainedSentences << "\n";
		std::cout << "total trained words:" << currentWordCount << "\n";
	}
	void loadCorpus()
	{
		string fn("test.txt");
		std::ifstream fin;
	/*	std::cout << "please enter your raw corpus file name:";
		std::cin >> fn;*/
		SentenceP sentence(new Sentence);
		string line;
		string token;
		fin.open(fn);
		while (std::getline(fin, line))
		{
			if (line.empty())continue;
			std::stringstream str(line);
			while (str >> token)
			{
				sentence->tokens.push_back(move(token));
			}
			sentences.push_back(std::move(sentence));
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
			int reduced_window = rand() % contextWindow;
			int lowCur = std::max(0, targCur - reduced_window - 1);
			int upCur = std::min(sentLens, targCur + reduced_window + 1);
			Vector&targWeight = inWeight[targTokenIdex];
			for (; lowCur<upCur; lowCur++)
			{
				std::fill(gradHolder.begin(), gradHolder.end(), 0);
				if (lowCur == targCur)continue;
				int contTokenIdex = sentence[lowCur];
				Vector&contWeight = inWeight[contTokenIdex];
				float f = sDot(targWeight, contWeight);
				if (f <= -maxExp || f >= maxExp)
					continue;
				int fi = int((f + maxExp) * (spanNum / maxExp / 2.0));
				f = sigTable[fi];
				float g = (1 - f) * alpha;
				sSaxpy(contWeight, g, targWeight);
				sSaxpy(gradHolder, g, contWeight);
				for (int neg = 0; neg < negSamples_; neg++)
				{
					int negTokenIdex = unigram[rand() % unigram.size()];
					if (negTokenIdex == targTokenIdex || negTokenIdex == contTokenIdex)continue;
					Vector&contWeight = inWeight[negTokenIdex];
					float f = sDot(targWeight, contWeight);
					if (f <= -maxExp || f >= maxExp)
						continue;
					int fi = int((f + maxExp) * (spanNum / maxExp / 2.0));
					f = sigTable[fi];
					float g = (0 - f) * alpha;
					sSaxpy(contWeight, g, targWeight);
					sSaxpy(gradHolder, g, contWeight);
				}
				sSaxpy(targWeight, 1.0, gradHolder);
				 
			}
		}
		return sentLens;
	}

};

#endif /* snakeWord2Vec_h */
