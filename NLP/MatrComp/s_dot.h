//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_DOT_H
#define S_DOT_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>
#include<cmath>
#include "s_aggregate.h"
#include "s_vecMaping.h"
#include "s_saxpy.h"
namespace SVC
{
	using std::vector;
	using Vector = vector<float>;
	 
	 
	inline float dot(const Vector &x, const Vector &y)
	{
		float sum = 0.0;
		if (y.size() != x.size() | x.size() == 0) { printf("the dimension is not right!!!"); abort(); }
		int m = x.size(); const float *xp = x.data(), *yp = y.data();
		while (m-->0)
		{
			sum += (*xp++)*(*yp++);
		}
		return sum;
	}

	inline vector<Vector> dot(const vector<Vector> &x, const vector<Vector> &y)
	{
		vector<Vector> z(x.size());
		int xDim = x.size(); const Vector *xp = x.data(); Vector *zp = z.data();
		while (xDim-->0)
		{
			int yDim = y.size(); const Vector *yp = y.data(); zp->resize(yDim); float *zv = zp->data();
			while (yDim-->0)
			{
				(*zv++) = dot(*xp, *yp++);
			}
			xp++; zp++;
		}
		return z;
	}
	 
	inline Vector dot(const Vector &x, const vector<Vector> &y)
	{
		const vector<Vector>nx{ x };
		return *dot(nx, y).data();
	}

	inline vector<Vector> dot(const vector<Vector> &x, const Vector &y)
	{
		const vector<Vector>ny{ y };
		return dot(x, ny);
	}
	 
	//normalize
	inline void norml2(Vector &x)
	{
		float norm = ::sqrt(dot(x, x));
		if (norm == 0) return;
		int m = x.size(); float *xp = x.data();
		while (m-->0)(*xp++) /= norm;
	}
	inline void norml1(Vector &x)
	{
        float norm = SUM(MAP(x,std::abs));
		if (norm == 0) return;
		int m = x.size(); float *xp = x.data();
		while (m-->0)(*xp++) /= norm;
	}
	inline float normdot(const Vector &x, const Vector &y)
	{
		 Vector xn = x;
		 Vector yn = y;
		 norml1(xn);
		 norml1(yn);
		 return dot(xn,yn); 
	}
}

#endif  
