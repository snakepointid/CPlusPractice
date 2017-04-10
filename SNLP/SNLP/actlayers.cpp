#include "neuron/actlayers.h"
namespace SNLP {
	void actLayer::_initialACT()
	{
		if (actor_ == "sigmoid") { active = speedSigmd; gradtive = speedSigmdGrad; }
		else if (actor_ == "tanh") { active = speedTanh; gradtive = speedTanhGrad; }
		else if (actor_ == "relu") { active = relu; gradtive = reluGrad; }
		else if (actor_ == "noact") {}
		else { printf("your act type is not exsit,eta.sigmoid,tanh,relu\n"); abort(); }
	}
	Fvector actLayer::actNeuron(const Fvector & inputs)
	{
		if (actor_ == "noact")return inputs;
		return  MAP(inputs, active);
	}
	Fvector actLayer::updateWeight(const Fvector & outputError, const Fvector & inputs)
	{
		if (actor_ == "noact")return outputError;
		return  PairWiseMulti(outputError, MAP(inputs, gradtive));
	}
}
