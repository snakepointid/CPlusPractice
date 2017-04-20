#include"include/boost.h"
namespace rootGBM {
	void  boost::train(dataset*datasets, bin*bin,int exactMode)
	{	
		NegLikeGrad neglikegrad;
		if (exactMode == 0) { neglikegrad = common::speedNegLikeGrad; }
		else {
			 neglikegrad = common::exactNegLikeGrad;
		}
		if (datasets->trainedIterNum > 0) {
			initDataset(datasets);
		}
		int featAddGap = 4 * max_tree_iter_ / datasets->feature_num;
		treeP.reserve(max_tree_iter_);
		int sampFeatNum = 0;
		for (int i = 0; i < max_tree_iter_; i++) {
			if (i%featAddGap == 0) { sampFeatNum++; }
		 
			treeP.emplace_back(new Tree(alpha_, neglikegrad));
			treeP[i]->train(datasets, bin->binLength, sampFeatNum);
			datasets->trainedIterNum++;
		}
	}
	void boost::predict(dataset*datasets)
	{
		if (datasets->trainedIterNum > 0) {
			initDataset(datasets);
		}
		for (auto&tree : treeP) {
			tree->predict(datasets->BinedData);
		}
	}
	void boost::initDataset(dataset * datasets)
	{
		datasets->trainedIterNum = 0;
		for (auto&data : datasets->BinedData) {
			data->score = 0.0;
			data->errLabel = data->label;
		}
	}
}
