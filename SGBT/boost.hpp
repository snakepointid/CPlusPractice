#pragma once
#ifndef SGBT_BOOST_H_
#define SGBT_BOOST_H_
#include<memory>
#include<iostream>
#include "dataset.hpp"
#include"tree.hpp"
namespace SGBT {
	using treeP_t = vector<std::unique_ptr<Tree>>;
	using subP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	class boost
	{
	private:
		int max_tree_iter_;
		float alpha_;
		int max_tree_depth_;
		int min_leaves_num_;
		treeP_t treeP;
	public:
		void train(dataset&datasets, int featAddGap=1);
		void predict(subP_t&dataframeP);
		boost(int max_tree_iter,float alpha,int max_tree_depth,int min_leaves_num):
			max_tree_iter_(max_tree_iter), alpha_(alpha), max_tree_depth_(max_tree_depth), min_leaves_num_(min_leaves_num) {};
		~boost() {};
	};

	void SGBT::boost::train(dataset & datasets, int featAddGap )
	{
		treeP.reserve(max_tree_iter_);
		int sampFeatNum = 0;
		for (int i = 0; i < max_tree_iter_; i++) {
			if (i%featAddGap == 0) { sampFeatNum++; }
			treeP.emplace_back(new Tree(max_tree_depth_, min_leaves_num_, alpha_));
			treeP[i]->train(datasets, sampFeatNum);
		}
	}
	inline void boost::predict(subP_t&dataframeP)
	{
		for (auto&tree : treeP) {
			tree->predict(dataframeP);
		}
	}
}
#endif //!SGBT_BOOST_H_