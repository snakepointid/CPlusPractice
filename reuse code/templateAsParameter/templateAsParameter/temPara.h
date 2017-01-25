#include<iostream>
using std::cout;
using std::endl;
#ifndef TEMPARA_H_
#define TEMPARA_H_
template<class T>
class base
{
private:
	T item;
	static int ct;
public:
	base(const T &ip) :item(ip) { ct++; }
	~base() { ct--; }
	//friend method
	friend void counts();
	friend void report(base<T>&);
};
#endif // !TEMPARA_H_
template<class T>
int base<T>::ct = 0;
void counts()
{
	cout << "int type object numbers: " << base<int>::ct << '\n';
	cout << "double type object numbers: " << base<double>::ct << '\n';
}

void report(base<int>& b)
{
	cout << "int type object item is : " << b.item << '\n';
}
void report(base<double>& b)
{
	cout << "double type object item is : " << b.item << '\n';
}
