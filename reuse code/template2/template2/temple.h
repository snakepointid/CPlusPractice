 #pragma once
#include<iostream>
using std::cout;
using std::endl;
using std::cin;
#ifndef TEMPLE2_H_
#define TEMPLE2_H_
template<class T, int n>
class base
{
private:
	T arr[n];
public:
	base(T &ar) :arr(ar) {}
	base() {}
	~base() {}
	void setArr();
	void show();
	//operator
	T &operator[](int n);
	T operator[](int n)const;
};
#endif // !TEMPLE2_H_

template<class T, int n>
inline void base<T, n>::setArr()
{
	if (n > 0)
	{
		cout << "please enter your number: \n";
		for (int i = 0; i < n; i++)
			cin >> arr[i];
	}
	cout << '\n';
	while (cin.get() != '\n')
		continue;
	cout << '\n';
}
template<class T, int n>
inline void base<T, n>::show()
{
	for (int i = 0; i < n; i++)
		cout << arr[i]<<' ';
}

template<class T, int n>
inline T & base<T, n>::operator[](int n)
{
	return arr[n];
}

template<class T, int n>
inline T base<T, n>::operator[](int n) const
{
	return arr[n];
}

