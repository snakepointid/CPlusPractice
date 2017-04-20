#include"include/Bin.h"
namespace rootGBM {
	void bin::constructBins(dataset*dataset)
	{
		
		feature_num = dataset->feature_num;
		int observe_num = dataset->observe_num;
		int min_bin_num = observe_num/ binNum_;
 
		featureValueSpan.resize(feature_num);
		BinVal.resize(feature_num);
		binLength.resize(feature_num);
 
#pragma omp parallel for
		for (int rowCur = 0; rowCur < observe_num; rowCur++) {
			for (int colCur = 0; colCur < feature_num; colCur++) {
				featureValueSpan[colCur][dataset->rawData.get()[rowCur*feature_num + colCur]] += 1;
			}
		}
		
#pragma omp parallel for
		for (int fidx = 0; fidx < feature_num; fidx++) {
			int sampNumSum = 0;
			for (auto it = featureValueSpan[fidx].begin(); it != featureValueSpan[fidx].end(); it++) {
				BinVal[fidx].reserve(featureValueSpan[fidx].size());
				sampNumSum += it->second;
				if (sampNumSum > min_bin_num) {
					BinVal[fidx].push_back(it->first);
					sampNumSum = 0;
				}
			}
			Dvector(BinVal[fidx]).swap(BinVal[fidx]);
			binLength[fidx] = BinVal[fidx].size() + 1;
		}
		 
 
	}
	void bin::binMapper(dataset*dataset)
	{
		int observe_num = dataset->observe_num;
		dataset->BinedData.reserve(observe_num);
		Ivector tmpFeatures(feature_num);
		for (int rowCur = 0; rowCur < observe_num; rowCur++) {
			for (int colCur = 0; colCur < feature_num; colCur++) {
 
				tmpFeatures[colCur] = getBinMapValue(colCur, dataset->rawData.get()[rowCur*dataset->feature_num + colCur]);
			}
			dataset->BinedData.emplace_back(new dataset::oneRowData{ tmpFeatures,dataset->rawLabel.get()[rowCur],dataset->rawLabel.get()[rowCur],0.0 });
		}
 
		dataset->FeatBinedData.resize(feature_num);
#pragma omp parallel for
		for (int i = 0; i < feature_num; i++) {
			dataset->FeatBinedData[i].resize(binLength[i]);
			for (auto&data : dataset->BinedData) {
				dataset->FeatBinedData[i][data->features[i]].emplace_back(data);
			}
		}
		dataset->rawData.reset();
		dataset->rawLabel.reset();
	}
	 float bin::getBinMapValue(int fidx, const float f) {
		for (int i = 0; i < BinVal[fidx].size(); i++) {
			if (BinVal[fidx][i] > f) { return i; }
		}
		return BinVal[fidx].size();
	}
}