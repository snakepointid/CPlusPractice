#ifndef MAPFUNC_H_
#define MAPFUNC_H_
#include"meta.h"
namespace SNLP {
	typedef float(*fps)(const float);
	template<typename T >
	inline T MAP(const T &x, fps fp)
	{
		int m = x.size(); T z(m); const auto *xp = x.data(); auto *zp = z.data();
		while (m-->0)
		{
			*zp++ = MAP(*xp++, fp);
		}
		return z;
	}
	template< >inline Fvector MAP(const Fvector &x, fps fp)
	{
		int m = x.size(); Fvector z(m); const float *xv = x.data(); float *zv = z.data();
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
	template< >inline Fvector MAP(const Fvector &x, fpss fp, float g)
	{
		int m = x.size(); Fvector z(m); const float *xv = x.data(); float *zv = z.data();
		while (m-->0)
		{
			(*zv++) = fp(*xv++, g);
		}
		return z;
	}
}
#endif // !MAPFUNC_H_

