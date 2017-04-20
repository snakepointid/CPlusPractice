#ifndef ROOTGBM_DATASET_H_
#define ROOTGBM_DATASET_H_
#include<string>
#include<memory>
#include"Bin.h"
#include "common.h"
#include<fstream>
using std::string;
using std::ifstream;
using std::ofstream;

namespace rootGBM
{
	struct bin;
	struct dataset
	{
		//a row data struct which hold the label,features,score and gradient value
		struct oneRowData {
			const Ivector features;
			const float label;
			float errLabel;
			float score;
		};
		using subP_t = vector<std::shared_ptr<oneRowData>>;
		std::unique_ptr< float>rawData;
		std::unique_ptr< float>rawLabel;

		int feature_num = 0;
		int observe_num = 0;
		int randGroupNum=1;
		int exactMode_;
		int trainedIterNum = 0;
		subP_t BinedData;
		vector<Ivector> dataRandGroup;
		vector<vector<subP_t>> FeatBinedData;

		explicit dataset(int exactMode=0) :exactMode_(exactMode) {};

	 	void  loadDataFromCsv(string & filename, string & label, int nrow, int ncol);
		void  loadDataFromNP(float *data, float *label, int nrow, int ncol, bin* binMapper);
		void  loadDataFromNP(float *data,int nrow, int ncol, bin * binMapper);
		void  writeOutToCsv(string & filename);
 
	};
}

#endif // !SGBT_DATASET_H_
