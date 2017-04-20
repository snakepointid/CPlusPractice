#include "include/c_api.h"
using namespace std;
 
rootGBM_C_EXPORT int DatasetCreateFromNumpy( float *data,  float *label, int nrow, int ncol,int binNum,int exactMode, binhandle_t *binhandle,datasethandle_t * datahandle)
{
	std::unique_ptr<rootGBM::dataset> daset(new rootGBM::dataset(exactMode));
	std::unique_ptr<rootGBM::bin> bin(new rootGBM::bin(binNum));
	daset->loadDataFromNP(data, label, nrow, ncol, bin.get());
	printf("sample numbers:%lu, feature numbers:%lu\n", daset->observe_num, daset->feature_num);
	*datahandle = daset.release();
	*binhandle = bin.release();
	return 1;
}
 

rootGBM_C_EXPORT int CreateBoost(int max_tree_iter, float alpha,int threadNums, BoosterHandle_t * boosthandle)
{
	std::unique_ptr<rootGBM::boost> ret(new rootGBM::boost(max_tree_iter,alpha, threadNums));
	*boosthandle = ret.release();
	return 1;
}

 
 

rootGBM_C_EXPORT int getScore(datasethandle_t datahandle, float * preds)
{
	for (int i = 0; i < datahandle->BinedData.size(); i++) {
		preds[i] = datahandle->BinedData[i]->score;
	}
	return 1;
}

rootGBM_C_EXPORT int TrainBoost(datasethandle_t datahandle, binhandle_t binhandle, BoosterHandle_t boosthandle ,int exactMode)
{
	boosthandle->train(datahandle, binhandle, exactMode);
	return 1;
}

rootGBM_C_EXPORT int pred_np_array(BoosterHandle_t booster, binhandle_t binmapper,  float * data, int nrow, int ncol, float * preds)
{
 
	std::unique_ptr<rootGBM::dataset> tmp(new rootGBM::dataset);
	tmp->loadDataFromNP(data, nrow, ncol, binmapper);
	booster->predict(tmp.get());
	for (int i = 0; i < tmp->BinedData.size();i++) {
		preds[i] = tmp->BinedData[i]->score;
	}
	tmp.reset();
	return 1;
}
 

rootGBM_C_EXPORT int BoosterFree(BoosterHandle_t boosthandle)
{
	delete boosthandle;
	return 1;
}

rootGBM_C_EXPORT int DatasetFree(datasethandle_t datahandle)
{
	delete datahandle;
	return 1;
}

rootGBM_C_EXPORT int BinFree(binhandle_t binhandle)
{
	delete binhandle;
	return 1;
}
