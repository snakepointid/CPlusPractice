#pragma once
#ifndef SGBT_BOOST_H_
#define SGBT_BOOST_H_
#include<memory>
#include<iostream>
#include<omp.h>
#include "dataset.h"
#include "tree.h"
#include "Bin.h"
 
namespace rootGBM {
	using treeP_t = vector<std::unique_ptr<Tree>>;
	using dataP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	typedef float(*NegLikeGrad)(float label, float score);
	class boost
	{
	private:
		int max_tree_iter_;
		float alpha_;
		treeP_t treeP;
	public:
		void train(dataset*datasets, bin*bin ,int exactMode=0);
		void predict(dataset*datasets);
		void initDataset(dataset*datasets);
		explicit boost(int max_tree_iter,float alpha,int thread_num):
			max_tree_iter_(max_tree_iter), alpha_(alpha) {
			if (thread_num > 0) {
				omp_set_num_threads(thread_num);
			}
		};
	};

	
	 
}
#endif //!SGBT_BOOST_H_