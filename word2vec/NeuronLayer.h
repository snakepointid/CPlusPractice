//
//  NeuronLayer.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/19.
//  Copyright © 2017年 佘昌略. All rights reserved.
//
#ifndef NeuronLayer_h
#define NeuronLayer_h
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include <algorithm>
#include"vecComp.h"
using std::vector;
using std::string;
template<typename inputType>
struct BasicLayer
{
    //random seed
    std::default_random_engine eng(::time(NULL));
	std::uniform_real_distribution<float> rng(0.0, 1.0);
    //controler
    int inputDim_;
    int outputDim_;
 	//constructor
	BasicLayer(int inputDim, int outputDim, string& Actor) :inputDim_(inputDim), outputDim_(outputDim)  {};
    //member
	///datas
	inputType *inputs_;
	///weights
	vector<Vector> Weights_;
	Vector         Bias_;
	///gradient holder
	vector<Vector> WeightsHolder_;
	Vector         BiasHolder_;
	inputType      inputErrorHolder_;
    //initial member
    Weights_.resize(outputDim_);
    WeightsHolder_.resize(outputDim_);
    for(auto&wt: Weights_) {wt.resize(inputDim_);for(auto&w:wt){w=(rng(eng) - 0.5) / inputDim_;};}
    for(auto&w:WeightsHolder_){w.resize(inputDim_);};
	BiasHolder_.resize(outputDim_);
	Bias_.resize(outputDim_);
    //funcs
	inputType& actNeuron(const inputType &inputs)
	{
		inputs_ = &inputs;
		return sPairWiseADD(sDot(inputs, Weights_), Bias_);
	}
	inputType &saveGradient(inputType &outputError)
    {               
		sSaxpy(WeightsHolder_, 1, sDot(sTp(outputError), sTp(*inputs_)));
		sSaxpy(BiasHolder_, 1, sSum(outputError,1));
        return sDot(outputError, sTp(Weights_));
    }
	void updateWeight(float alpha, float l1)
	{
		sSaxpy(Weights_, alpha, WeightsHolder_);
		sSaxpy(Bias_, alpha, BiasHolder_);
		if (l1 > 0)
		{
			sSaxpy(Weights_, -l1);
			sSaxpy(Bias_, -l1）;
		}
		WeightsHolder_.clear();
		Bias_.clear();
		WeightsHolder_.resize(outputDim_);
		for (auto&w : WeightsHolder_) { w.resize(inputDim_); };
		BiasHolder_.resize(outputDim_);
	}
	void updateWeight(inputType &outputError,float alpha, float l1)
	{
		sSaxpy(Weights_, alpha, sDot(sTp(outputError), sTp(*inputs_)));
		sSaxpy(Bias_, alpha, sSum(outputError, 1));
		if (l1 > 0)
		{
			sSaxpy(Weights_, -l1);
			sSaxpy(Bias_, -l1）;
		}	 
	}
};

template<typename inputType>
struct ActLayer
{
    string actor_;
    ActLayer(string&actor):actor_(actor){};
	float(*Active)(float f);
	float(*gradtive)(float f);
    switch(actor_)
    {
        case "sigmoid": Active = sigmoid; gradtive = sigmoidGrad; break;
		case "tanh"   : Active = tanh   ; gradtive = tanhGrad; break;
		case "relu"   : Active = relu   ; gradtive = reluGrad; break;
		default:printf("your actor type doesn't exist!!!\n'");abort(); break;
    };
    //members
	inputType gradient_(m);
    //funcs
	Vector& actNeuron(const Vector &inputs)
	{
		gradient_.clear();
		int m = inputs.size();
		inputType outputs_(m);
		gradient_.resize(m);
		std::transform(inputs.begin(), inputs.end(), outputs.begin(), Active);
		std::transform(outputs.begin(), outputs.end(), gradient_.begin(), gradtive);
		return outputs;
	}
	vector<Vector>& actNeuron(const vector<Vector> &inputs)
	{
		gradient_.clear();
		int m = inputs.size();
		inputType outputs_(m);
		gradient_.resize(m);
		const Vector *ip = inputs.data(); Vector *op = outputs.data(); Vector *gp = gradient_.data();
		while (m-- > 0)
		{
			std::transform(ip->begin(), ip->end(), op->begin(), Active);
			std::transform(op.begin(), op.end(), gp.begin(), gradtive);
			ip++; op++; gp++;
		}
		return outputs;
	}
    Vector updateWeight(Vector &outputError)
    {               
        return sPairWiseMulti(outputError,gradient_);
    }
     
    private:
    //inital speed table
        int spanNum_=1000;
        float maxExp_ = 6.0;
        Vector sigmoidTable;
        Vector tanhTable;
        sigmoidTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::exp((i / spanNum_ * 2 - 1)*maxExp_);
            f = f / (f + 1);
			sigmoidTable[i] = f
        };
        tanhTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::tanh((i / spanNum_ * 2 - 1)*maxExp_);
			tanhTable[i] = f;	
		};
    //act and gradient function
		float sigmoid(float f)
		{
			if (f <= -maxExp_) { actf = 0; }
			else if (f >= maxExp_) { actf = 1; }
			else
			{
				int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
				actf = sigmoidTable[fi];
			}
			return actf;
		}

		float tanh(float f)
		{
			if (f <= -maxExp_) { actf = -1; }
			else if (f >= maxExp_) { actf = 1; }
			else
			{
				int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
				actf = tanhTable[fi];
			}
			return actf;
		}
		float relu(float f)
		{
			if (f < 0) { actf = 0; }
			else if (f >  maxExp_) { actf = maxExp_; }
			else
			{		 
				actf = f;
			}
			return actf;
		}
		float sigmoidGrad(float f)
		{
			return f*（1-f);
		}

		float tanhGrad(float f)
		{
			return 1 - f*f;
		}
		float reluGrad(float f)
		{
			if (f > 0) { return 1; }
			else { return 0; } 
		}
		 
}
#endif /* NeuronLayer_h */
