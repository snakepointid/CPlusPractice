#ifndef SGBT_COMMON_H_
#define SGBT_COMMON_H_
#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<cmath>
#include"meta.h"
using std::vector;
using std::string;

namespace SGBT {
	std::vector<std::string> splits(const std::string & s, char dim, std::vector<std::string>& elements)
	{
		{
			std::stringstream ss(s);
			std::string item;
			while (getline(ss, item, dim))
			{
				if (item.empty())break;
				elements.push_back(move(item));
			}
			return elements;
		}
	}

	std::vector<std::string> splits(const std::string & s, char dim)
	{
		std::vector<std::string> elements;
		return splits(s, dim, elements);
	}
	inline Dvector ATOF(vector<string>& svec)
	{
		int m = svec.size();
		Dvector dvec(m); float *v = dvec.data(); string *s = svec.data();
		while (m-- > 0) {
			*v = atof(s->c_str()); v++; s++;
		}
		return dvec;
	}

	inline Dvector ATOF(const std::string & s, char dim)
	{
		Dvector elements;
		std::stringstream ss(s);
		std::string item;
		while (getline(ss, item, dim))
		{
			if (item.empty())break;
			elements.push_back(atof(item.c_str()));
		}
		return elements;
	}

	template<typename T>
	inline void showVec(const T & v)
	{
		int m = v.size(); const auto*vp = v.data();
		while (m-- > 0) { showVec(*vp++); }
		std::cout << std::endl;


	}
	template<>inline void showVec(const Dvector& v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
	template<>inline void showVec(const Ivector & v)
	{
		for (auto&vv : v) { std::cout << vv << " "; }
		std::cout << std::endl;
	}
}
#endif // !SGBT_COMMON_H_
