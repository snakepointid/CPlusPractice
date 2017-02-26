//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_AGGREGATE_H
#define S_AGGREGATE_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>
#include"s_saxpy.h"
namespace SVC
{
	//scale
	using std::vector;
	using Vector = vector<float>;
	//sum
	template<typename T >
	inline float SUM(const T &x)
	{
		float sumr = 0.0; int m = x.size(); const auto *xp = x.data();
		while (m-->0)
		{
			sumr += SUM(*xp++);
		}
		return sumr;
	}

	template<>inline float SUM(const Vector &x)
	{
		float sumr = 0.0; int m = x.size(); const float *xv = x.data();
		while (m-->0)
		{
			sumr += *xv++;
		}
		return sumr;
	}
	inline Vector SUM(const vector<Vector> &x, int dim)
	{
		Vector z;
		if (dim == 1)
		{
			int m = x.size(); const Vector *xp = x.data();
			while (m-- > 0) { saxpy(z, 1, *xp++); }
		}
		else
		{
			int m = x.size(); z.resize(m); float *zv = z.data(); const Vector *xp = x.data();
			while (m-- > 0) { *zv++ = SUM(*xp++); }
		}
		return z;
	}
	inline Vector SUM(const Vector &x, int dim)
	{
		return x;
	}
	//sum
	template<typename T >
	inline float MEAN(const T &x)
	{
		float sumr = 0.0; int m = x.size(); const auto *xp = x.data();
		while (m-->0)
		{
			sumr += MEAN(*xp++);
		}
		return sumr/x.size();
	}
	template<>inline float MEAN(const Vector &x)
	{
		float sumr = 0.0; int m = x.size(); const float *xv = x.data();
		while (m-->0)
		{
			sumr += *xv++;
		}
		return sumr/x.size();
	}
}

#endif /* vecComp_h */
