//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef vecComp_h
#define vecComp_h
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>
namespace SVC
{
	using std::vector;
	using Vector = vector<float>;
	std::default_random_engine eng(::time(NULL));
	std::uniform_real_distribution<float> rng(0.0, 1.0);
	//dot
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
	//scale
	template<typename T>
	inline void scale(T &x, float g)
	{
		int m = x.size(); auto *xp = x.data();
		while (m-- > 0)scale(*xp++, g);
	}

	template<>inline void scale(float &x, float g)
	{
		x = x*g;
	}
	//gradient update
	///gradient pass always use the true flag -pred,and use the positive passing!!!!!!!
	template<typename T>
	inline void saxpy(T &x, float g, const T &y)
	{
		if (x.empty()) { x = y; scale(x, g); }
		else if (y.empty()) {}
		else
		{
			int m = x.size(); auto *xp = x.data(); const auto *yp = y.data();
			while (m-- > 0)saxpy(*xp++, g, *yp++);
		}

	}

	template<>inline void saxpy(float &x, float g, const float &y)
	{
		x += y*g;
	}

	template<typename T>
	inline void saxpy(T &x, float g)
	{
		int m = x.size(); auto *xp = x.data();
		while (m-- > 0)saxpy(*xp++, g);
	}

	template<>inline void saxpy(float &x, float g)
	{
		x += x*g;
	}
	//outer
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
	//convlution
	inline float mdot(const vector<Vector> &x, const vector<Vector> &y)
	{
		if (x.size() != y.size() | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
		int m = x.size(); const Vector *xp = x.data(), *yp = y.data();
		float sum = 0.0;
		while (m-->0)
		{
			sum += dot(*xp++, *yp++);
		}
		return sum;
	}
	inline Vector mdot(const vector<Vector> &x, const vector<vector<Vector>> &y)
	{
		int m = y.size();
		Vector z(m); float *zv = z.data(); const vector<Vector>*yp = y.data();
		while (m-- > 0)
		{
			*zv++ = mdot(x, *yp);
		}
		return z;
	}
	inline vector<Vector> mdot(const vector<vector<Vector>> &x, const vector<vector<Vector>> &y)
	{
		int m = x.size();
		vector<Vector> z(m); Vector *zp = z.data(); const vector<Vector> *xp = x.data();
		while (m-- > 0)
		{
			zp++->swap(mdot(*xp++, y));
		}
		return z;
	}
	inline vector<Vector> mouter(const float &x, const vector<Vector> &y)
	{
		vector<Vector>z = y;
		scale(z, x);
		return z;
	}
	inline vector<Vector> mouter(const Vector &x, const vector<vector<Vector>> &y)
	{
		int m = y.size();
		vector<Vector>z; const float *xv = x.data(); const vector<Vector>*yp = y.data();
		while (m-- > 0)
		{
			saxpy(z, 1, mouter(*xv++, *yp++));
		}
		return z;
	}
	inline vector<vector<Vector>> mouter(const Vector &x, const vector<Vector> &y)
	{
		int m = x.size();
		vector<vector<Vector>>z(m); const float *xv = x.data(); vector<Vector>*zp = z.data();
		while (m-- > 0)
		{
			zp++->swap(mouter(*xv++, y));
		}
		return z;
	}

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
		while (m-- > 0) { zp++->swap(PairWiseMulti(*xp++, *yp++)); }
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
		float norm = sqrt(dot(x, x));
		if (norm == 0) return;
		int m = x.size(); float *xp = x.data();
		while (m-->0)(*xp++) /= norm;
	}
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
	//show vector
	template<typename T>
	inline void showVec(const T & v)
	{
		int m = v.size(); const auto*vp = v.data();
		while (m-- > 0) { showVec(*vp++); std::cout << std::endl; }
	}
	template<>inline void showVec(const Vector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }

	}
	//matrixInitial
	template<typename T, typename... Args>
	void initialMatrix(const bool rnd, T &matrix, int i, Args... args)
	{
		matrix.resize(i);
		auto *matrixp = matrix.data();
		while (i-->0) { initialMatrix(rnd, *matrixp++, args...); }

	}
	template<typename T >
	T initialMatrix(const bool rnd, const T &matrix)
	{
		T nmatrix;
		vector<int>sp = Shape(matrix);
		initialMatrix(rnd, nmatrix, sp.data());
		return nmatrix;
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
	void initialMatrix(const bool rnd, Vector &matrix, int i)
	{
		matrix.resize(i);
		if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
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
}

#endif /* vecComp_h */
