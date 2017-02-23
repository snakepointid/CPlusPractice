#pragma once
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
#include"V.h"
using std::vector;
using std::string;
using SVC::Vector;
template<typename inputType>
struct BasicLayer
{
	//controler
	int inputDim_ = 0;
	int outputDim_ = 0;
	//constructor
	BasicLayer(int outputDim, int inputDim) :inputDim_(inputDim), outputDim_(outputDim) { initMatrix(); }
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
	inputType actNeuron(const inputType &inputs)
	{
		return SVC::PairWiseADD(SVC::dot(inputs, Weights_), Bias_);
	}
	inputType saveGradient(inputType &outputError, const inputType &inputs)
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
	inputType updateWeight(inputType &outputError, const inputType &inputs, float alpha, float l1)
	{
		inputType inputError;
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
	Vector actNeuron(const vector<Vector> &inputs)
	{
		return SVC::PairWiseADD(SVC::mdot(inputs, Weights_), Bias_);
	}
	vector<Vector>  saveGradient(Vector &outputError, const vector<Vector> &inputs)
	{
		SVC::saxpy(WeightsHolder_, 1, SVC::mouter(outputError, inputs));
		SVC::saxpy(BiasHolder_, 1, SVC::SUM(outputError, 1));
		return SVC::mouter(outputError, Weights_);
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
	vector<Vector> updateWeight(Vector &outputError, vector<Vector> inputs, float alpha, float l1)
	{
		vector<Vector> inputError;
		inputError.swap(SVC::mouter(outputError, Weights_));
		SVC::saxpy(Weights_, alpha, SVC::mouter(outputError, inputs));
		SVC::saxpy(Bias_, alpha, SVC::SUM(outputError, 1));
		if (l1 > 0)
		{
			SVC::saxpy(Weights_, -l1);
			SVC::saxpy(Bias_, -l1);
		}
		return inputError;
	}
};

template<typename inputType>
struct ActLayer
{
	string actor_;
	ActLayer(string&actor) :actor_(actor) {};
	float(*Active)(float f);
	float(*gradtive)(float f);
	switch (actor_)
	{
	case "sigmoid": Active = sigmoid; gradtive = sigmGrad; break;
	case "tanh": Active = tanh; gradtive = tanhGrad; break;
	case "relu": Active = relu; gradtive = reluGrad; break;
	default:printf("your actor type doesn't exist!!!\n'"); abort(); break;
	};
	//members
	inputType gradient_(m);
	//funcs
	Vector& actNeuron(const Vector &inputs)
	{
		actNeros
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
		return sPairWiseMulti(outputError, gradient_);
	}

private:
	//inital speed table
	int spanNum_ = 1000;
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
	float sigmGrad(float f)
	{
		return f*（1 - f);
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
	template<typename T>
	T actNeros(const T &x)
	{
		int m = x.size(); T z(m); const auto*xp = x.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(actNeros(*xp++)); }
		return z;
	}
	vector actNeros(const vector &ip)
	{
		vector op(x.size());

		std::transform(ip.begin(), ip.end(), op.begin(), Active);

		return op;
	}
	template<typename T>
	T gradNeros(const T &x)
	{
		int m = x.size(); T z(m); const auto*xp = x.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(gradNeros(*xp++)); }
		return z;
	}
	vector gradNeros(const vector &ip)
	{
		vector op(x.size());
		std::transform(op.begin(), op.end(), gp.begin(), gradtive);
		return op;
	}
};
#endif