#ifndef SGBT_BIN_H_
#define SGBT_BIN_H_
#include"meta.h"
#include<map>
#include"dataset.hpp"
namespace SGBT {
	struct bin
	{
		vector<std::map<float, int> >featureValueSpan;
		vector<Dvector >BinVal;
		Ivector binLength;
		
		int feature_num;
		float binWeight_;
		explicit bin(float binWeight):binWeight_(binWeight){};
		~bin() {};
		void constructBins(vector<Dvector>&rawData)
		{		
			feature_num = rawData[0].size() - 1;
			int observe_num = 1000;
			int min_bin_num = observe_num*binWeight_;
			featureValueSpan.resize(feature_num);
			BinVal.resize(feature_num);
			binLength.resize(feature_num);
			for (int ob = 0; ob < observe_num; ob++) {
				for (int i = 0; i < feature_num; i++) {
					featureValueSpan[i][rawData[ob][i]] += 1;
				}
			}
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
		void binMapper(dataset&dataset)
		{
			int observe_num = dataset.rawData.size();
			dataset.BinedData.reserve(observe_num);
			Ivector tmpFeatures(feature_num);
			for (int i = 0; i < observe_num; i++) {
			#pragma omp parallel for
				for (int j = 0; j < feature_num; j++) {
					tmpFeatures[j] = getBinMapValue(j, dataset.rawData[i][j]);
				}
				dataset.BinedData.emplace_back(new dataset::oneRowData{ tmpFeatures,dataset.rawData[i][feature_num],dataset.rawData[i][feature_num],0.0 });
			}
			dataset.FeatBinedData.resize(feature_num);
			#pragma omp parallel for
			for (int i = 0; i < feature_num; i++) {
				dataset.FeatBinedData[i].resize(binLength[i]);
				for (auto&data : dataset.BinedData) {
					dataset.FeatBinedData[i][data->features[i]].push_back(data);
				}
			}
			vector<Dvector>().swap(dataset.rawData);
		}
	private:
		inline float getBinMapValue(int fidx,const float f) {
			for (int i = 0; i < BinVal[fidx].size(); i++) {
				if (BinVal[fidx][i] > f) { return i; }
			}
			return BinVal[fidx].size();
		}
	};
}
#endif // !SGBT_BIN_H_
