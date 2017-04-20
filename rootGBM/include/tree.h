#pragma once
#ifndef SGBT_TREE_H_
#define SGBT_TREE_H_
#include "meta.h"
#include "dataset.h"
#include<memory>
//include<omp.h>

namespace rootGBM {
 
	using dataP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	typedef float(*NegLikeGrad)(float label, float score);
	//tree class
	class Tree
	{

	private:
		//node struct which will save the split info
		struct Node {
			//split info gain,always choose the split point which get max info gain
			float infoGain = -1.0;
			// the split feature
			int featureIdx;
			// the threshold which use to determine the data will classify into the left or right
			int threshold;
			//point to the left treeRoot and right treeRoot
			float leftValue;
			float rightValue;
		};

		//subsample the features
		Ivector subFeatures;
		// the learning rate which can control the learning step
		float alpha_;
		//initial a treeRoot node ,classify always start from the treeRoot
		std::unique_ptr<Node>treeRoot;
		NegLikeGrad neglikegrad_;
		//split control 
		inline bool numericDecision(int feat, int threshold) {
			if (feat <= threshold) { return true; }
			else { return false; }
		}
		//compute the residual which used to train the next tree
		inline float getGradient(const float label, const float score) {
			return neglikegrad_(label, score);
		}
		//add the value of the leaf which the data belong to the score,this function attempt to only used in the predict steps
		inline void addValueToScore(const std::shared_ptr<dataset::oneRowData>&data) {
			if (numericDecision(data->features[treeRoot->featureIdx], treeRoot->threshold)) {
				data->score += treeRoot->leftValue;
			}
			else {
				data->score += treeRoot->rightValue;
			}
		}

	public:
		//train the tree
		void train(dataset*dataset, const Ivector &binValLen, int subfeatnum);
		void growRoot(const vector<dataP_t>&subsets);
		void Splits(const int fidx, const dataP_t&subsets, Ivector&randDataGroup, int bin_size);
		void _getSubFeature(int featNum, int subFeatNum);
		void predict(const dataP_t & testdtset);
		Tree(float eta, NegLikeGrad neglikegrad) : alpha_(eta),neglikegrad_(neglikegrad) {}
	};	
}
#endif // !SGBT_TREE_H_
