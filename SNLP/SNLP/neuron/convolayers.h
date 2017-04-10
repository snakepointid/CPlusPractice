#ifndef CONVOLAYERS_H_
#define CONVOLAYERS_H_
#include"../common/meta.h"
#include"../common/dot.h"
#include"../common/matrInit.h"
#include"../common/pointWise.h"
namespace SNLP {
	struct convoLayer
	{
		//controler
	private:
		int inputDim_;
		int outputDim_;
		int window_;
		//constructor
		convoLayer(int outputDim, int window, int inputDim) :inputDim_(inputDim), outputDim_(outputDim), window_(window) { _initMatrix(); }
		//member
		///weights
		vector<vector<Fvector>> Weights_;
		Fvector				   Bias_;
		///gradient holder
		vector<vector<Fvector>> WeightsHolder_;
		Fvector                 BiasHolder_;
		//funcs
		void _initMatrix();
	public:
		vector<Fvector> actNeuron(const vector<Fvector> &inputs);
		vector<Fvector>  saveGradient(vector<Fvector> &outputError, const vector<Fvector> &inputs);
		void updateWeight(float alpha, float l1);	
		vector<Fvector> updateWeight(vector<Fvector> &outputError, const vector<Fvector> inputs, float alpha, float l1);	
	};
}
#endif // !CONVOLAYERS_H_
