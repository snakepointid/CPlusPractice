
#include<vector>
#include<memory>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string>
#include "dataset.h"
#include "common.h"
using namespace std;

int main()
{
	string datasetFileName = "train.csv";
	string targetName = "label";
	SGBT::dataset dataset;
	dataset.loadDataFromCsv(datasetFileName, targetName);
	dataset.getSpan();
	dataset.binMapper();
	SGBT::showVec(dataset.rankData);
	


}
