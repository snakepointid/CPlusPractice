#pragma once
#ifndef SGBT_TREE_H_
#define SGBT_TREE_H_
#include"meta.h"
#include<algorithm>
#include"dataset.hpp"
#include<memory>
namespace SGBT {
	using subP_t = vector<std::unique_ptr<dataset::oneRowData> >;
	using hist_t = vector<Dvector>;
	using fmp_t = vector<std::map<float, int> >;
	class Tree
	{
	private:
		int featureNums; 
		int observeNums;
		Ivector subFeatures;
		int max_leave_depth;
		int min_child_num;
		std::unique_ptr<dataset>dtset;
		struct Node{
			float infoGain=-1.0;
			int depth=1;
			float value;
			int featureIdx;
			int threshold;
			std::shared_ptr<Node> leftChild = nullptr;
			std::shared_ptr<Node>rightChild = nullptr;
		};
		inline bool numericDecision(int feat,int threshold){
			if(feat<threshold){
				return true;}else{
				return false;}
		}
	public:
		bool findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets,const fmp_t&featureValueSpan,const bool left);
		void Splits(Dvector &histgram,std::shared_ptr<Node>&child,const int fidx,const subP_t&subsets,const fmp_t&featureValueSpan);
		void train(dataset& dataset,int subfeatnum); 
		void getSubset(const subP_t&subsets,subP_t&left_subsets,subP_t&right_subsets,const std::shared_ptr<Node>&node);
		void getHist(const hist_t&histGrams,const hist_t&left_histGrams,hist_t&right_histGrams);
		void growTree(std::shared_ptr<Node>&node,const hist_t&histGrams,const subP_t&subsets,const fmp_t&featureValueSpan);
		 
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
	void Tree::getHist(const hist_t&histGrams,const hist_t&left_histGrams,hist_t&right_histGrams){

		int m = histGrams.size();const Dvector *histgram = histGrams.data(),*left_histgram = left_histGrams.data();Dvector *right_histgram = right_histGrams.data();
		while(m-->0){
			if(!histgram->empty()){
				int n = histgram->size();const float *hv = histgram->data(),*lhv = left_histgram->data();float *rhv = right_histgram->data();
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
		for(auto&data:subsets){
			if(numericDecision(data->features[node->featureIdx],node->threshold)){
				left_subsets.push_back(data);
			}else{
				right_subsets.push_back(data);
			}
		}
		subP_t(left_subsets).swap(left_subsets);
		subP_t(right_subsets).swap(right_subsets);
	}

	void Tree::train(dataset&dataset,int subfeatnum){
		featureNums = dataset.feature_num;
		observeNums = dataset.observe_num;
		
		std::shared_ptr<Node>root(new Node);
		if(subfeatnum<featureNums){
			subFeatures.reserve(subfeatnum);
			int featNum=0;
			while(featNum<subfeatnum){
				int feat = rand()% featureNums;
				auto it = find(subFeatures.begin(),subFeatures.end(),feat);
				if(it==subFeatures.end()){subFeatures.push_back(feat);featNum++;}
				}; }else{
					while(subfeatnum)
				}
		
		 
		hist_t histGrams(featureNums);
		for(int fix:subFeatures){
				Splits(histGrams[fix],root,fix,dataset.dataframeP,dataset.featureValueSpan);
			}
		growTree(root,histGrams,dataset.dataframeP,dataset.featureValueSpan);	 
	}
 
	bool Tree::findBestSplitFeature(hist_t&histGrams,std::shared_ptr<Node>&parent,const subP_t&subsets,const fmp_t&featureValueSpan,const bool left){
		std::shared_ptr<Node>child(new Node);
		child->depth = parent->depth+1;
		if(child->depth>=max_leave_depth||subsets.size()<=min_child_num){
			float sumValue = 0.0;
			for(auto&data:subsets){
				sumValue+=data->label;
			}
			child->value = sumValue/subsets.size();
			return false;
		}
		for(int&fix:subFeatures){
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
				histgram[feat_bin]+=data->label;
			}
		}
		float nowColmul =0.0;
		float best =-1.0;
		int bestBinIdx=0;
		for(int i=0;i<bin_size;i++){
			nowColmul+=histgram[i];
			if(std::abs(nowColmul)>best){
				best = nowColmul;
				bestBinIdx = i;
			}
		}
		if(std::abs(best)>child->infoGain){
			child->infoGain = std::abs(best);
			child->threshold = bestBinIdx;
			child->featureIdx = fidx;
		}
	}
}
#endif // !SGBT_TREE_H_
