#pragma once
#include<iostream>
using std::cout;
using std::endl;
using std::cin;
#ifndef TRYCATCH_H_
#define TRYCATCH_H_
class bad_hmean
{
private:
	double x;
	double y;
public:
	bad_hmean(double xx, double yy) :x(xx), y(yy) {}
	void msg();
};
class bad_gmean
{
private:
	double x;
	double y;
public:
	bad_gmean(double xx, double yy) :x(xx), y(yy) {}
	void msg();
};

void bad_hmean::msg()
{
	cout << "the x is£º" << x << " the y is£º" << y << "\nthe two number can not be opposite\n";
}

void bad_gmean::msg()
{
	cout << "the x is£º" << x << " the y is£º" << y << "\nneither of the two number can be negtive\n";
}

#endif // !TRYCATCH_H_
