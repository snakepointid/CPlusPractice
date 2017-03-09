#pragma once
#ifndef SGBT_TREE_H_
#define SGBT_TREE_H_
#include"meta.h"
#include"dataset.hpp"
#include<memory>
#include<omp.h>

namespace SGBT {
	using subP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	using hist_t = Dvector;
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
		std::shared_ptr<Node>treeRoot;

		//split control 
		inline bool numericDecision(int feat,int threshold){
			if(feat<=threshold){return true;}else{return false;}
		}
		//compute the residual which used to train the next tree
		inline float getGradient(float label, float score) {
			 return 2 * label / (1 + std::exp(2 * label*score));  
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
		void train(dataset& dataset, Ivector &binValLen, int subfeatnum); 
		void growRoot(const vector<subP_t>&subsets);	 
		void Splits(const int fidx,const subP_t&subsets,int bin_size); 
		void getSubFeature(  int featNum, int subFeatNum);
		inline void predict(const subP_t & testdtset);

		Tree( float eta) :  alpha_(eta){};
		~Tree() {};
	};
 
	void Tree::growRoot(const vector<subP_t>& subsets)
	{
		float sumValue = 0.0;
		float sumNT = 0.0;
		for (int i = 0; i <= treeRoot->threshold; i++) {
			for (auto&data : subsets[i]) {
				sumValue += data->errLabel;
				sumNT += abs(data->errLabel)*(2 - abs(data->errLabel));
			}
		}
		if (sumNT == 0) { sumNT = 1.0; }
		treeRoot->leftValue = (sumValue / sumNT)*alpha_;
 
#pragma omp parallel for
		for (int i = 0; i <= treeRoot->threshold; i++) {
			for (auto&data : subsets[i]) {
				data->score += treeRoot->leftValue;
				data->errLabel = getGradient(data->label, data->score);
			}
		}
		sumValue = 0.0;
		sumNT = 0.0;
		for (int i = treeRoot->threshold+1; i <subsets.size(); i++) {
			for (auto&data : subsets[i]) {
				sumValue += data->errLabel;
				sumNT += abs(data->errLabel)*(2 - abs(data->errLabel));
			}
		}
		if (sumNT == 0) { sumNT = 1.0; }
		treeRoot->rightValue = (sumValue / sumNT)*alpha_;
	//	rightChild->leaf = true;
#pragma omp parallel for
		for (int i = treeRoot->threshold + 1; i <subsets.size(); i++) {
			for (auto&data : subsets[i]) {
				data->score += treeRoot->rightValue;
				data->errLabel = getGradient(data->label, data->score);
			}
		}
	}
	inline void Tree::predict(const subP_t & testdtset)
	{
		#pragma omp parallel for
		for (int i = 0; i < testdtset.size(); i++) {
			const auto&data = testdtset[i];
			addValueToScore(data);
		} 
	}
	// not support deep tree struct now,only use binary tree,this function is useless for binary tree
	void Tree::train(dataset&dataset, Ivector & binValLen,int subfeatnum){
		treeRoot.reset(new Node);
		getSubFeature(dataset.feature_num,subfeatnum);
		#pragma omp parallel for
		for (int i = 0; i < subFeatures.size(); i++) {
			int fix = subFeatures[i]; 
			Splits( fix, dataset.BinedData, binValLen[fix] );
		}	 
		//if (max_leave_depth_ > 1) { growTree(treeRoot, histGrams, dataset.BinedData); }	 
		growRoot(dataset.FeatBinedData[treeRoot->featureIdx]);		
	}

	 
	void Tree::Splits(int fidx,const subP_t&subsets, int bin_size){
		hist_t histgram(bin_size);		
		for(auto &data:subsets){
			int feat_bin = data->features[fidx];
			histgram[feat_bin]+=data->errLabel;
		}
 
		float nowColmul =0.0;
#pragma omp parallel for
		for(int i=0;i<bin_size;i++){
			nowColmul+=histgram[i];
			if(std::abs(nowColmul)> treeRoot->infoGain){
				treeRoot->infoGain = std::abs(nowColmul);
				treeRoot->threshold = i;
				treeRoot->featureIdx = fidx;
			}
		}
	}
	inline void Tree::getSubFeature(  int featNum, int subFeatNum)
	{
		if (subFeatNum<featNum) {
			subFeatures.reserve(subFeatNum);
			int sampNum = 0;
			while (sampNum<subFeatNum) {
				int feat = rand() % featNum;
				auto it = find(subFeatures.begin(), subFeatures.end(), feat);
				if (it == subFeatures.end()) { subFeatures.push_back(feat); sampNum++; }
			};
		}
		else {
			subFeatures.resize(featNum);
			int idx = 0;
			while (idx<featNum) {
				subFeatures[idx] = idx; idx++;
			}
		}
	}
}
#endif // !SGBT_TREE_H_
