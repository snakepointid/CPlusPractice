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
			const Ivector features;
			const float label;
			float errLabel;
			float score;
		};

		vector<Dvector>rawData;
		vector<std::map<float, int> >featureValueSpan;		 
		vector<std::shared_ptr<oneRowData> >dataframeP;
		int feature_num=0;
		int observe_num=0;
	 
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
				//if (observe_num++ > 1000)break;
				feat_val = ATOF(line, ',');
				rawData.push_back(move(feat_val));
			}
			fin.close();
			featureValueSpan.resize(feature_num);		
			observe_num = rawData.size();
		}
		void getSpan()
		{
 
		for (int ob = 0; ob < observe_num;ob++) {
			for (int i = 0; i < feature_num; i++) {
				featureValueSpan[i][rawData[ob][i]]++;
			}
		}
 
		for(int fix=0;fix<feature_num;fix++) {
			int idx = 0;
			for (auto it = featureValueSpan[fix].begin(); it != featureValueSpan[fix].end(); it++) {
				featureValueSpan[fix][it->first] = idx++;
			}
		}
		}

		void binMapper()
		{		
			dataframeP.reserve(observe_num);
			Ivector tmpFeatures(feature_num);		
			for (int i = 0; i < observe_num; i++) {
				#pragma omp parallel for
				for (int j = 0; j < feature_num; j++) {
					tmpFeatures[j] = featureValueSpan[j][rawData[i][j]];
				}
			dataframeP.emplace_back(new oneRowData{ tmpFeatures,rawData[i][feature_num],rawData[i][feature_num],0.0});
			}
			vector<Dvector>().swap(rawData);
		}

	};
}

#endif // !SGBT_DATASET_H_
