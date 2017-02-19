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
#include"vecComp.h"
using std::vector;
using std::string;
struct BasicLayer
{
    //random seed
    std::default_random_engine eng(::time(NULL));
	std::uniform_real_distribution<float> rng(0.0, 1.0);
    //controler
    int inputDim_;
    int outputDim_;
    string Actor_;
    //member
    vector<Vector > Weights_;
    Vector     Bias_;
    Vector  outputs_;
    Vector  *inputs_;
    Vector inputError_;
    vector<Vector>  WeightsHolder_;
    Vector          BiasHolder_;
    //constructor
    BasicLayer(int inputDim,int outputDim,string& Actor)
    :inputDim_(inputDim),outputDim_(outputDim),Actor_(Actor){};
    //initial member
    outputs_.resize(outputDim_);
    Weights_.resize(outputDim_);
    inputError_.resize(inputDim_);
    WeightsHolder_.resize(outputDim_);
    Bias_.resize(outputDim_);
    BiasHolder_.resize(outputDim_);
    for(auto&wt:Weights_) {wt.resize(inputDim_);for(auto&w:wt){w=(rng(eng) - 0.5) / inputDim_;};}
    for(auto&b:Bias_){b=(rng(eng) - 0.5) / inputDim_;};
    for(auto&w:WeightsHolder_){w.resize(inputDim_);};
    //inital Actor
    //funcs
    Vector& actNeuron(Vector &inputs)
    {
        inputs_=&inputs;
        for(int outidx=0;outidx<outputDim_;outidx++)
        {
                outputs_[outidx]=sDot(inputs,Weights_[outidx])+Bias_[outidx];
        }
        return outputs_;
    }
    Vector &updateWeight(Vector &outputError)
    {               
        fill(inputError_.begin(),inputError_.end(),0.0);
        for(int outidx=0;outidx<outputDim_;outidx++)
        {
            float error = outputError[outidx];
            sSaxpy(WeightsHolder_[outidx],error,*inputs_);
            BiasHolder_[outidx]+=error;
            sSaxpy(inputError_,error,Weights_[outidx]);
        }  
        return inputError_;
    }
    Vector &updateWeight(float error)
    {               
        fill(inputError_.begin(),inputError_.end(),0.0);       
        sSaxpy(WeightsHolder_[0],error,*inputs_);
        BiasHolder_[0]+=error;
        sSaxpy(inputError_,error,Weights_[0]);      
        return inputError_;
    }
    void updateWeight(float alpha,float l1)
    {                     
         for(int outidx=0;outidx<outputDim_;outidx++)
        {
            sSaxpy(Weights_[outidx],-alpha,WeightsHolder_[outidx],l1);
            Bias_[outidx]-=(alpha*BiasHolder_[outidx]+Bias_[outidx]*l1);
            BiasHolder_[outidx]=0.0;
            fill(Weights_[outidx].begin(),Weights_[outidx].end(),0.0);
        }  
    }
};
struct ActLayer
{
    string actor_;
    ActLayer(string&actor):actor_(actor){};
    float (*Active)(float f);
    switch(actor_)
    {
        case "sigmoid": Active = sigmoid;break;
		case "tanh":  Active = tanh;break;
        default:throw "your actor type doesn't exist!!!\n'";break;
    };
    //members
    Vector outputs_;
    Vector gradient_;
    vector<Vector> outputs_2d_;
    vector<Vector> gradient_2d_;
    
    //funcs
    Vector& actNeuron(Vector &inputs)
    {
        int outDim = inputs.size();
        outputs_.resize(outputs_);
        gradient_.resize(outputs_);
        for(int outidx=0;outidx<outDim;outidx++)
        {
             Active(inputs[outidx]);
             outputs_[outidx]=outAndGrad[0];
             gradient_[outidx]=outAndGrad[1];
        }
        return outputs_;
    }
    Vector updateWeight(Vector &outputError)
    {               
        return sPairWiseMulti(outputError,gradient_);
    }
    vector<Vector>& actNeuron(vector<Vector> &inputs)
    {
        int outDim1 = inputs.size();
        int outDim2 = inputs[0].size();
        outputs_2d_.resize(outDim1);
        gradient_2d_.resize(outDim1);
        for(int outidx1=0;outidx1<outDim1;outidx1++)
        {
            outputs_2d_[outidx1].resize(outDim2);
            gradient_2d_[outidx1].resize(outDim2);
            for(int outidx2=0;outidx2<outDim2;outidx2++)
            {
             Active(inputs[outidx1][outidx2]);
             outputs_2d_[outidx1][outidx2] =outAndGrad[0];
             gradient_2d_[outidx1][outidx2]=outAndGrad[1];
            }
        }
        return outputs_2d_;
    }
    vector<Vector> updateWeight(vector<Vector>&outputError)
    {               
        return sPairWiseMulti(outputError,gradient_2d_);
    }
    private:
    //inital speed table
        Vector outAndGrad(2);
        int spanNum_=1000;
        float maxExp_ = 6.0;
        Vector sigmoidTable;
        Vector tanhTable;
        Vector sigmoidGradTable;
        Vector tanhGradTable;
        sigmoidTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::exp((i / spanNum_ * 2 - 1)*maxExp_);
            f = f / (f + 1);
			sigmoidTable[i] = f
            sigmoidGradTable[i] = f*(1-f);	
        };
        tanhTable.resize(spanNum_);
		for (int i = 0; i < spanNum_; i++)
		{
			float f = ::tanh((i / spanNum_ * 2 - 1)*maxExp_);
			tanhTable[i] = f;	
            sigmoidGradTable[i] = 1-f*f;		
		};
    //act function
        void sigmoid(float f)
        {   
            if(f<= -maxExp_){actf=0;}
            else if(f>= maxExp_){actf=1;}
            else
            {
                int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
                outAndGrad[0] = sigmoidTable[fi];
                outAndGrad[1] = sigmoidGradTable[fi];
            }      
        }
        void tanh(float f)
        {
            if(f<= -maxExp_){actf=-1;}
            else if(f>= maxExp_){actf=1;}
            else
            {
                int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
                outAndGrad[0] = tanhTable[fi];
                outAndGrad[1] = tanhGradTable[fi];
            }      
        }
}
#endif /* NeuronLayer_h */
