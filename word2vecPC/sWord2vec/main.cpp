#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <chrono>
#include "word2vec.h" 
using namespace std;
vector<string>splits(const string&s, char dim, vector<string>&elements);
vector<string>splits(const string&s, char dim);
int main()
{
	string fn = "test.txt";

	std::ifstream fin;
	fin.open(fn);
	string line;
	string word;
	int count = 0;
	vector<SW2V::SentenceP> sentences;
	SW2V::SentenceP sentence(new SW2V::Sentence);
	string s;
	/*auto cstart = std::chrono::high_resolution_clock::now();
	while (std::getline(fin, line))
	{
	SW2V::SentenceP sentence(new SW2V::Sentence);
	sentence->tokens.swap(splits(line, ' '));
	sentences.push_back(std::move(sentence));
	}
	cout << "have " << sentences.size() << " 's sentences\n";
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);*/
	auto cstart = std::chrono::high_resolution_clock::now();
	while (1)
	{
		fin >> s;
		if (s.empty())break;
		sentence->tokens.push_back(move(s));
		count++;
		if (count == 10)
		{
			count = 0;
			if (!sentence->tokens.empty())sentences.push_back(std::move(sentence));
			sentence.reset(new SW2V::Sentence);
		}

	}
	cout << "have " << sentences.size() << " 's sentences\n";
	auto cend = std::chrono::high_resolution_clock::now();
	printf("load vocab: %.4f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0);


}
vector<string> splits(const string & s, char dim, vector<string>& elements)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, dim))
		elements.push_back(item);
	return elements;
}

vector<string> splits(const string & s, char dim)
{
	vector<string> elements;
	return splits(s, dim, elements);
}
