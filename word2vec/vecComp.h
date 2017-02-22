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
#include<random>
using std::vector;
using Vector = vector<float>;
std::default_random_engine eng(::time(NULL));
std::uniform_real_distribution<float> rng(0.0, 1.0);
//dot
inline float sDot(const Vector &x, const Vector &y)
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
 
inline vector<Vector> sDot(const vector<Vector> &x, const vector<Vector> &y )
{
	vector<Vector> z(x.size());
	int xDim = x.size(); const Vector *xp = x.data(); Vector *zp = z.data();
	while (xDim-->0)
	{
		int yDim = y.size(); const Vector *yp = y.data(); zp->resize(yDim); float *zv = zp->data();
		while (yDim-->0)
		{
			(*zv++) = sDot(*xp, *yp++ );
		}
		xp++; zp++;
	}
	return z;
}
inline Vector sDot(const Vector &x, const vector<Vector> &y )
{
	const vector<Vector>nx{ x };
	return *sDot(nx, y ).data();
}

inline vector<Vector> sDot(const vector<Vector> &x, const Vector &y )
{
	const vector<Vector>ny{ y };
	return sDot(x,ny );
}
//outer
inline vector<Vector> sTp(const vector<Vector> &x)
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
inline vector<Vector> sTp(const Vector &x)
{
	vector<Vector>nx{ x };
	return sTp(nx);

}
inline vector<Vector> sTp(const float &x)
{
	Vector nx{ x };
	return sTp(nx);

}
//convlution
inline float sMdot(const vector<Vector> &x, const vector<Vector> &y)
{
	if (x.size() != y.size() | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data();
	float sum = 0.0;
	while (m-->0)
	{
		sum += sDot(*xp++, *yp++);
	}
	return sum;
}
inline Vector sMdot(const vector<Vector> &x, const vector<vector<Vector>> &y)
{
	int m = y.size();
	Vector z(m); float *zv = z.data(); const vector<Vector>*yp = y.data();
	while (m-- > 0)
	{
		*zv++ = sMdot(x, *yp);
	}
	return z;
}
inline vector<Vector> sMdot(const vector<vector<Vector>> &x, const vector<vector<Vector>> &y)
{
	int m = x.size();
	vector<Vector> z(m); Vector *zp = z.data(); const vector<Vector> *xp = x.data();
	while (m-- > 0)
	{
		zp++->swap(sMdot(*xp++, y));
	}
	return z;
}
inline vector<Vector> sMdotBP(const float &x, const vector<Vector> &y)
{
	int m = y.size();
	vector<Vector>z(m); Vector *zp = z.data(); const Vector *yp = y.data();
	while (m-- > 0)
	{
		int n = yp->size(); zp->resize(n); float *zv = zp++->data(); const float *yv = yp++->data();
		while (n-- > 0)
		{
			*zv++ = x*(*yv++);
		}
	}
	return z;
}
inline vector<Vector> sMdotBP(const Vector &x, const vector<vector<Vector>> &y)
{
	int m = y.size();
	vector<Vector>z(m); const float *xv = x.data(); const vector<Vector>*yp=y.data();
	z.swap(sMdotBP(*xv++, *yp++));
	while (m-- > 1)
	{
		sSaxpy(z, 1, sMdotBP(*xv++, *yp++));
	}
	return z;
}
inline vector<vector<Vector>> sMdotBP(const Vector &x, const vector<Vector> &y)
{
	int m = x.size();
	vector<vector<Vector>>z(m); const float *xv=x.data(); vector<Vector>*zp = z.data();
	while (m-- > 0)
	{
		zp++->swap(sMdotBP(*xv++, y));
	}
	return z;
}
//gradient update
///gradient pass always use the true flag -pred,and use the positive passing!!!!!!!
template<typename T>
inline void sSaxpy(T &x, float g, const T &y)
{
	int m = x.size(); auto *xp = x.data(); const auto *yp = y.data();
	while (m-- > 0)sSaxpy(*xp++, g, *yp++);
}
inline void sSaxpy(float &x, float g, const float &y)
{
	x += y*g;
}

template<typename T>
inline void sSaxpy(T &x, float g)
{
	int m = x.size(); auto *xp = x.data();
	while (m-- > 0)sSaxpy(*xp++, g);
}
inline void sSaxpy(float &x, float g)
{
	x += x*g;
}
//pairwise multiplication
template<typename T >
inline T sPairWiseMulti(const T &x, const T &y)
{
	T z(x.size());
	int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
	while (m-- > 0) { *zp++.swap(sPairWiseMulti(*xp++, *yp++)); }
	return z;
}
inline Vector sPairWiseMulti(const Vector &x, const Vector &y)
{
	int m = x.size();
	Vector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
	while (m-- > 0) { *zv++ = (*xv++)*(*yv++); }
	return z;
}
//pairwise add
template<typename T >
inline T sPairWiseADD(const T &x, const T &y)
{
	T z(x.size());
	int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
	while (m-- > 0) { *zp++.swap(sPairWiseMulti(*xp++, *yp++)); }
	return z;
}
inline Vector sPairWiseADD(const Vector &x, const Vector &y)
{
	int m = x.size();
	Vector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
	while (m-- > 0) { *zv++ = (*xv++)+(*yv++); }
	return z;
}
//normalize
inline void sUnit(Vector &x)
{
	float norm = ::sqrt(sDot(x, x));
	if (norm == 0) return;
	int m = x.size(); float *xp = x.data();
	while (m-->0)(*xp++) /= norm;

}
//sum
inline float sSum(const Vector &x)
{
	float sum = 0.0;
	int m = x.size(); const float *xp = x.data();
	while (m-->0)sum += *xp++;
	return sum;

}
inline float sSum(const vector<Vector> &x)
{
	float sum = 0.0;
	int m = x.size(); const Vector *xp = x.data();
	while (m-- > 0) { sum += sSum(*xp++); }
	return sum;

}
inline Vector sSum(const vector<Vector> &x, int dim)
{
	Vector z;
	if (dim == 0)
	{
		int m = x.size(); z.resize(m); float *zv = z.data(); const Vector *xp = x.data();
		while (m-- > 0) { *zv++ = sSum(*xp++); }
	}
	else if (dim == 1)
	{
		int m = x.size(); z.resize(x[0].size()); const Vector *xp = x.data();
		while (m-- > 0) { sSaxpy(z, 1, *xp++); }
	}
	else
	{
		printf("there are no such dimension\n"); abort();
	}
	return z;
}
inline Vector sSum(const Vector &x, int dim)
{
	return x;
}
//show vector
template<typename T>
inline void showVec(const T & v)
{
	int m = v.size(); const auto*vp = v.data();
	while (m-- > 0) { showVec(*vp++); cout << endl;}
}
inline void showVec(const Vector& v)
{
	for (auto&vv : v) { cout << vv << " "; }
	
}
//matrixInitial
template<typename T, typename... Args>
void initialMatrix(const bool rnd, T &matrix, int i, Args... args)
{
	matrix.resize(i);
	auto *matrixp = matrix.data();
	while (i-->0) { initialMatrix(rnd, *matrixp++, args...); }

}
void initialMatrix(const bool rnd, Vector &matrix, int i)
{
	matrix.resize(i);
	if (rnd) { for (auto&w : matrix) { w = (rng(eng) - 0.5) / i; }; }
}
#endif /* vecComp_h */
