//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_PAIRWISECOMP_H
#define S_PAIRWISECOMP_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>
namespace SVC
{
	//scale
	using std::vector;
	using Vector = vector<float>;
	//gradient update
	///gradient pass always use the true flag -pred,and use the positive passing!!!!!!!
	//pairwise multiplication
	template<typename T >
	inline T PairWiseMulti(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(PairWiseMulti(*xp++, *yp++)); }
		return z;
	}
	template<>inline Vector PairWiseMulti(const Vector &x, const Vector &y)
	{
		int m = x.size();
		Vector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { *zv++ = (*xv++)*(*yv++); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseMulti(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { zp++->swap(PairWiseMulti(x, *yp++)); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseMulti(const vector<T> &x, const T&y)
	{
		return return PairWiseMulti(y, x);
	}
	//pairwise add
	template<typename T >
	inline T PairWiseADD(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(PairWiseADD(*xp++, *yp++)); }
		return z;
	}
	template<>inline Vector PairWiseADD(const Vector &x, const Vector &y)
	{
		int m = x.size();
		Vector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { *zv++ = (*xv++) + (*yv++); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseADD(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { zp++->swap(PairWiseADD(x, *yp++)); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseADD(const vector<T> &x, const T&y)
	{
		return  PairWiseADD(y, x);
	}

	//pairwise add
	template<typename T >
	inline T PairWiseMinus(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(PairWiseMinus(*xp++, *yp++)); }
		return z;
	}
	template<>inline Vector PairWiseMinus(const Vector &x, const Vector &y)
	{
		int m = x.size();
		Vector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { *zv++ = (*xv++) - (*yv++); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseMinus(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { zp++->swap(PairWiseMulti(x, *yp++)); }
		return z;
	}
	template<typename T >
	inline  vector<T> PairWiseMinus(const vector<T> &x, const T&y)
	{
		return  PairWiseMinus(y, x);
	}
}

#endif /* vecComp_h */
