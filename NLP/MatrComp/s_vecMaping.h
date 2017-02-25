//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_VECMAPING_H
#define S_VECMAPING_H
#include<vector>
#include <cstdlib>
#include<iostream>
 
#include<random>
namespace SVC
{
	using std::vector;
	using Vector = vector<float>;

	//maping
	typedef float(*fps)(const float);
	template<typename T >
	inline T MAP(const T &x, fps fp)
	{
		int m = x.size(); T z(m); const auto *xp = x.data(); auto *zp = z.data();
		while (m-->0)
		{
			zp++->swap(MAP(*xp++, fp));
		}
		return z;
	}
	template<>inline Vector MAP(const Vector &x, fps fp)
	{
		int m = x.size(); Vector z(m); const float *xv = x.data(); float *zv = z.data();
		while (m-->0)
		{
			(*zv++) = fp(*xv++);
		}
		return z;
	}
	typedef float(*fpss)(const float, float);
	template<typename T >
	inline T MAP(const T &x, fpss fp, float g)
	{
		int m = x.size(); T z(m); const auto *xp = x.data(); auto *zp = z.data();
		while (m-->0)
		{
			zp++->swap(MAP(*xp++, fp, g));
		}
		return z;
	}
	template<>inline Vector MAP(const Vector &x, fpss fp, float g)
	{
		int m = x.size(); Vector z(m); const float *xv = x.data(); float *zv = z.data();
		while (m-->0)
		{
			(*zv++) = fp(*xv++, g);
		}
		return z;
	}
}

#endif /* vecComp_h */
