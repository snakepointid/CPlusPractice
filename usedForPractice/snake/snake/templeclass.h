#pragma once
#include<iostream>
using std::cout;
using std::endl;
#ifndef TEMPLECLASS_H_
#define TEMPLECLASS_H_
template<class Type>
class base
{
private:
	enum{MAX=10};
	Type items[MAX];
	int top;
public:
	base(); 
	bool isEmpty();
	bool isFull();
	bool push(const Type &item);
	bool pop(Type &item);
	void show();
};
#endif // !TEMPLECLASS_H_

template<class Type>
inline base<Type>::base()
{
	top = 0;
}

template<class Type>
inline bool base<Type>::isEmpty()
{
	return top == 0;
}

template<class Type>
inline bool base<Type>::isFull()
{
	return top==MAX;
}

template<class Type>
inline bool base<Type>::push(const Type & item)
{
	if (top < MAX)
	{
		items[top++] = item;
		return true;
	}
	else
	{
		cout << "the items is full,can't add more item\n";
		return false;
	}
}

template<class Type>
inline bool base<Type>::pop(Type & item)
{
	if (top > 0)
	{
		item = items[--top];
		return true;
	}
	else
	{
		cout << "the items is empty,can't drop any item\n";
		return false;
	}
		
}

template<class Type>
inline void base<Type>::show()
{
	for (int i = 0; i < MAX; i++)
		cout << items[i] << ' ';
}
