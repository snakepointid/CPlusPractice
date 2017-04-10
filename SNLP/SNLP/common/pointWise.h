#ifndef POINTWISE_H_
#define POINTWISE_H_
#include"meta.h"
 
namespace SNLP {
	template<typename T >
	inline T PairWiseMulti(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { *zp++ = PairWiseMulti(*xp++, *yp++); }
		return z;
	}
	template< >  inline Fvector PairWiseMulti(const Fvector &x, const Fvector &y)
	{
		int m = x.size();
		Fvector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { *zv++ = (*xv++)*(*yv++); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseMulti(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { zp++->swap(PairWiseMulti(x, *yp++)); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseMulti(const vector<T> &x, const T&y)
	{
		return  PairWiseMulti(y, x);
	}
	//pairwise add
	template<typename T >
	inline T PairWiseADD(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { zp++->swap(PairWiseADD(*xp++, *yp++)); }
		return z;
	}
	template< >  inline Fvector PairWiseADD(const Fvector &x, const Fvector &y)
	{
		int m = x.size();
		Fvector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { *zv++ = (*xv++) + (*yv++); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseADD(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { *zp++ = PairWiseADD(x, *yp++); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseADD(const vector<T> &x, const T&y)
	{
		return  PairWiseADD(y, x);
	}

	//pairwise add
	template<typename T >
	inline T PairWiseMinus(const T &x, const T &y)
	{
		T z(x.size());
		int m = x.size(); const auto *xp = x.data(), *yp = y.data(); auto *zp = z.data();
		while (m-- > 0) { (*zp++) = PairWiseMinus(*xp++, *yp++); }
		return z;
	}
	template<>  inline Fvector PairWiseMinus(const Fvector &x, const Fvector &y)
	{
		int m = x.size();
		Fvector z(m); float *zv = z.data(); const float *xv = x.data(), *yv = y.data();
		while (m-- > 0) { (*zv++) = (*xv++) - (*yv++); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseMinus(const T &x, const  vector<T>&y)
	{
		vector<T> z(y.size());
		int m = y.size(); z.resize(m); T *zp = z.data(); const T *yp = y.data();
		while (m-- > 0) { zp++->swap(PairWiseMulti(x, *yp++)); }
		return z;
	}
	template<typename T >
	inline vector<T> PairWiseMinus(const vector<T> &x, const T&y)
	{
		return  PairWiseMinus(y, x);
	}

	 vector<Fvector> inline PairWiseMinus(const float &x, const vector<Fvector> &y)
	{
		vector<Fvector> z(y.size());
		int m = y.size();  const Fvector *yp = y.data(); Fvector *zp = z.data();
		while (m-- > 0)
		{
			int n = yp->size(); zp->resize(n); const float *yv = yp++->data(); float *zv = zp++->data();
			while (n-->0) { (*zv++) = x - (*yv++); }
		}
		return z;
	}
}
#endif // !POINTWISE_H_
#pragma once
