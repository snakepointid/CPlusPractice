#include"common\matrInit.h"
namespace SNLP {
	vector<Fvector> mtp(const vector<Fvector>& x)
	{
		int yRdim = x[0].size(), yCdim = x.size();
		vector<Fvector> y(yRdim); Fvector *yp = y.data();
		int xcol = 0;
		while (yRdim-- > 0)
		{
			const  Fvector *xp = x.data();
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
	vector<Fvector> mtp(const Fvector & x)
	{
		vector<Fvector>nx{ x };
		return mtp(nx);
	}
	vector<Fvector> mtp(const float & x)
	{
		Fvector nx{ x };
		return mtp(nx);
	}
}