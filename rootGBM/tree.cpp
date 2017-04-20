#include"include/tree.h"
#include<iostream>
namespace rootGBM {
	using dataP_t = vector<std::shared_ptr<dataset::oneRowData> >;
	void Tree::growRoot(const vector<dataP_t>& subsets)
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
		for (int i = treeRoot->threshold + 1; i <subsets.size(); i++) {
			for (auto&data : subsets[i]) {
				sumValue += data->errLabel;
				sumNT += abs(data->errLabel)*(2 - abs(data->errLabel));
			}
		}
		if (sumNT == 0) { sumNT = 1.0; }
		treeRoot->rightValue = (sumValue / sumNT)*alpha_;
#pragma omp parallel for
		for (int i = treeRoot->threshold + 1; i <subsets.size(); i++) {
			for (auto&data : subsets[i]) {
				data->score += treeRoot->rightValue;
				data->errLabel = getGradient(data->label, data->score);
			}
		}
	}
 void Tree::predict(const dataP_t & testdtset)
	{
#pragma omp parallel for
		for (int i = 0; i < testdtset.size(); i++) {
			const auto&data = testdtset[i];
			addValueToScore(data);
		}
	}
	// not support deep tree struct now,only use binary tree,this function is useless for binary tree
	void Tree::train(dataset*dataset, const Ivector & binValLen, int subfeatnum) {
		treeRoot.reset(new Node);
		_getSubFeature(dataset->feature_num, subfeatnum);
		int groupID = rand() % dataset->randGroupNum;
#pragma omp parallel for
		for (int i = 0; i < subFeatures.size(); i++) {
			int fix = subFeatures[i];
			Splits(fix, dataset->BinedData, dataset->dataRandGroup[groupID], binValLen[fix]);
		}
 
		growRoot(dataset->FeatBinedData[treeRoot->featureIdx]);
	}


	void Tree::Splits(int fidx, const dataP_t&subsets, Ivector&randDataGroup, int bin_size) {
		Dvector histgram(bin_size);
#pragma omp parallel for
		for (int i = 0; i < randDataGroup.size(); i++) {
			auto &data = subsets[randDataGroup[i]];
			int feat_bin = data->features[fidx];
			histgram[feat_bin] += data->errLabel;
		}

		float nowColmul = 0.0;
		for (int i = 0; i<bin_size; i++) {
			nowColmul += histgram[i];
			if (std::abs(nowColmul)> treeRoot->infoGain) {
				treeRoot->infoGain = std::abs(nowColmul);
				treeRoot->threshold = i;
				treeRoot->featureIdx = fidx;
			}
		}
	}
	inline void Tree::_getSubFeature(int featNum, int subFeatNum)
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