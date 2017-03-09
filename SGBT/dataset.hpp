#pragma once
#ifndef SGBT_DATASET_H_
#define SGBT_DATASET_H_
#include<string>

#include<memory>
 
#include"common.hpp"
#include<fstream>
using std::string;
using std::ifstream;
using std::ofstream;
namespace SGBT
{
	struct dataset
	{
		//a row data struct which hold the label,features,score and gradient value
		struct oneRowData{
			const Ivector features;
			const float label;
			float errLabel;
			float score;
		};
		using subP_t = vector<std::shared_ptr<oneRowData>>;
		vector<Dvector>rawData;
				 
		subP_t BinedData;
		vector<vector<subP_t>> FeatBinedData;
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
			int n = 0;
			while (getline(fin, line))
			{
				//if (n++ > 100000) { break; }
				feat_val = ATOF(line, ',');
				rawData.push_back(move(feat_val));
			}
			fin.close();		
			observe_num = rawData.size();
		}
		
	
		void writeOutToCsv(string & filename) {
			ofstream fout(filename);
			fout << "score" << "," << "label" << "\n";
			for (auto&data : BinedData) {
				fout << data->score << "," << data->label << "\n";
			}
			fout.close();
		}

	};
}

#endif // !SGBT_DATASET_H_
