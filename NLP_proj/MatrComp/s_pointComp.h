//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_POINTCOMP_H
#define S_POINTCOMP_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>
namespace SVC
{
	using std::vector;
	using Vector = vector<float>;
 //sigmoid 
	float speedSigmd(const float f)
	{
		int spanNum_ = 1000;
		float maxExp_ = 6.0;
		const static Vector sigmoidTable = [&]()
		{	 
			Vector tmpTable(spanNum_);
			for (int i = 0; i < spanNum_; i++)
			{
				float fv = ::exp((float(i) / spanNum_ * 2 - 1)*maxExp_);
				fv = fv / (fv + 1);
				tmpTable[i] = fv;
			}
			return tmpTable;
		}();
		float rf;
		if (f <= -maxExp_) { rf = 0; }
		else if (f >= maxExp_) { rf = 1; }
		else
		{
			int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
			rf = sigmoidTable[fi];
	 
		}
		return rf;
	}
	//sigmoid gradient
	float speedSigmdGrad(const float f)
	{
		int spanNum_ = 1000;
		float maxExp_ = 6.0;
		const static Vector sigmdGradTable = [&]()
		{
			int spanNum_ = 1000;
			
			Vector tmpTable(spanNum_);
			for (int i = 0; i < spanNum_; i++)
			{
				float fv = ::exp((float(i) / spanNum_ * 2 - 1)*maxExp_);
				fv = fv / (fv + 1);
				tmpTable[i] = fv*(1 - fv);
			}
			return tmpTable;
		}();
		float rf;
		if (f <= -maxExp_) { rf = 0.0001; }
		else if (f >= maxExp_) { rf = 0.0001; }
		else
		{
			int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
			rf = sigmdGradTable[fi];
		}
		return rf;
		
	}
	//tanh
	float speedTanh(const float f)
	{
		int spanNum_ = 1000;
		float maxExp_ = 6.0;
		const static Vector tanhTable = [&]()
		{
			int spanNum_ = 1000;
			
			Vector tmpTable(spanNum_);
			for (int i = 0; i < spanNum_; i++)
			{
				float fv = ::tanh((float(i) / spanNum_ * 2 - 1)*maxExp_);
				tmpTable[i] = fv;
			}
			return tmpTable;
		}();
		float rf;
		if (f <= -maxExp_) { rf = -1; }
		else if (f >= maxExp_) { rf = 1; }
		else
		{
			int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
			rf = tanhTable[fi];
		}
		return rf;
	}
	//tanh gradient
	float speedTanhGrad(const float f)
	{
		int spanNum_ = 1000;
		float maxExp_ = 6.0;
		const static Vector tanhGradTable = [&]()
		{
			int spanNum_ = 1000;

			Vector tmpTable(spanNum_);
			for (int i = 0; i < spanNum_; i++)
			{
				float fv = ::tanh((float(i) / spanNum_ * 2 - 1)*maxExp_);
				tmpTable[i] = 1 - fv*fv;
			}
			return tmpTable;
		}();
		float rf;
		if (f <= -maxExp_) { rf = 0.0001; }
		else if (f >= maxExp_) { rf = 0.0001; }
		else
		{
			int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
			rf = tanhGradTable[fi];
		}
		return rf;
	}
	//relu
	float relu(const float f)
	{
		float rf=f;
		if (f <  0) { rf = 0; }
		if(f > 6.0) { rf = 6.0; }
		return rf;
	}
	//relu gradient
	float reluGrad(const float f)
	{
		float rf=1;
		if (f <  0) { rf = 0; }
		return rf;
	}
}

#endif /* vecComp_h */
