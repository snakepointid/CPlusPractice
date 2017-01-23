#pragma once
#include<string>
#include<iostream>
#include<valarray>
#include<iostream>
using std::string;
using std::valarray;
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
#ifndef STUDENT_H_
#define STUDENT_H_
namespace prit
{
	class student :private string, private valarray<double>
	{
	private:
		typedef valarray<double> arryDB;
		//private method
		ostream &arr_out(ostream &os)const;
	public:
		//constructor and deconstructor
		student() :string("None of string"), arryDB() {}
		explicit student(const string &na = "None of string") :string(na), arryDB() {}
		explicit student(int n) :string("None of string"), arryDB(n) {}
		student(const string &na = "None of string", int n = 0) :string(na), arryDB(n) {}
		student(const string &na, arryDB &adb) :string(na), arryDB(adb) {}
		student(const string &na = "None of string", const double val = 0, int n = 0) :string(na), arryDB(val, n) {}
		~student() {}
		//method
		double average()const;
		const string &getName()const { return (const string &) *this; }
		//member operator
		double &operator[](int n) { return arryDB::operator[](n); }
		double operator[](int n)const { return arryDB::operator[](n); }
		//friend operator
		friend const istream&operator >> (istream&is, student &stu);
		friend const istream&getline(istream&is, student &stu);
		friend const ostream&operator << (ostream&os, student &stu);

	};
}

#endif // !STUDENT_H_
