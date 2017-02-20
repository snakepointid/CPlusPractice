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
using std::vector;
using Vector = vector<float>;
//dot
inline float sDot(const Vector &x, const Vector &y,const bool bias)
{
	float sum = 0.0;
	if (bias)
	{
		if (y.size() != x.size() | x.size() == 0) { printf("the dimension is not right!!!"); abort(); }		
	}
	else
	{
		if (y.size() != x.size()+1 | x.size() == 0) { printf("the dimension is not right!!!"); abort(); }
		sum += y.back();
	}
	
	int m = x.size(); const float *xp = x.data(), *yp = y.data();
	while (m-->0)
	{
		sum += (*xp++)*(*yp++);
	}
	return sum;
}
inline vector<Vector> sDot(const vector<Vector> &x, const vector<Vector> &y, const bool bias)
{
	if (y.size() == 0 | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
	vector<Vector> z(x.size());
	int xd = x.size(); const Vector *xp = x.data(); Vector *zp = z.data();
	while (xd-->0)
	{
		int yd = y.size(); const Vector *yp = y.data(); zp->resize(yd); float *zv = zp->data();
		while (yd-->0)
		{
			(*zv++) = sDot(*xp, *yp++, bias);
		}
		xp++; zp++;
	}
	return z;
}
inline Vector sDot(const Vector &x, const vector<Vector> &y, const bool bias)
{
	const vector<Vector>nx{ x };
	return *sDot(nx, y, bias).data();
}

inline vector<Vector> sDot(const vector<Vector> &x, const Vector &y, const bool bias)
{
	const vector<Vector>ny{ y };
	return sDot(x,ny, bias);
}
//outer
inline vector<Vector> sOuter(const Vector &x, const Vector &y)
{
	int xDim = x.size();
	vector<Vector>z(xDim);
	Vector *zp = z.data(); const float *xv = x.data();
	while (xDim-- > 0)
	{

		int yDim = y.size(); zp->resize(yDim); const float *yv = y.data(); float *zv = zp->data();
		while (yDim-- > 0)
		{
			(*zv++) = (*yv++)*(*xv);
		}
		xv++; zp++;
	}
	return z;
}
inline Vector sOuter(const Vector &x, const vector<Vector> &y)
{
	Vector z(y[0].size());
	int xDim = x.size(); const Vector *yp = y.data(); const float  *xv = x.data();
	while (xDim-- > 0)
	{
		int yDim = y[0].size(); const float *yv = yp->data(); float *zv = z.data();
		while (yDim-- > 0)
		{
			(*zv++) += (*yv++)*(*xv);
		}
		xv++; yp++;
	}
	return z;
}
inline vector<Vector> sOuter(const Vector &x, const Vector &y)
{
	int xDim = x.size();
	vector<Vector>z(xDim);
	Vector *zp = z.data(); const float *xv = x.data();
	while (xDim-- > 0)
	{

		int yDim = y.size(); zp->resize(yDim); const float *yv = y.data(); float *zv = zp->data();
		while (yDim-- > 0)
		{
			(*zv++) = (*yv++)*(*xv);
		}
		xv++; zp++;
	}
	return z;
}
inline Vector sOuter(const Vector &x, const vector<Vector> &y)
{
	Vector z(y[0].size());
	int xDim = x.size(); const Vector *yp = y.data(); const float  *xv = x.data();
	while (xDim-- > 0)
	{
		int yDim = y[0].size(); const float *yv = yp->data(); float *zv = z.data();
		while (yDim-- > 0)
		{
			(*zv++) += (*yv++)*(*xv);
		}
		xv++; yp++;
	}
	return z;
}
//convlution

inline float sConv(const vector<Vector> &x, const vector<Vector> &y)
{
	if (x.size() != y.size() | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data();
	float sum = 0.0;
	while (m-->0)
	{
		sum += sDot(*xp++, *yp++,0);
	}
	return sum;
}
//gradient update
///gradient pass always use the true flag -pred,and use the positive passing!!!!!!!
inline void sSaxpy(Vector &x, float g, const Vector &y)
{
	int m = x.size(); float *xp = x.data(); const float *yp = y.data();
	while (m-->0)(*xp++) += (*yp++)*g;
}
inline void sSaxpy(vector<Vector> &x, float g, const vector<Vector> &y)
{
	int m = x.size(); Vector *xp = x.data(); const Vector *yp = y.data();
	while (m-- > 0) { sSaxpy(*xp++, g, *yp++); }
}
inline void sSaxpy(Vector &x, float g, const Vector &y, float l1)
{
	int m = x.size(); float *xp = x.data(); const float *yp = y.data();
	while (m-->0) { (*xp) += ((*yp++)*g - (*xp)*l1); xp++; }
}
inline void sSaxpy(vector<Vector>  &x, float g, const vector<Vector>  &y, float l1)
{
	int m = x.size(); Vector *xp = x.data(); const Vector *yp = y.data();
	while (m-- > 0) { sSaxpy(*xp++, g, *yp++,l1); }
}
//pairwise multiplication
inline Vector sPairWiseMulti(const Vector &x, const Vector &y)
{
	Vector z(x.size());
	int m = x.size(); const float *xp = x.data(), *yp = y.data(); float *zp = z.data();
	while (m-- > 0) { (*zp++) = (*xp++)*(*yp++); }
	return z;
}
inline vector<Vector> sPairWiseMulti(vector<Vector> &x, vector<Vector> &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data(); Vector *zp = z.data();
	while (m-->0)
	{
		(*zp++) = sPairWiseMulti(*xp++, *yp++);
	}
	return z;
}
//pairwise multiplication
inline Vector sPairWiseADD(const Vector &x, const Vector &y)
{
	Vector z(x.size());
	int m = x.size(); const float *xp = x.data(), *yp = y.data(); float *zp = z.data();
	while (m-- > 0) { (*zp++) = (*xp++)+(*yp++); }
	return z;
}
inline vector<Vector> sPairWiseADD(vector<Vector> &x, vector<Vector> &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data(); Vector *zp = z.data();
	while (m-->0)
	{
		(*zp++) = sPairWiseADD(*xp++, *yp++);
	}
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

#endif /* vecComp_h */
