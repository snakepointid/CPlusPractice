#pragma once
//
//  NeuronLayer.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/19.
//  Copyright © 2017年 佘昌略. All rights reserved.
//
#ifndef S_BASICLAYERS_H
#define S_BASICLAYERS_H
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include <algorithm>
#include"MatrComp/vecCompApi.h"
using std::vector;
using std::string;
using SVC::Vector;

struct basicLayer
{
	//controler
	int inputDim_ = 0;
	int outputDim_ = 0;
	//constructor
	basicLayer(int outputDim, int inputDim) :inputDim_(inputDim), outputDim_(outputDim) { initMatrix(); }
	//member
	///weights
	vector<Vector> Weights_;
	Vector         Bias_;
	///gradient holder
	vector<Vector> WeightsHolder_;
	Vector         BiasHolder_;
	//initial member : first parameter set 1 means random initial 0 means set to 0
	void initMatrix()
	{
		SVC::initialMatrix(1, Weights_, outputDim_, inputDim_);
		SVC::initialMatrix(1, Bias_, outputDim_);
		SVC::initialMatrix(0, WeightsHolder_, outputDim_, inputDim_);
		SVC::initialMatrix(0, BiasHolder_, outputDim_);
	}
	//funcs
	template<typename T>
	T actNeuron(const T &inputs)
	{
		return SVC::PairWiseADD(SVC::dot(inputs, Weights_), Bias_);
	}
	template<typename T>
	T saveGradient(const T &outputError, const T &inputs)
	{
		SVC::saxpy(WeightsHolder_, 1, SVC::dot(SVC::mtp(outputError), SVC::mtp(inputs)));
		SVC::saxpy(BiasHolder_, 1, SVC::SUM(outputError, 1));
		return SVC::dot(outputError, SVC::mtp(Weights_));
	}
	void updateWeight(float alpha, float l1)
	{
		SVC::saxpy(Weights_, alpha, WeightsHolder_);
		SVC::saxpy(Bias_, alpha, BiasHolder_);
		if (l1 > 0)
		{
			SVC::saxpy(Weights_, -l1);
			SVC::saxpy(Bias_, -l1);
		}
		//set holder to 0 again
		WeightsHolder_.clear();
		BiasHolder_.clear();
		SVC::initialMatrix(0, WeightsHolder_, outputDim_, inputDim_);
		SVC::initialMatrix(0, BiasHolder_, outputDim_);
	}
	template<typename T>
	T updateWeight(T &outputError, const T &inputs, float alpha, float l1)
	{
		T inputError;
		inputError.swap(SVC::dot(outputError, SVC::mtp(Weights_)));
		SVC::saxpy(Weights_, alpha, SVC::dot(SVC::mtp(outputError), SVC::mtp(inputs)));
		SVC::saxpy(Bias_, alpha, SVC::SUM(outputError, 1));
		if (l1 > 0)
		{
			SVC::saxpy(Weights_, -l1);
			SVC::saxpy(Bias_, -l1);
		}
		return inputError;
	}
 
};
 
struct actLayer
{
	typedef float(*ActiveP)(float f);
	typedef float(*GradtiveP)(float f);
	//controler
	string actor_;
	ActiveP active;
	GradtiveP gradtive;
	//constructor
	actLayer(const string&actor) :actor_(actor) { initialACT(active, gradtive, actor_); };
	//functions
	void initialACT(ActiveP&act, GradtiveP &gct, string&actor)
	{
		if (actor == "sigmoid") { act = SVC::speedSigmd; gct = SVC::speedSigmdGrad; }
		else if (actor == "tanh") { act = SVC::speedTanh; gct = SVC::speedTanhGrad; }
		else if (actor == "relu") { act = SVC::relu; gct = SVC::reluGrad; }
		else { printf("your act type is not exsit,eta.sigmoid,tanh,relu\n"); abort(); }
	}
	template<typename T>
	T actNeuron(const T &inputs)
	{
		return  SVC::MAP(inputs, active);
	}
	template<typename T>
	T updateWeight(const T &outputError, const T &inputs)
	{
		return  SVC::PairWiseMulti(outputError, SVC::MAP(inputs, gradtive));
	}
};
#endif