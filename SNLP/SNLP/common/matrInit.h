#ifndef MATRINIT_H_
#define MATRINIT_H_
#include"meta.h"
#include<ctime>
#include<iostream>
#include<random>
 
namespace SNLP {

	static std::default_random_engine eng(::time(NULL));
	static std::uniform_real_distribution<float> rng(0.0, 1.0);
	//matrix transpose
	vector<Fvector> mtp(const vector<Fvector> &x);	
	vector<Fvector> mtp(const Fvector &x);	
	vector<Fvector> mtp(const float &x);
	//reset matrix
	template<typename T>
	inline void resMatr( T &matrix )
	{
		auto *subMatr = matrix.data();
		int m = matrix.size();
		while (m-- > 0) {
			resMatr(*subMatr++);
		}
	}
	template<> inline void resMatr( Fvector &matrix)
	{
		std::fill(matrix.begin(), matrix.end(), 0);
	}
	//get matrix shape
	template<typename T>
	inline void Shape(const T &matrix, Ivector&sp)
	{
		const auto *nm = matrix.data(); sp.push_back(matrix.size());
		Shape(*nm, sp);
	}
	template<typename T>
	inline vector<int> Shape(const T &matrix)
	{
		Ivector sp;
		Shape(matrix, sp);
		return sp;
	}
	template< > inline void Shape(const Fvector &matrix, Ivector&sp)
	{
		sp.push_back(matrix.size());
	}
	//show vector
	template<typename T>
	inline void showVec(const T & v)
	{
		int m = v.size(); const auto*vp = v.data();
		while (m-- > 0) { showVec(*vp++); }
		std::cout << std::endl;
	}
	template< >inline void showVec(const Fvector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
	template< >inline void showVec(const Ivector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
	//matrixInitial
	inline void initialMatrix(const bool rnd, Fvector &matrix, int i)
	{
		matrix.resize(i);
		if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
	}
	template<typename T >
	inline T initialMatrix(const bool rnd, const T &matrix)
	{
		T nmatrix;
		vector<int>sp = Shape(matrix);
		initialMatrix(rnd, nmatrix, sp.data());
		return nmatrix;
	}
	template<typename T, typename... Args>
	inline void initialMatrix(const bool rnd, T &matrix, int i, Args... args)
	{
		matrix.resize(i);
		auto *matrixp = matrix.data();
		while (i-->0) { initialMatrix(rnd, *matrixp++, args...); }

	}

	template<typename T >
	inline void initialMatrix(const bool rnd, T &matrix, int *dim)
	{
		int i = *dim;
		matrix.resize(i);
		auto *matrixp = matrix.data(); dim++;
		while (i-->0) { initialMatrix(rnd, *matrixp++, dim); }
	}
	template< >inline void initialMatrix(const bool rnd, Fvector &matrix, int *dim)
	{
		int i = *dim;
		matrix.resize(i);
		if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
	}
}
#endif // !MATRINIT_H_
