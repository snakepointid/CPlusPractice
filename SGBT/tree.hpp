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
	using fmp_t = vector<std::map<float, int> >;

	class Tree
	{
	private:
		struct Node {
			float infoGain = -1.0;
			int depth = 1;
			float value;
			int featureIdx;
			int threshold;
			bool leaf = false;
			std::shared_ptr<Node> leftChild = nullptr;
			std::shared_ptr<Node>rightChild = nullptr;
		};
		int featureNums; 
		int observeNums;
		Ivector subFeatures;
		int max_leave_depth_;
		int min_child_num_;
		std::shared_ptr<Node>root;
		float eta_;
		inline bool numericDecision(int feat,int threshold){
			if(feat<threshold){
				return true;}else{
				return false;}
		}
		inline float speedSigmd(const float f)
		{
			const static int spanNum_ = 1000;
			const static float maxExp_ = 6.0;
			const static Dvector sigmoidTable = [&]()
			{
				Dvector tmpTable(spanNum_);
				for (int i = 0; i < spanNum_; i++)
				{
					float fv = std::exp((float(i) / spanNum_ * 2 - 1)*maxExp_);
					fv = fv / (fv + 1);
					tmpTable[i] = fv;
				}
				return tmpTable;
			}();
			float rf;
			if (f <= -maxExp_) { rf = 0; }
			else if (f >= maxExp_) { rf = 1; }
			else
			{
				int fi = int((f + maxExp_) * (spanNum_ / maxExp_ / 2.0));
				rf = sigmoidTable[fi];

			}
			return rf;
		}
		inline float getResidual(float label, float score,bool exact=false) {
			if (exact) { return 2 * label / (1 + std::exp(2 * label*score)); }
			else {
				return 2 * label / (1 + speedSigmd(2 * label*score));
			}
		}
		inline void addValueToScore(const std::shared_ptr<dataset::oneRowData>&data) {
			std::shared_ptr<Node>tmpNode = root;
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
		inline void printNode(std::shared_ptr<Node>&node) {
			if (node->leaf) { printf("leave value: %.4f\n", node->value); }
			else {
				printf("feature: %lu,Bin: %lu\n", node->featureIdx, node->threshold);
			}	
			if (node->leftChild) {
				printNode(node->leftChild);
			}
			if(node->rightChild) {
				printNode(node->rightChild);
			}
		}
		
	public:
		bool findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets,const fmp_t&featureValueSpan,const bool left);
		void Splits(Dvector &histgram,std::shared_ptr<Node>&child,const int fidx,const subP_t&subsets,const fmp_t&featureValueSpan);
		void train(dataset& dataset,int subfeatnum); 
		void printTree();
		void getSubset(const subP_t&subsets,subP_t&left_subsets,subP_t&right_subsets,const std::shared_ptr<Node>&node);
		void getHist(const hist_t&histGrams,const hist_t&left_histGrams,hist_t&right_histGrams);
		void growTree(std::shared_ptr<Node>&node,const hist_t&histGrams,const subP_t&subsets,const fmp_t&featureValueSpan);
		inline void Tree::predict(const subP_t & testdtset);
		Tree(int max_depth, int min_leave_num,float eta) :max_leave_depth_(max_depth), min_child_num_(min_leave_num), eta_(eta){};
		~Tree() {};
	};

	void Tree::growTree(std::shared_ptr<Node>&node,const hist_t&histGrams,const subP_t&subsets,const fmp_t&featureValueSpan){
		hist_t left_histGrams(featureNums);
		hist_t right_histGrams(featureNums);
		subP_t left_subsets;
		subP_t right_subsets;
		getSubset(subsets,left_subsets,right_subsets,node);
		bool growedLeft = findBestSplitFeature(left_histGrams,node,left_subsets,featureValueSpan,1);
		if(growedLeft){getHist(histGrams,left_histGrams,right_histGrams);}
		findBestSplitFeature(right_histGrams,node,right_subsets,featureValueSpan,0);
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

	void Tree::train(dataset&dataset,int subfeatnum){
		featureNums = dataset.feature_num;
		root.reset(new Node);
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
			Splits(histGrams[fix], root, fix, dataset.dataframeP, dataset.featureValueSpan);
		}	 
		growTree(root,histGrams,dataset.dataframeP,dataset.featureValueSpan);	 
	}

	inline void Tree::printTree(){printNode(root);}
 
	bool Tree::findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets,const fmp_t&featureValueSpan,const bool left){
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
			child->value = (sumValue / sumNT)*eta_;	 
			child->leaf = true;
			#pragma omp parallel for
			for (int i = 0; i < subsets.size(); i++) {
				auto&data = subsets[i];
				data->score += child->value;
				data->errLabel = getResidual(data->label,data->score);
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
			Splits(histGrams[fix],child,fix,subsets,featureValueSpan);
		}	
		if(left){
			parent->leftChild = child;
		}else{
			parent->rightChild = child;
		}	  
		growTree(child,histGrams,subsets,featureValueSpan);	
		return true;	 
	}
	void Tree::Splits(Dvector &histgram,std::shared_ptr<Node>&child,int fidx,const subP_t&subsets,const fmp_t&featureValueSpan){
		int bin_size = featureValueSpan[fidx].size();
		if(histgram.empty())
		{
			histgram.resize(bin_size);
			for(auto &data:subsets){
				int feat_bin = data->features[fidx];
				histgram[feat_bin]+=data->errLabel;
			}
		}
		float nowColmul =0.0;
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
