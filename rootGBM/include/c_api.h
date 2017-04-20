#ifndef C_API_H_
#define C_API_H_
#define rootGBM_C_EXPORT extern "C" __declspec(dllexport)
 
 
#include<vector>
#include<memory>
#include "dataset.h"
#include "Bin.h"
#include "boost.h"
typedef rootGBM::dataset* datasethandle_t;
typedef rootGBM::bin* binhandle_t;
typedef rootGBM::boost* BoosterHandle_t;
 
rootGBM_C_EXPORT int DatasetCreateFromNumpy(float *data, float *label, int nrow, int ncol, int binNum, int exactMode, binhandle_t *binhandle, datasethandle_t * datahandle);
 
rootGBM_C_EXPORT int CreateBoost(int max_tree_iter, float alpha, int threadNums, BoosterHandle_t*boosthandle);

 
rootGBM_C_EXPORT int getScore(datasethandle_t datahandle, float *preds);

rootGBM_C_EXPORT int TrainBoost(datasethandle_t datahandle, binhandle_t binhandle, BoosterHandle_t boosthandle ,int exactMode);
rootGBM_C_EXPORT int pred_np_array(BoosterHandle_t booster, binhandle_t binmapper, float * data, int nrow, int ncol, float * preds);
 
rootGBM_C_EXPORT int BoosterFree(BoosterHandle_t boosthandle);
rootGBM_C_EXPORT int DatasetFree(datasethandle_t datahandle);
rootGBM_C_EXPORT int BinFree(binhandle_t binhandle);
 
#endif // !C_API_H_
