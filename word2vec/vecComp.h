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
//convlution
inline float sMdot(const vector<Vector> &x, const vector<Vector> &y)
{
	if (x.size() != y.size() | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data();
	float sum = 0.0;
	while (m-->0)
	{
		sum += sDot(*xp++, *yp++ );
	}
	return sum;
}
//gradient update
///gradient pass always use the true flag -pred,and use the positive passing!!!!!!!
inline void sSaxpy(Vector &x, float g, const Vector &y)
{
	int m = x.size(); float *xv = x.data(); const float *yp = y.data();
	while (m-->0)(*xv++) += (*yp++)*g;
}
inline void sSaxpy(vector<Vector> &x, float g, const vector<Vector> &y)
{
	int m = x.size(); Vector *xp = x.data(); const Vector *yp = y.data();
	while (m-- > 0) { sSaxpy(*xp++, g, *yp++); }
}
 
inline void sSaxpy(Vector &x, float g)
{
	int m = x.size(); float *xv = x.data();
	while (m-- > 0) { (*xv) += g*(*xv); xv++; }
}
inline void sSaxpy(vector<Vector> &x, float g)
{
	int m = x.size(); Vector *xp = x.data();
	while (m-- > 0) sSaxpy(*xp++, g);
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
		(*zp++).swap(sPairWiseMulti(*xp++, *yp++));
	}
	return z;
}
inline vector<Vector> sPairWiseMulti(const vector<Vector> &x, const Vector &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(); Vector *zp = z.data();
	while (m-- > 0) { (*zp++).swap(sPairWiseMulti(*xp++, y)); }
	return z;
}
inline vector<Vector> sPairWiseMulti(const Vector &x, const  vector<Vector> &y)
{
	return sPairWiseMulti(y, x);
}
//pairwise add
inline Vector sPairWiseADD(const Vector &x, const Vector &y)
{
	Vector z(x.size());
	int m = x.size(); const float *xp = x.data(), *yp = y.data(); float *zp = z.data();
	while (m-- > 0) { (*zp++) = (*xp++) + (*yp++); }
	return z;
}

inline vector<Vector> sPairWiseADD(vector<Vector> &x, vector<Vector> &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data(); Vector *zp = z.data();
	while (m-->0)
	{
		(*zp++).swap(sPairWiseADD(*xp++, *yp++));
	}
	return z;
}
inline vector<Vector> sPairWiseADD(const vector<Vector> &x, const Vector &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(); Vector *zp = z.data();
	while (m-- > 0) { (*zp++).swap(sPairWiseADD(*xp++, y)); }
	return z;
}
inline vector<Vector> sPairWiseADD(const Vector &x, const  vector<Vector> &y)
{
	return sPairWiseADD(y, x);
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
inline Vector sSum(const vector<Vector> &x,int dim)
{
	Vector z;
	if (dim == 0)
	{
		int m = x.size();z.resize(m) ; float *zv = z.data(); const Vector *xp = x.data();
		while (m-- > 0) { *zv++ = sSum(*xp++); }
	}
	else if (dim == 1)
	{
		int m = x.size(); z.resize(x[0].size());const Vector *xp = x.data();
		while (m-- > 0) { sSaxpy(z, 1, *xp++);}
	}
	else
	{
		printf("there are no such dimension\n"); abort();
	}
	return z;
}
//show vector
inline void showVec(vector<Vector>& v)
{
	int m = v.size(); const Vector *vp = v.data();
	while (m-- > 0)
	{
		int n = vp->size(); const float *vv = vp->data();
		while (n-- > 0)
		{
			cout << *vv << " ";
			vv++;
		}
		cout << "\n";
		vp++;
	}
}
inline void showVec(Vector& v)
{
	vector<Vector>nv{ v };
	showVec(nv);
}
#endif /* vecComp_h */
