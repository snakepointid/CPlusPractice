#include "dataset.h"
#include<fstream>
#include<cmath>
#include"common.h"
using std::ifstream;
void SGBT::dataset::loadDataFromCsv(string & filename, string & label)
{
	ifstream fin(filename);
	string line;
	getline(fin, line);
	
	vector<string>featureName = splits(line, ',');
	Dvector feat_val;
	for (auto&v : featureName) { if (v != label) { labelIdx++; } else { break; } }
	int obnum = 0;
	while (getline(fin, line))
	{
		if (obnum++ > 1000)break;
		feat_val = ATOF(line, ',');
		rawData.push_back(move(feat_val));
	}
	fin.close();
	featureValueSpan.resize(labelIdx);
 
}

void SGBT::dataset::getSpan()
{
#pragma omp parallel for
	for (auto&data : rawData) {
		for (int i = 0; i < labelIdx; i++) {
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

void SGBT::dataset::binMapper()
{
	rankData.resize(rawData.size());
	for (int i = 0; i < rawData.size(); i++) {
		rankData[i].resize(labelIdx+1);
		for (int j = 0; j < labelIdx; j++) {
			rankData[i][j] = featureValueSpan[j][rawData[i][j]];
		}
		rankData[i][labelIdx] = rawData[i][labelIdx];
	}
	rawData.swap(vector<Dvector>{});
}
