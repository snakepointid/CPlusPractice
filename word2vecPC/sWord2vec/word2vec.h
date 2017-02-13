//
//  snakeWord2Vec.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef snakeWord2Vec_h
#define snakeWord2Vec_h
#include"vecComp.h"
#include<string>
#include<iostream>
#include<fstream>
#include<random>
#include<ctime>
#include <numeric>
#include<memory>
#include <algorithm>
#include <unordered_map>
using std::string;
struct SW2V
{
	//two basic data struct--word and sentences
	struct Word
	{
		uint32_t index_;
		string text_;
		uint32_t count_;
 
		Word(uint32_t index, string text, uint32_t count):index_(index), text_(text), count_(count){}
		Word(const Word&w) = delete;
		const Word& operator=(const Word&w) = delete;
	};
	using WordP = std::shared_ptr<Word>;
	
	struct Sentence
	{
		vector<Word*> words;
		vector<string> tokens;
	};
	using SentenceP = std::shared_ptr<Sentence>;
	////hyperparameter
	//bool phrase_;
	//unsigned int min_count_;
	////basic variable
	//unsigned int total_words;
	//vector<Vector> inWeight, outWeight;
	//std::unordered_map<string, WordP> vocab_;
	//vector<Word *>words_;
	//int wordDim;
	//vector<int>negSampTable;
	//int build_vocab(const vector<SentenceP> &sentences)
	//{
	//	unsigned int sentenCount = 0;
	//	std::unordered_map<string, int>vocab;
	//	auto progress = [&sentenCount](){
	//		printf("now handle %s sentences", sentenCount);
	//	};
	//	for (auto &sentence : sentences)
	//	{
	//		if (sentence->tokens.empty())continue;			
	//		sentenCount += 1;
	//		string last_token;
	//		for (auto &token : sentence->tokens)
	//		{
	//			vocab[token] += 1;
	//			if (phrase_)
	//			{
	//				if (!last_token.empty()) { vocab[last_token + "_" + token] += 1; };
	//			}
	//			last_token = token;
	//		}
	//		if (sentenCount % 10000 == 0)progress();
	//	}
	//	progress();
	//	total_words = std::accumulate(vocab.begin(), vocab.end(), 0, [](int x, std::pair<string, int>&v) {return x + v.second; });
	//	printf("\n total words:%lu, distinct words:%lu", total_words, vocab.size());
	//	//if need handle phrase
	//	if (phrase_)
	//	{
	//		std::unordered_map<string, int>phrase_vocab;
	//		for (auto &sentence : sentences)
	//		{				
	//			if (sentence->tokens.empty())continue;
	//			vector<string> phrase_tokens;				
	//			string last_token;
	//			for (auto &token : sentence->tokens)
	//			{
	//				if (!last_token.empty())
	//				{
	//					string phrase = last_token + "_" + token;
	//					int pa = vocab[last_token], pb = vocab[token], pab = vocab[phrase];
	//					float score = 0;
	//					if(pab>min_count_){  score = (pab - min_count_) / (float(pa) * pb) * total_words; }					
	//					if (score > 100) { phrase_tokens.push_back(phrase); phrase_vocab[phrase] += 1; token.clear(); }
	//					else if(pa>min_count_){ phrase_tokens.push_back(last_token); phrase_vocab[last_token] += 1; }
	//				}
	//				last_token = token;	
	//			}
	//			if(!last_token.empty()&& vocab[last_token]>min_count_){ phrase_tokens.push_back(last_token); phrase_vocab[last_token] += 1; }
	//			sentence->tokens.swap(phrase_tokens);
	//		}
	//		vocab.swap(phrase_vocab);
	//	}
	//	//update the vocab_ and words_
	//	unsigned int disctWordNum = vocab.size();
	//	if (disctWordNum < 10)return -1;
	//	for (auto &wp : vocab)
	//	{
	//		auto r = vocab_.emplace((wp.first, WordP(new Word(0, wp.first, wp.second))));
	//		words_.push_back(r.first->second.get());
	//	}
	//	auto comp = [](const Word&w1, const Word&w2) {return w1.count_ > w2.count_; };
	//	std::sort(words_.begin(), words_.end(), comp);
	//	unsigned int index = 0;
	//	for (auto &wp : words_) { wp->index_ = index++; }
	//	//intial the wordvec
	//	int n_words = words_.size();
	//	inWeight.reserve(n_words);
	//	outWeight.reserve(n_words);

	//	std::default_random_engine eng(::time(NULL));
	//	std::uniform_real_distribution<float> rng(0.0, 1.0);

	//	for (auto&wt : inWeight)
	//	{
	//		wt.reserve(wordDim);
	//		for (auto&wv : wt) { wv = (rng(eng) - 0.5) / wordDim; }
	//	}
	//	for (auto&wt : outWeight)
	//	{
	//		wt.reserve(wordDim);
	//		for (auto&wv : wt) { wv = (rng(eng) - 0.5) / wordDim; }
	//	}
	//	//get negtive sample table
	//	float power = 0.75;
	//	negSampTable.reserve(1e8);
	//	float sum = std::accumulate(words_.begin(), words_.end(), 0, [&power](float x, const Word *w) {return x + ::pow(w->count_, power); });
	//	float d1 = ::pow(words_[0]->count_, power) / sum;
	//	int idx = 0;
	//	int tableSize = negSampTable.size();
	//	for (int a = 0; a < tableSize; a++)
	//	{
	//		negSampTable[a] = idx;
	//		if (float(a) / tableSize > d1) { idx++;d1+= ::pow(words_[idx]->count_, power) / sum;}
	//		if (idx > n_words - 1) { idx = n_words - 1; }
	//	}
	//	return 0;
	//};
	//void readSentence(vector<SentenceP>&sentences)
	//{
	//	string fn;
	//	std::cout << "please enter your raw data file name: ";
	//	std::cin >> fn;
	//	std::ifstream fin;
	//	fin.open(fn);
	//	string line;
	//	while (std::getline(fin,line))
	//	{

	//	}
	//}
};

#endif /* snakeWord2Vec_h */
