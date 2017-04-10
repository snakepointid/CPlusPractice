#ifndef DOT_H_
#define DOT_H_
 
#include"meta.h"
#include"aggregate.h"
#include"mapFunc.h"
 
namespace SNLP {
	//dot 
	float dot(const Fvector &x, const Fvector &y);
	vector<Fvector> dot(const vector<Fvector> &x, const vector<Fvector> &y);
	Fvector dot(const Fvector &x, const vector<Fvector> &y);
	vector<Fvector> dot(const vector<Fvector> &x, const Fvector &y);

//normalize
	void norml2(Fvector &x);
	void norml1(Fvector &x);
	float normdot(const Fvector &x, const Fvector &y);

	//convolutional dot
	float mdot(const vector<Fvector> &x, const vector<Fvector> &y);
	Fvector mdot(const vector<Fvector> &x, const vector<vector<Fvector>> &y);
	vector<Fvector> mdot(const vector<vector<Fvector>> &x, const vector<vector<Fvector>> &y);
	//convolution
	vector<Fvector> mdot(const vector<Fvector> &x, const vector<vector<Fvector>> &y, int window);
	//out dot
	vector<Fvector> mouter(const float &x, const vector<Fvector> &y);
	vector<Fvector> mouter(const Fvector &x, const vector<vector<Fvector>> &y);
	vector<vector<Fvector>> mouter(const Fvector &x, const vector<Fvector> &y);
	vector<vector<Fvector>> mouter(const vector<Fvector> &x, const vector<Fvector> &y, int window);
	vector<Fvector> mouter(const vector<Fvector> &x, const vector<vector<Fvector>> &y, int window);
}
 
#endif // !DOT_H_
