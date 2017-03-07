#pragma once
#ifndef SGBT_TREE_H_
#define SGBT_TREE_H_
#include"meta.h"
#include"dataset.hpp"
#include<memory>
#include<omp.h>

namespace SGBT {
	using subP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	using hist_t = vector<Dvector>;
 
	//tree class
	class Tree
	{
	private:
		//node struct which will save the split info
		struct Node {
			//split info gain,always choose the split point which get max info gain
			float infoGain = -1.0;
			// the node depth in the tree
			int depth = 1;
			// the leaves maping value
			float value;
			// the split feature
			int featureIdx;
			// the threshold which use to determine the data will classify into the left or right
			int threshold;
			//welther the node is the leaf
			bool leaf = false;
			//point to the left child and right child
			std::shared_ptr<Node> leftChild = nullptr;
			std::shared_ptr<Node>rightChild = nullptr;
		};
		//features numbers
		int featureNums; 
		//subsample the features
		Ivector subFeatures;
		//the max tree depth
		int max_leave_depth_;
		//the leaves samples number
		int min_child_num_;
		// the learning rate which can control the learning step
		float alpha_;
		// used to determine the histgram wide
		Ivector binValLen_;
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
			std::shared_ptr<Node>tmpNode = treeRoot;
			while (!tmpNode->leaf) {
				if (numericDecision(data->features[tmpNode->featureIdx], tmpNode->threshold)) {
					tmpNode = tmpNode->leftChild;
				}
				else {
					tmpNode = tmpNode->rightChild;
				}
			}
			data->score+=tmpNode->value;
		}
		//print the node structure 
		inline void printNode(std::shared_ptr<Node>&node) {
			if (node->leaf) { printf("leave value: %.4f\n", node->value); }
			else {
				printf("feature: %lu, threshold: %lu\n", node->featureIdx, node->threshold);
			}	
			if (node->leftChild) {
				printNode(node->leftChild);
			}
			if(node->rightChild) {
				printNode(node->rightChild);
			}
		}
		
	public:
		//begin to train the tree
		void train(dataset& dataset, Ivector &binValLen, int subfeatnum);
		//grow the tree
		void growTree(std::shared_ptr<Node>&node, const hist_t&histGrams, const subP_t&subsets );
		//paralleled to split those samp feature and choose the best 
		bool findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets ,const bool left);
		//split feature and get best threshold and info gain
		void Splits(Dvector &histgram,std::shared_ptr<Node>&child,const int fidx,const subP_t&subsets,int bin_size);
		//split the training sample into left and right subsets
		void getSubset(const subP_t&subsets, subP_t&left_subsets, subP_t&right_subsets, const std::shared_ptr<Node>&node);
		//overload the getSubset funcs
		void getSubset(const vector<vector<subP_t>>&subsets, subP_t&left_subsets, subP_t&right_subsets, const std::shared_ptr<Node>&node, int datasize);
		//get the right subsets histgram by using parent histgram minus the left histgram
		void getHist(const hist_t&histGrams, const hist_t&left_histGrams, hist_t&right_histGrams);
		//print the basic tree structure
		void printTree();
		//predict the test datas
		inline void predict(const subP_t & testdtset);
		Tree(int max_depth, int min_leave_num,float eta) :max_leave_depth_(max_depth), min_child_num_(min_leave_num), alpha_(eta){};
		~Tree() {};
	};

	void Tree::growTree(std::shared_ptr<Node>&node,const hist_t&histGrams,const subP_t&subsets ){
		hist_t left_histGrams(featureNums);
		hist_t right_histGrams(featureNums);
		subP_t left_subsets;
		subP_t right_subsets;
		getSubset(subsets,left_subsets,right_subsets,node);
		bool growedLeftNode = findBestSplitFeature(left_histGrams,node,left_subsets,1);
		if(growedLeftNode){getHist(histGrams,left_histGrams,right_histGrams);}
		findBestSplitFeature(right_histGrams,node,right_subsets,0);
	}

	inline void Tree::predict(const subP_t & testdtset)
	{
		#pragma omp parallel for
		for (int i = 0; i < testdtset.size(); i++) {
			const auto&data = testdtset[i];
			addValueToScore(data);
		} 
	}

	void Tree::getHist(const hist_t&histGrams,const hist_t&left_histGrams,hist_t&right_histGrams){
		int m = histGrams.size();const Dvector *histgram = histGrams.data(),*left_histgram = left_histGrams.data();Dvector *right_histgram = right_histGrams.data();
		while(m-->0){
			if(!histgram->empty()&& histgram->size()==left_histgram->size()){
				int n = histgram->size(); right_histgram->resize(n); const float *hv = histgram->data(), *lhv = left_histgram->data(); float *rhv = right_histgram->data();
				while(n-->0){
					*rhv = *hv-*lhv;
					rhv++;hv++;lhv++;
				}
			}
			histgram++;left_histgram++;right_histgram++;
		}
	}

	void Tree::getSubset(const subP_t&subsets,subP_t&left_subsets,subP_t&right_subsets,const std::shared_ptr<Node>&node){
		left_subsets.reserve(subsets.size());
		right_subsets.reserve(subsets.size());
		for(auto& data:subsets){
			if(numericDecision(data->features[node->featureIdx],node->threshold)){
				left_subsets.emplace_back(data);
			}else{
				right_subsets.emplace_back(data);
			}
		}
		subP_t(left_subsets).swap(left_subsets);
		subP_t(right_subsets).swap(right_subsets);
	}

	 void Tree::getSubset(const vector<vector<subP_t>>& subsets, subP_t & left_subsets, subP_t & right_subsets, const std::shared_ptr<Node>& node,int datasize)
	{
		 left_subsets.reserve(datasize);
		 right_subsets.reserve(datasize);
		 int fidx = node->featureIdx;
		 int i = 0;
		 for (; i <= node->threshold; i++ ) {
			 left_subsets.insert(left_subsets.end(), subsets[fidx][i].begin(), subsets[fidx][i].end());
			 }
		 for (; i < subsets[fidx][i].size(); i++) {
			 right_subsets.insert(right_subsets.end(), subsets[fidx][i].begin(), subsets[fidx][i].end());
		 }
		 subP_t(left_subsets).swap(left_subsets);
		 subP_t(right_subsets).swap(right_subsets);
	}

	void Tree::train(dataset&dataset, Ivector & binValLen,int subfeatnum){
		binValLen_ = binValLen;
		featureNums = dataset.feature_num;
		treeRoot.reset(new Node);
		if(subfeatnum<featureNums){
			subFeatures.reserve(subfeatnum);
			int featNum=0;
			while(featNum<subfeatnum){
				int feat = rand()% featureNums;
				auto it = find(subFeatures.begin(),subFeatures.end(),feat);
				if(it==subFeatures.end()){subFeatures.push_back(feat);featNum++;}
				}; }
		else{
			subFeatures.resize(featureNums);
			int idx = 0;
			while (idx<featureNums) {
				subFeatures[idx] = idx; idx++;
			}
		} 
		hist_t histGrams(featureNums);
		#pragma omp parallel for
		for (int i = 0; i < subFeatures.size(); i++) {
			int fix = subFeatures[i]; 
			Splits(histGrams[fix], treeRoot, fix, dataset.BinedData,binValLen_[fix] );
		}	 
		hist_t left_histGrams(featureNums);
		hist_t right_histGrams(featureNums);
		subP_t left_subsets;
		subP_t right_subsets;
		getSubset(dataset.FeatBinedData, left_subsets, right_subsets, treeRoot, dataset.BinedData.size());
		bool growedLeftNode = findBestSplitFeature(left_histGrams, treeRoot, left_subsets, 1);
		if (growedLeftNode) { getHist(histGrams, left_histGrams, right_histGrams); }
		findBestSplitFeature(right_histGrams, treeRoot, right_subsets, 0);
	}

	inline void Tree::printTree(){printNode(treeRoot);}
 
	bool Tree::findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets ,const bool left){
		std::shared_ptr<Node>child(new Node);
		child->depth = parent->depth+1;
		if(child->depth>=max_leave_depth_||subsets.size()<=min_child_num_){
			float sumValue = 0.0;
			float sumNT = 0.0;
			for (auto&data:subsets ) {
				sumValue+=data->errLabel;
				sumNT += abs(data->errLabel)*(2 - abs(data->errLabel));
			}
			if (sumNT == 0) { sumNT = 1.0; }
			child->value = (sumValue / sumNT)*alpha_;	 
			child->leaf = true;
			#pragma omp parallel for
			for (int i = 0; i < subsets.size(); i++) {
				auto&data = subsets[i];
				data->score += child->value;
				data->errLabel = getGradient(data->label,data->score);
			}
			if (left) {
				parent->leftChild = child;
			}
			else {
				parent->rightChild = child;
			}
			return false;
		}
	 	#pragma omp parallel for
		for (int i = 0; i < subFeatures.size();i++) {
			int fix = subFeatures[i];
			Splits(histGrams[fix],child,fix,subsets, binValLen_[fix]);
		}	
		if(left){
			parent->leftChild = child;
		}else{
			parent->rightChild = child;
		}	  
		growTree(child,histGrams,subsets );	
		return true;	 
	}
	void Tree::Splits(Dvector &histgram,std::shared_ptr<Node>&child,int fidx,const subP_t&subsets, int bin_size){
		  
		if(histgram.empty())
		{
			histgram.resize(bin_size);
			for(auto &data:subsets){
				int feat_bin = data->features[fidx];
				histgram[feat_bin]+=data->errLabel;
			}
		}
		float nowColmul =0.0;
#pragma omp parallel for
		for(int i=0;i<bin_size;i++){
			nowColmul+=histgram[i];
			if(std::abs(nowColmul)> child->infoGain){
				child->infoGain = std::abs(nowColmul);
				child->threshold = i;
				child->featureIdx = fidx;
			}
		}
	}
}
#endif // !SGBT_TREE_H_
