#pragma once
#ifndef SGBT_DATASET_H_
#define SGBT_DATASET_H_
#include<string>
#include<map>
#include"meta.h"
using std::string;
namespace SGBT
{
	struct dataset
	{
		vector<Dvector>rawData;
		vector<std::map<float, int>>featureValueSpan;
		 
		vector<Ivector>rankData;
		int feature_num;
		int labelIdx = 0;

	 
		void loadDataFromCsv(string&filename, string&label);
		void getSpan();
		void binMapper();

	};
}

#endif // !SGBT_DATASET_H_
