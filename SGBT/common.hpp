#ifndef SGBT_COMMON_H_
#define SGBT_COMMON_H_
#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<cmath>
#include<random>
#include<ctime>
#include"meta.h"
using std::vector;
using std::string;

namespace SGBT {
	std::default_random_engine eng(::time(NULL));
	std::uniform_real_distribution<float> rng(0.0, 1.0);
	std::vector<std::string> splits(const std::string & s, char dim, std::vector<std::string>& elements)
	{
		{
			std::stringstream ss(s);
			std::string item;
			while (getline(ss, item, dim))
			{
				if (item.empty())break;
				elements.push_back(move(item));
			}
			return elements;
		}
	}

	std::vector<std::string> splits(const std::string & s, char dim)
	{
		std::vector<std::string> elements;
		return splits(s, dim, elements);
	}
	inline Dvector ATOF(vector<string>& svec)
	{
		int m = svec.size();
		Dvector dvec(m); float *v = dvec.data(); string *s = svec.data();
		while (m-- > 0) {
			*v = atof(s->c_str()); v++; s++;
		}
		return dvec;
	}

	void initialMatrix(const bool rnd, Dvector &matrix, int i)
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
	template<>void initialMatrix(const bool rnd, Dvector &matrix, int *dim)
	{
		int i = *dim;
		matrix.resize(i);
		if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
	}

	inline Dvector ATOF(const std::string & s, char dim)
	{
		Dvector elements;
		std::stringstream ss(s);
		std::string item;
		while (getline(ss, item, dim))
		{
			if (item.empty())break;
			elements.push_back(atof(item.c_str()));
		}
		return elements;
	}

	template<typename T>
	inline void showVec(const T & v)
	{
		int m = v.size(); const auto*vp = v.data();
		while (m-- > 0) { showVec(*vp++); }
		std::cout << std::endl;


	}
	template<>inline void showVec(const Dvector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
	template<>inline void showVec(const Ivector & v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
}
#endif // !SGBT_COMMON_H_
