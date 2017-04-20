#ifndef SGBT_BIN_H_
#define SGBT_BIN_H_
#include"meta.h"
#include<map>
#include"dataset.h"
namespace rootGBM {
	struct dataset;
	struct bin
	{
		vector<std::map<float, int> >featureValueSpan;
		vector<Dvector >BinVal;
		Ivector binLength;
		int feature_num;
		int binNum_;

		explicit bin(int binNum):binNum_(binNum){};
		void constructBins(dataset*dataset);
		void binMapper(dataset*dataset);
		float getBinMapValue(int fidx, const float f);
	};
}
#endif // !SGBT_BIN_H_
