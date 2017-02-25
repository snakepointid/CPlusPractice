#pragma once
//
//  NeuronLayer.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/19.
//  Copyright © 2017年 佘昌略. All rights reserved.
//
#ifndef S_CONVOLAYERS_H
#define S_CONVOLAYERS_H
#include<vector>
#include<string>
#include"vecCompApi.h"
using std::vector;
using std::string;
using SVC::Vector;
struct convoLayer
{
	//controler
	int inputDim_;
	int outputDim_;
	int window_;
	//constructor
	convoLayer(int outputDim, int window, int inputDim) :inputDim_(inputDim), outputDim_(outputDim), window_(window) { initMatrix(); }
	//member
	///weights
	vector<vector<Vector>> Weights_;
	Vector				   Bias_;
	///gradient holder
	vector<vector<Vector>> WeightsHolder_;
	Vector                 BiasHolder_;
	//funcs
	void initMatrix()
	{
		SVC::initialMatrix(1, Weights_, outputDim_, window_, inputDim_);
		SVC::initialMatrix(1, Bias_, outputDim_);
		SVC::initialMatrix(0, WeightsHolder_, outputDim_, window_, inputDim_);
		SVC::initialMatrix(0, BiasHolder_, outputDim_);
	}
	vector<Vector> actNeuron(const vector<Vector> &inputs)
	{
		return SVC::PairWiseADD(SVC::mdot(inputs, Weights_, window_), Bias_);
	}
	vector<Vector>  saveGradient(vector<Vector> &outputError, const vector<Vector> &inputs)
	{
		SVC::saxpy(WeightsHolder_, 1, SVC::mouter(outputError, inputs,window_));
		SVC::saxpy(BiasHolder_, 1, SVC::SUM(outputError, 1));
		return SVC::mouter(outputError, Weights_,window_);
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
		WeightsHolder_.clear();
		BiasHolder_.clear();
		SVC::initialMatrix(0, WeightsHolder_, outputDim_, window_, inputDim_);
		SVC::initialMatrix(0, BiasHolder_, outputDim_);
	}
	vector<Vector> updateWeight(vector<Vector> &outputError, const vector<Vector> inputs, float alpha, float l1)
	{
		vector<Vector> inputError;
		inputError.swap(SVC::mouter(outputError, Weights_, window_));	 
		SVC::saxpy(Weights_, alpha, SVC::mouter(outputError, inputs, window_));
		SVC::saxpy(Bias_, alpha, SVC::SUM(outputError, 1));
		if (l1 > 0)
		{
			SVC::saxpy(Weights_, -l1);
			SVC::saxpy(Bias_, -l1);
		}
		return inputError;
	}
	void setWeights(vector<vector<Vector>>&w, Vector&b)
	{
		Weights_.swap(w);
		Bias_.swap(b);
	}
};
 
#endif