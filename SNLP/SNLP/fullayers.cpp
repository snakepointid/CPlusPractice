#include "neuron/fullayers.h"
namespace SNLP {
	void fullayer::_initMatrix()
	{
		initialMatrix(1, Weights_, outputDim_, inputDim_);
		initialMatrix(1, Bias_, outputDim_);
		initialMatrix(0, WeightsHolder_, outputDim_, inputDim_);
		initialMatrix(0, BiasHolder_, outputDim_);
	}

	Fvector fullayer::actNeuron(const Fvector & inputs)
	{
		return  PairWiseADD(dot(inputs, Weights_), Bias_);
	}

	Fvector fullayer::saveGradient(const Fvector & outputError, const Fvector & inputs)
	{
		saxpy(WeightsHolder_, 1, dot(mtp(outputError), mtp(inputs)));
		saxpy(BiasHolder_, 1, SUM(outputError, 1));
		return dot(outputError, mtp(Weights_));
	}

	void fullayer::updateWeight(float alpha, float l1)
	{
		saxpy(Weights_, alpha, WeightsHolder_);
		saxpy(Bias_, alpha, BiasHolder_);
		if (l1 > 0)
		{
			saxpy(Weights_, -l1);
			saxpy(Bias_, -l1);
		}
		//set holder to 0 again
		WeightsHolder_.clear();
		BiasHolder_.clear();
		initialMatrix(0, WeightsHolder_, outputDim_, inputDim_);
		initialMatrix(0, BiasHolder_, outputDim_);
	}

	Fvector fullayer::updateWeight(Fvector & outputError, const Fvector & inputs, float alpha, float l1)
	{
		Fvector inputError;
		inputError = dot(outputError, mtp(Weights_));
		saxpy(Weights_, alpha, dot(mtp(outputError), mtp(inputs)));
		saxpy(Bias_, alpha, SUM(outputError, 1));
		if (l1 > 0)
		{
			saxpy(Weights_, -l1);
			saxpy(Bias_, -l1);
		}
		return inputError;
	}

}
