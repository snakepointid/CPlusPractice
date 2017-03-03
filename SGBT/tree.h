#pragma once
#ifndef SGBT_TREE_H_
#define SGBT_TREE_H_
#include"meta.h"
namespace SGBT {
	class Tree
	{
	public:
		void Split(bool left);
		void train(vector<Dvector*>dataset);

	private:
		Ivector left_child_;
		Ivector right_child_;

	};
}
#endif // !SGBT_TREE_H_
