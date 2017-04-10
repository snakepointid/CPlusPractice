#ifndef FULLAYERS_H_
#define FULLAYERS_H_
#include "../common/meta.h"
#include"../common/matrInit.h"
#include"../common/pointWise.h"
#include"../common/dot.h"
namespace SNLP {
	struct fullayer
	{
		//controler
	private:
		int inputDim_ = 0;
		int outputDim_ = 0;
		//constructor
		fullayer(int outputDim, int inputDim) :inputDim_(inputDim), outputDim_(outputDim) { _initMatrix(); }
		//member
		///weights
		vector<Fvector> Weights_;
		Fvector         Bias_;
		///gradient holder
		vector<Fvector> WeightsHolder_;
		Fvector         BiasHolder_;
		//initial member : first parameter set 1 means random initial 0 means set to 0
		void _initMatrix();
	public:
		//funcs
		Fvector actNeuron(const Fvector &inputs);
		Fvector saveGradient(const Fvector &outputError, const Fvector &inputs);
		void updateWeight(float alpha, float l1);
		Fvector updateWeight(Fvector &outputError, const Fvector &inputs, float alpha, float l1);
	};
}
#endif // !FULLAYERS_H_
