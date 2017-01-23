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
namespace cont
{
	class student
	{
	private:
		typedef valarray<double> arryDB;
		string name;
		arryDB scores;
		//private method
		ostream &arr_out(ostream &os)const;
	public:
		//constructor and deconstructor
		student() :name("None of name"), scores() {}
		explicit student(const string &na = "None of name") :name(na), scores() {}
		explicit student(int n) :name("None of name"), scores(n) {}
		student(const string &na = "None of name", int n = 0) :name(na), scores(n) {}
		student(const string &na, arryDB &adb) :name(na), scores(adb) {}
		student(const string &na = "None of name", const double val = 0, int n = 0) :name(na), scores(val, n) {}
		~student() {}
		//method
		double average()const;
		const string &getName()const { return name; }
		//member operator
		double &operator[](int n) { return scores[n]; }
		double operator[](int n)const { return scores[n]; }
		//friend operator
		friend const istream&operator >> (istream&is, student &stu);
		friend const istream&getline(istream&is, student &stu);
		friend const ostream&operator << (ostream&os, student &stu);

	};
}

#endif // !STUDENT_H_
