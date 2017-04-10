#ifndef SAXPY_H_
#define SAXPY_H_
#include"meta.h"
 
namespace SNLP {
	//scale
	template<typename T>
	inline void scale(T &x, float g)
	{
		int m = x.size();
		auto *xp = x.data();
		while (m-->0) { scale(*xp++, g); }

	}
	template< >inline void scale(Fvector &x, float g)
	{
		for (auto&v : x) { v = v*g; }
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

	template< >inline void saxpy(Fvector &x, float g, const Fvector &y)
	{
		int m = x.size(); float *xv = x.data(); const float *yv = y.data();
		while (m-- > 0) {
			*xv += g*(*yv); xv++; yv++;
		}
	}
 

	template<typename T>
	inline void saxpy(T &x, const float g)
	{
		int m = x.size(); auto *xp = x.data();
		while (m-- > 0)saxpy(*xp++, g);
	}

	template< >inline void saxpy(Fvector &x, const float g)
	{
		int m = x.size(); float *xv = x.data();
		while (m-- > 0) {
			*xv += g*(*xv); xv++;
		}
	}
	 
}
#endif // !SAXPY_H_
