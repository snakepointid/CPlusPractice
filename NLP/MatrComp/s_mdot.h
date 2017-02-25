//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_MDOT_H
#define S_MDOT_H
#include<vector>
#include"s_dot.h"
#include"s_saxpy.h"
namespace SVC
{
	using std::vector;
	using Vector = vector<float>;
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
			*zv++ = mdot(x, *yp++);
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

	inline vector<Vector> mdot(const vector<Vector> &x, const vector<vector<Vector>> &y,int window)
	{
		vector<Vector> mapper(window); int winLen = x.size() - window + 1;  vector<Vector> conver(winLen);
		Vector *conp = conver.data(); const Vector *xp = x.data(); Vector *mapp = mapper.data();
		while (winLen-->0)
		{
			int winIcur = window;		
			while (winIcur-- > 0)
			{
				*mapp++ = *xp++;
			}
			xp -= (window - 1); mapp -= window;
			conp++->swap(mdot(mapper, y));
		}
		return conver;
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

	inline vector<vector<Vector>> mouter(const vector<Vector> &x, const vector<Vector> &y, int window)
	{
		vector<Vector> mapper(window); int winLen = x.size(); vector<vector<Vector>>conver;
		const Vector *xp = x.data(); const Vector *yp = y.data(); Vector *mapp = mapper.data();
		while (winLen-->0)
		{
			int winIcur = window;			 
			while (winIcur-- > 0)
			{
				*mapp++ = *yp++;
			}
			yp -= (window - 1); mapp -= window;
			saxpy(conver, 1, mouter(*xp++, mapper));
		}
		return conver;
	}
	inline  vector<Vector> mouter(const vector<Vector> &x, const vector<vector<Vector>> &y, int window)
	{
		int winLen = x.size(); vector<Vector> z(winLen + window - 1);
		const Vector *xp = x.data(); vector<Vector>mapper(window); Vector *zp = z.data(); 
		while (winLen-->0)
		{
			mapper.swap(mouter(*xp++, y)); const Vector *mapp = mapper.data();
			int winIcur = window;
			while (winIcur-- > 0)
			{		
				saxpy(*zp++,1,*mapp++);
			}
			zp -= (window - 1); 
		}
		return z;
	}
}

#endif  
