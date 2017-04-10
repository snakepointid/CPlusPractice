#include "common\dot.h"
namespace SNLP {
	float dot(const Fvector & x, const Fvector & y)
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
	vector<Fvector> dot(const vector<Fvector>& x, const vector<Fvector>& y)
	{
		vector<Fvector> z(x.size());
		int xDim = x.size(); const Fvector *xp = x.data(); Fvector *zp = z.data();
		while (xDim-->0)
		{
			int yDim = y.size(); const Fvector *yp = y.data(); zp->resize(yDim); float *zv = zp->data();
			while (yDim-->0)
			{
				(*zv++) = dot(*xp, *yp++);
			}
			xp++; zp++;
		}
		return z;
	}
	Fvector dot(const Fvector & x, const vector<Fvector>& y)
	{
		const vector<Fvector>nx{ x };
		return *dot(nx, y).data();
	}
	vector<Fvector> dot(const vector<Fvector>& x, const Fvector & y)
	{
		const vector<Fvector>ny{ y };
		return dot(x, ny);
	}
	void norml2(Fvector & x)
	{
		float norm = ::sqrt(dot(x, x));
		if (norm == 0) return;
		int m = x.size(); float *xp = x.data();
		while (m-->0)(*xp++) /= norm;
	}
	void norml1(Fvector & x)
	{
		float norm = SUM(MAP(x, std::abs));
		if (norm == 0) return;
		int m = x.size(); float *xp = x.data();
		while (m-->0)(*xp++) /= norm;
	}
	float normdot(const Fvector & x, const Fvector & y)
	{
		Fvector xn = x;
		Fvector yn = y;
		norml1(xn);
		norml1(yn);
		return dot(xn, yn);
	}

	float mdot(const vector<Fvector> &x, const vector<Fvector> &y)
	{
		if (x.size() != y.size() | x.size() == 0 | x[0].size() != y[0].size()) { printf("the dimension is not right!!!"); abort(); }
		int m = x.size(); const Fvector *xp = x.data(), *yp = y.data();
		float sum = 0.0;
		while (m-->0)
		{
			sum += dot(*xp++, *yp++);
		}
		return sum;
	}
	Fvector mdot(const vector<Fvector> &x, const vector<vector<Fvector>> &y)
	{
		int m = y.size();
		Fvector z(m); float *zv = z.data(); const vector<Fvector>*yp = y.data();
		while (m-- > 0)
		{
			*zv++ = mdot(x, *yp++);
		}
		return z;
	}
	vector<Fvector> mdot(const vector<vector<Fvector>> &x, const vector<vector<Fvector>> &y)
	{
		int m = x.size();
		vector<Fvector> z(m); Fvector *zp = z.data(); const vector<Fvector> *xp = x.data();
		while (m-- > 0)
		{
			*zp++ = mdot(*xp++, y);
		}
		return z;
	}
	vector<Fvector> mdot(const vector<Fvector> &x, const vector<vector<Fvector>> &y, int window)
	{
		vector<Fvector> mapper(window); int winLen = x.size() - window + 1;  vector<Fvector> conver(winLen);
		Fvector *conp = conver.data(); const Fvector *xp = x.data(); Fvector *mapp = mapper.data();
		while (winLen-->0)
		{
			int winIcur = window;
			while (winIcur-- > 0)
			{
				*mapp++ = *xp++;
			}
			xp -= (window - 1); mapp -= window;
			*conp++ = mdot(mapper, y);
		}
		return conver;
	}
	vector<Fvector> mouter(const float &x, const vector<Fvector> &y)
	{
		vector<Fvector>z = y;
		scale(z, x);
		return z;
	}
	vector<Fvector> mouter(const Fvector &x, const vector<vector<Fvector>> &y)
	{
		int m = y.size();
		vector<Fvector>z; const float *xv = x.data(); const vector<Fvector>*yp = y.data();
		while (m-- > 0)
		{
			saxpy(z, 1, mouter(*xv++, *yp++));
		}
		return z;
	}
	vector<vector<Fvector>> mouter(const Fvector &x, const vector<Fvector> &y)
	{
		int m = x.size();
		vector<vector<Fvector>>z(m); const float *xv = x.data(); vector<Fvector>*zp = z.data();
		while (m-- > 0)
		{
			*zp++ = mouter(*xv++, y);
		}
		return z;
	}
	vector<vector<Fvector>> mouter(const vector<Fvector> &x, const vector<Fvector> &y, int window)
	{
		vector<Fvector> mapper(window); int winLen = x.size(); vector<vector<Fvector>>conver;
		const Fvector *xp = x.data(); const Fvector *yp = y.data(); Fvector *mapp = mapper.data();
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
	vector<Fvector> mouter(const vector<Fvector> &x, const vector<vector<Fvector>> &y, int window)
	{
		int winLen = x.size(); vector<Fvector> z(winLen + window - 1);
		const Fvector *xp = x.data(); vector<Fvector>mapper(window); Fvector *zp = z.data();
		while (winLen-->0)
		{
			mapper = mouter(*xp++, y); const Fvector *mapp = mapper.data();
			int winIcur = window;
			while (winIcur-- > 0)
			{
				saxpy(*zp++, 1, *mapp++);
			}
			zp -= (window - 1);
		}
		return z;
	}
}