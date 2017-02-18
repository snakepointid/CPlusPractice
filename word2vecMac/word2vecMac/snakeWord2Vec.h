//
//  snakeWord2Vec.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef snakeWord2Vec_h
#define snakeWord2Vec_h
 
#include "vecComp.h"
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
#include <chrono>	
using std::string;
struct SW2V
{
    struct Word
    {       
        string text;
        uint32_t count;
        Vector inWeight;
        Vector outWeight;

        Word(const Word&w)=delete;
        const Word& operator=(const Word&w) = delete;
    };
    //create a inter pointer to handle word obj
    using WordP = std::shared_ptr<Word>;
    //create a word dict to hold word obj
    std::unordered_map<string, WordP> vocab_;
    struct Sentence
    {
        vector<Word*> words;
        vector<string> tokens;
    };
    //basic variable
    bool phrase_;
    using SentenceP = std::shared_ptr<Sentence>;
    void build_vocab(vector<SentenceP>&sentences)
    {
        int sentCount;
        std::unordered_map<string,int>vocab;
        for(auto&sentence:sentences)
        {
            sentCount+=1;
            if(sentence->tokens.empty())continue;
            string last_word;
            for(auto&word:sentence->tokens)
            {
                vocab[word]+=1;
                if(phrase_)
                {
                    if(!last_word.empty())
                    {
                        string phrase = last_word+" "+word;
                        vocab[phrase]+=1;
                    }
                    last_word = word;
                }
            }
        };
        int total_words = std::accumulate(vocab.begin(), vocab.end(), 0, [&](int x, const std::pair<string, int>& v) { return x + v.second; });
        printf("handle %lu sentences, total %lu tokens with %lu distinct words",sentCount);

    }
};

#endif /* snakeWord2Vec_h */
