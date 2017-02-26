//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef S_SAXPY_H
#define S_SAXPY_H
#include<vector>
#include <cstdlib>
#include<iostream>
#include<random>

namespace SVC
{
	//scale
	using std::vector;
	using Vector = vector<float>;
	
	//scale
	template<typename T>
	inline void scale(T &x, float g )
	{
		 int m = x.size();
		 auto *xp = x.data();
		 while(m-->0){scale(*xp++,g);}

	}
	template<>inline void scale(Vector &x, float g )
	{
        for(auto&v:x){v=v*g;}
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
	inline void saxpy(vector<Vector*> &x, float g, const vector<Vector> &y)
	{	 
		int m = x.size(); auto *xp = x.data(); const auto *yp = y.data();
		while (m-- > 0)saxpy(**xp++, g, *yp++);	
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
	inline void saxpy(vector<Vector*> &x, float g)
	{	 
		int m = x.size(); auto *xp = x.data();  
		while (m-- > 0)saxpy(**xp++, g);	
	}
}

#endif /* vecComp_h */
