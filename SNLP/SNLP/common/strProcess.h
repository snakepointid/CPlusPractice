#ifndef STRPROCESS_H_
#define STRPROCESS_H_
#include"meta.h"
#include<algorithm>
#include<sstream>
using std::stringstream;
namespace SNLP {
	inline string& lowerTokens(string&token)
	{
		transform(token.begin(), token.end(), token.begin(), tolower);
		return token;
	}
	inline string& upperTokens(string&token)
	{
		transform(token.begin(), token.end(), token.begin(), toupper);
		return token;
	}
	inline  vector< string> splits(const  string & s, char dim,  vector< string>& elements)
	{
		{
			stringstream ss(s);
			string item;
			while (getline(ss, item, dim))
			{
				if (item.empty())break;
				elements.emplace_back(item);
			}
			return elements;
		}
	}

	inline  vector< string> splits(const  string & s, char dim)
	{
		 vector< string> elements;
		return splits(s, dim, elements);
	}
}
#endif // !STRPROCESS_H_
