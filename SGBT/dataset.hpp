#pragma once
#ifndef SGBT_DATASET_H_
#define SGBT_DATASET_H_
#include<string>
#include<map>
#include<memory>
#include"meta.h"
#include"common.hpp"
#include<fstream>
using std::string;
using std::ifstream;
namespace SGBT
{
	struct dataset
	{
		struct oneRowData{
			Ivector features;
			float label;
		};

		vector<Dvector>rawData;
		vector<std::map<float, int> >featureValueSpan;		 
		vector<std::unique_ptr<oneRowData> >dataframeP;
		int feature_num=0;
		int observe_num;
	 
		void loadDataFromCsv(string & filename, string & label)
		{
			ifstream fin(filename);
			string line;
			getline(fin, line);
			
			vector<string>featureName = splits(line, ',');
			for (auto&v : featureName) { if (v != label) { feature_num++; } else { break; } }

			Dvector feat_val;
			while (getline(fin, line))
			{	 
				feat_val = ATOF(line, ',');
				rawData.push_back(move(feat_val));
			}
			fin.close();
			featureValueSpan.resize(feature_num);		
		}
		void getSpan()
		{
		#pragma omp parallel for
			for (auto&data : rawData) {
				for (int i = 0; i < feature_num; i++) {
					featureValueSpan[i][data[i]]++;
				}
			}
		#pragma omp parallel for
			for (auto &mapper : featureValueSpan) {
				int idx = 0;
				for (auto it = mapper.begin(); it != mapper.end(); it++) {
					mapper[it->first] = idx++;
				}
			}
		}

		void binMapper()
		{
			observe_num = rawData.size();
			dataframeP.resize(observe_num);
			Ivector tmpFeatures(feature_num);
			for (int i = 0; i < observe_num; i++) {
				for (int j = 0; j < feature_num; j++) {
					tmpFeatures[j] = featureValueSpan[j][rawData[i][j]];
				}
				dataframeP[i]->features = tmpFeatures;
				dataframeP[i]->label = rawData[i][feature_num];			 
			}
			vector<Dvector>().swap(rawData);
		}

	};
}

#endif // !SGBT_DATASET_H_
