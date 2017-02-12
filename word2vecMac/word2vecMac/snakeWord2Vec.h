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
#include <unordered_map>
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
    using WordP = std::shared_ptr<Word>;
    std::unordered_map<string, WordP> vocab_;
    struct Sentence
    {
        vector<Word*> words;
        vector<string*> tokens;
    };
    using SentenceP = std::shared_ptr<Sentence>;

 


};

#endif /* snakeWord2Vec_h */
