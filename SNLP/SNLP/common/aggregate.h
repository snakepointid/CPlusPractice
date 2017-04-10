#ifndef AGGREGATE_H_
#define AGGREGATE_H_
#include"meta.h"
#include"saxpy.h"
 
namespace SNLP {
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

	template< >inline float SUM(const Fvector &x)
	{
		float sumr = 0.0; int m = x.size(); const float *xv = x.data();
		while (m-->0)
		{
			sumr += *xv++;
		}
		return sumr;
	}
	inline Fvector SUM(const vector<Fvector> &x, int dim)
	{
		Fvector z;
		if (dim == 1)
		{
			int m = x.size(); const Fvector *xp = x.data();
			while (m-- > 0) { saxpy(z, 1, *xp++); }
		}
		else
		{
			int m = x.size(); z.resize(m); float *zv = z.data(); const Fvector *xp = x.data();
			while (m-- > 0) { *zv++ = SUM(*xp++); }
		}
		return z;
	}
	inline Fvector SUM(const Fvector &x, int dim)
	{
		return x;
	}
	//sum
	template<typename T >
	inline float MEAN(const T &x)
	{
		float sumr = 0.0; int m = x.size(); const auto *xp = x.data();
		while (m-->0)
		{
			sumr += MEAN(*xp++);
		}
		return sumr / x.size();
	}
	template< >inline float MEAN(const Fvector &x)
	{
		float sumr = 0.0; int m = x.size(); const float *xv = x.data();
		while (m-->0)
		{
			sumr += *xv++;
		}
		return sumr / x.size();
	}
}
#endif // !AGGREGATE_H_
