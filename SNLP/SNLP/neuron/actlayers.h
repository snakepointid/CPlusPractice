#ifndef ACTLAYERS_H_
#define ACTLAYERS_H_
#include "../common/meta.h"
#include"../common/speedFunc.h"
#include"../common/mapFunc.h"
#include"../common/pointWise.h"
namespace SNLP {
	struct actLayer
	{
	private:
		typedef float(*ActiveP)(float f);
		typedef float(*GradtiveP)(float f);
		//controler
		string actor_;
		ActiveP active;
		GradtiveP gradtive;
		//constructor
		actLayer(const string&actor) :actor_(actor) { _initialACT(); };
		//functions
		void _initialACT();
	public:
		Fvector actNeuron(const Fvector &inputs); 
		Fvector updateWeight(const Fvector &outputError, const Fvector &inputs);	
	};
}
#endif // !ACTLAYERS_H_
