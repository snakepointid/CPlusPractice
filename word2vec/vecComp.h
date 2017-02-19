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
	int m = x.size(); const float *xp = x.data(), *yp = y.data();
	float sum = 0.0;
	while (m-->0)
	{
		sum += (*xp++)*(*yp++);
	}
	return sum;
}
inline float sDot(const vector<Vector> &x, const vector<Vector> &y)
{
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data();
	float sum = 0.0;
	while (m-->0)
	{
		sum+=sDot(*xp++,*yp++);	
	}
	return sum;
}
inline vector<Vector> sDot(const vector<Vector> &x, const vector<Vector> &y,int dim)
{
	vector<Vector> z(x.size());
	int xd = x.size(); const Vector *xp = x.data();Vector *zp = z.data();
	while (xd-->0)
	{
		 int yd=y.size();const Vector *yp = y.data();zp->resize(yd);float *zv = zp->data(); 
		 while(yd-->0)
		 {
			(*zv++)=sDot(*xp,*yp++);	
		 }
		 xp++;zp++;
	}
	return z;
}
//gradient update
inline void sSaxpy(Vector &x, float g, const Vector &y)
{
	int m = x.size(); float *xp = x.data(); const float *yp = y.data();
	while (m-->0)(*xp++) += (*yp++)*g;
}
inline void sSaxpy(Vector &x, float g, const Vector &y,float l)
{
	int m = x.size(); float *xp = x.data(); const float *yp = y.data();
	while (m-->0){(*xp) += ((*yp++)*g-(*xp)*l);xp++;}
}
//pairwise multiplication
inline Vector sPairWiseMulti(const Vector &x,const Vector &y)
{
	Vector z(x.size());
	int m = x.size(); const float *xp = x.data(), *yp = y.data();float *zp = z.data();
	while (m-->0){(*zp++) = (*xp++)*(*yp++);
	return z;
}
inline vector<Vector> sPairWiseMulti(vector<Vector> &x,vector<Vector> &y)
{
	vector<Vector> z(x.size());
	int m = x.size(); const Vector *xp = x.data(), *yp = y.data();Vector *zp = z.data();
	while (m-->0)
	{
		 (*z++)=sPairWiseMulti(*xp++,*yp++);	
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
