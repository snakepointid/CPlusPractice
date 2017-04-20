#include"include/dataset.h"
namespace rootGBM {
	void  dataset::loadDataFromCsv(string & filename, string & label, int nrow, int ncol)
	{
		rawData.reset(new float[nrow*ncol]);
		rawLabel.reset(new float[nrow]);
		ifstream fin(filename);
		string line;
		string item;
		//get label index
		getline(fin, line);
		int labelIdx;
		vector<string>featureName = common::splits(line, ',');
		for (auto&v : featureName) {
			if (v != label) { feature_num++; }
			else {
				labelIdx = feature_num;
			}
		}
		
		int arr_idx = 0;
		int rowCur = 0;
		while (getline(fin, line))
		{		 
			int colCur = 0;
			std::stringstream ss(line);
			while (getline(ss, item,',') ){
				if (colCur++ !=labelIdx) { rawData.get()[arr_idx++] = atof(item.c_str()); }
				else {
					rawLabel.get()[rowCur++] = atof(item.c_str());
				}			
			}
		}
		if (exactMode_ == 0 && nrow > 20000) {
			randGroupNum = nrow / 10000;
		}
		dataRandGroup.resize(randGroupNum);
		for (int i = 0; i < nrow; i++) {
			dataRandGroup[rand() % randGroupNum].push_back(i);
		}
		fin.close();
		observe_num = nrow;
	}

	void dataset::loadDataFromNP( float * data,  float * label, int nrow, int ncol,bin*binMapper)
	{
		observe_num = nrow;
		feature_num = ncol;	
		//
		if (exactMode_ == 0 && nrow > 20000) {
			randGroupNum = nrow / 10000;
		}
		dataRandGroup.resize(randGroupNum);
		for (int i = 0; i < nrow; i++) {
			dataRandGroup[rand() % randGroupNum].push_back(i);
		}
		//
		binMapper->feature_num =  feature_num;
		int min_bin_num = observe_num / binMapper->binNum_;
		binMapper->featureValueSpan.resize(feature_num);
		binMapper->BinVal.resize(feature_num);
		binMapper->binLength.resize(feature_num);

#pragma omp parallel for
		for (int rowCur = 0; rowCur < observe_num; rowCur++) {
			for (int colCur = 0; colCur < feature_num; colCur++) {
				binMapper->featureValueSpan[colCur][data[rowCur*feature_num + colCur]] += 1;
			}
		}

#pragma omp parallel for
		for (int fidx = 0; fidx < feature_num; fidx++) {
			int sampNumSum = 0;
			for (auto it = binMapper->featureValueSpan[fidx].begin(); it != binMapper->featureValueSpan[fidx].end(); it++) {
				binMapper->BinVal[fidx].reserve(binMapper->featureValueSpan[fidx].size());
				sampNumSum += it->second;
				if (sampNumSum > min_bin_num) {
					binMapper->BinVal[fidx].push_back(it->first);
					sampNumSum = 0;
				}
			}
			Dvector(binMapper->BinVal[fidx]).swap(binMapper->BinVal[fidx]);
			binMapper->binLength[fidx] = binMapper->BinVal[fidx].size() + 1;
		}
		//
		BinedData.reserve(observe_num);
		Ivector tmpFeatures(feature_num);
		for (int rowCur = 0; rowCur < observe_num; rowCur++) {
			for (int colCur = 0; colCur < feature_num; colCur++) {

				tmpFeatures[colCur] = binMapper->getBinMapValue(colCur,  data[rowCur*feature_num + colCur]);
			}
			 BinedData.emplace_back(new dataset::oneRowData{ tmpFeatures, label[rowCur], label[rowCur],0.0 });
		}

		 FeatBinedData.resize(feature_num);
#pragma omp parallel for
		for (int i = 0; i < feature_num; i++) {
			 FeatBinedData[i].resize(binMapper->binLength[i]);
			for (auto&data : BinedData) {
				 FeatBinedData[i][data->features[i]].emplace_back(data);
			}
		}
	}

	void dataset::loadDataFromNP(float * data, int nrow, int ncol, bin * binMapper)
	{
		observe_num = nrow;
		feature_num = ncol;
		 
		BinedData.reserve(observe_num);
		Ivector tmpFeatures(feature_num);
		for (int rowCur = 0; rowCur < observe_num; rowCur++) {
			for (int colCur = 0; colCur < feature_num; colCur++) {
				tmpFeatures[colCur] = binMapper->getBinMapValue(colCur, data[rowCur*feature_num + colCur]);
			}
			BinedData.emplace_back(new dataset::oneRowData{ tmpFeatures, 0.0, 0.0,0.0 });
		}
		FeatBinedData.resize(feature_num);
#pragma omp parallel for
		for (int i = 0; i < feature_num; i++) {
			FeatBinedData[i].resize(binMapper->binLength[i]);
			for (auto&data : BinedData) {
				FeatBinedData[i][data->features[i]].emplace_back(data);
			}
		}
	}


	void  dataset::writeOutToCsv(string & filename) {
		ofstream fout(filename);
		fout << "score" << "," << "label" << "\n";
		for (auto&data : BinedData) {
			fout << data->score << "," << data->label << "\n";
		}
		fout.close();
	}

 
}