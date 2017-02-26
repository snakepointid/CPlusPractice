//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_MATRIXINITIAL_H
#define S_MATRIXINITIAL_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<ctime>
#include<random>
namespace SVC
{
	//scale
	using std::vector;
	using Vector = vector<float>;
	using std::vector;
	using Vector = vector<float>;
	std::default_random_engine eng(::time(NULL));
	std::uniform_real_distribution<float> rng(0.0, 1.0);
	//matrix transpose
	inline vector<Vector> mtp(const vector<Vector> &x)
	{
		int yRdim = x[0].size(), yCdim = x.size();
		vector<Vector> y(yRdim); Vector *yp = y.data();
		int xcol = 0;
		while (yRdim-- > 0)
		{
			const  Vector *xp = x.data();
			yCdim = x.size();
			yp->resize(yCdim);
			float *yv = yp++->data();
			while (yCdim-- > 0)
			{
				const float *xv = xp++->data() + xcol;
				*yv++ = *xv;
			}
			xcol++;
		}
		return y;
	}
	inline vector<Vector> mtp(const Vector &x)
	{
		vector<Vector>nx{ x };
		return mtp(nx);

	}
	inline vector<Vector> mtp(const float &x)
	{
		Vector nx{ x };
		return mtp(nx);

	}
    //get matrix shape
    template<typename T>
    inline void Shape(const T &matrix, vector<int>&sp)
    {
        const auto *nm = matrix.data(); sp.push_back(matrix.size());
        Shape(*nm, sp);
    }
    template<typename T>
    inline vector<int> Shape(const T &matrix)
    {
        vector<int>sp;
        Shape(matrix, sp);
        return sp;
    }
    template<>inline void Shape(const Vector &matrix, vector<int>&sp)
    {
        sp.push_back(matrix.size());
    }
	//show vector
	template<typename T>
	inline void showVec(const T & v)
	{
		int m = v.size(); const auto*vp = v.data();
		while (m-- > 0) { showVec(*vp++);}
		 std::cout << std::endl; 
		 

	}
	template<>inline void showVec(const Vector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		 std::cout << std::endl; 
	}
	template<>inline void showVec(const vector<int>& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		 std::cout << std::endl; 
	}
	//matrixInitial
   
    void initialMatrix(const bool rnd, Vector &matrix, int i)
    {
        matrix.resize(i);
        if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
    }
    template<typename T >
    T initialMatrix(const bool rnd, const T &matrix)
    {
        T nmatrix;
        vector<int>sp = Shape(matrix);
        initialMatrix(rnd, nmatrix, sp.data());
        return nmatrix;
    }
	template<typename T, typename... Args>
	void initialMatrix(const bool rnd, T &matrix, int i, Args... args)
	{
		matrix.resize(i);
		auto *matrixp = matrix.data();
		while (i-->0) { initialMatrix(rnd, *matrixp++, args...); }

	}
	
	template<typename T >
	void initialMatrix(const bool rnd, T &matrix, int *dim)
	{
		int i = *dim;
		matrix.resize(i);
		auto *matrixp = matrix.data(); dim++;
		while (i-->0) { initialMatrix(rnd, *matrixp++, dim); }
	}
    template<>void initialMatrix(const bool rnd, Vector &matrix, int *dim)
    {
        int i = *dim;
        matrix.resize(i);
        if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
    }
	
}

#endif /* vecComp_h */
