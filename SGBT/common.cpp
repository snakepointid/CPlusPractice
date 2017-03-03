#include "common.h"
#include<iostream>
std::vector<std::string> SGBT::splits(const std::string & s, char dim, std::vector<std::string>& elements)
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

std::vector<std::string> SGBT::splits(const std::string & s, char dim)
{
	std::vector<std::string> elements;
	return splits(s, dim, elements);
}


