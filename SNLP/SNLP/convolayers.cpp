#include"neuron\convolayers.h"
namespace SNLP {
	void convoLayer::_initMatrix()
	{
		initialMatrix(1, Weights_, outputDim_, window_, inputDim_);
		initialMatrix(1, Bias_, outputDim_);
		initialMatrix(0, WeightsHolder_, outputDim_, window_, inputDim_);
		initialMatrix(0, BiasHolder_, outputDim_);
	}
	vector<Fvector> convoLayer::actNeuron(const vector<Fvector>& inputs)
	{
		return  PairWiseADD(mdot(inputs, Weights_, window_), Bias_);
	}
	vector<Fvector> convoLayer::saveGradient(vector<Fvector>& outputError, const vector<Fvector>& inputs)
	{
		saxpy(WeightsHolder_, 1, mouter(outputError, inputs, window_));
		saxpy(BiasHolder_, 1, SUM(outputError, 1));
		return  mouter(outputError, Weights_, window_);
	}
	void convoLayer::updateWeight(float alpha, float l1)
	{
		saxpy(Weights_, alpha, WeightsHolder_);
		saxpy(Bias_, alpha, BiasHolder_);
		if (l1 > 0)
		{
			saxpy(Weights_, -l1);
			saxpy(Bias_, -l1);
		}
		WeightsHolder_.clear();
		BiasHolder_.clear();
		initialMatrix(0, WeightsHolder_, outputDim_, window_, inputDim_);
		initialMatrix(0, BiasHolder_, outputDim_);
	}
	vector<Fvector> convoLayer::updateWeight(vector<Fvector>& outputError, const vector<Fvector> inputs, float alpha, float l1)
	{
		vector<Fvector> inputError;
		inputError = mouter(outputError, Weights_, window_);
		saxpy(Weights_, alpha, mouter(outputError, inputs, window_));
		saxpy(Bias_, alpha, SUM(outputError, 1));
		if (l1 > 0)
		{
			saxpy(Weights_, -l1);
			saxpy(Bias_, -l1);
		}
		return inputError;
	}
}