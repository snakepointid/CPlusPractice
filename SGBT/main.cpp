
#include<vector>
#include<memory>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string>
#include "dataset.hpp"
#include "common.hpp"

using namespace std;

int main()
{
	string datasetFileName = "train.csv";
	string targetName = "label";
	SGBT::dataset dataset;
	dataset.loadDataFromCsv(datasetFileName, targetName);
	dataset.getSpan();
	dataset.binMapper();
    for(auto &p:dataset.dataframeP){
        SGBT::showVec(p->features);
		cout<<" "<<p->label<<endl;
    }
}
