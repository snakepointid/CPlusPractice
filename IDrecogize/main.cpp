#include <stdlib.h>
#include <stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include <cstring>
#include<sstream>
using namespace std;
string & trim(string &s);
int main()
{
	ifstream fin("director.txt");
	//list all of the images
	string tessDir;
	string inputImgDir;
	getline(fin, tessDir);
	getline(fin, inputImgDir);
	fin.close();
	if (tessDir.empty() | inputImgDir.empty()) { printf("can not find the director.txt \n"); char c; cin >> c; return 1; }
	string outDir = inputImgDir+"saved\\";
	char cmdstr[256];
	
	sprintf_s(cmdstr, "cd /d \"%s\" & dir/on/b>list.txt", inputImgDir.data());
	system(cmdstr);
	sprintf_s(cmdstr, "MD %ssaved", inputImgDir.data());
	system(cmdstr);
	//get the image file name
	vector<string>imgfns;
	fin.open(inputImgDir+"list.txt");
	string imgfn;
	while (getline(fin, imgfn))
	{
		if (imgfn == "list.txt")continue;
		imgfns.push_back(move(imgfn));
	}
	fin.close();
	//image preprocess


	ofstream fout;
	fout.open(outDir+"PeopleInfo.txt");
	fout << "name,address,id\n";
	for (auto&fn : imgfns)
	{
		//preprocess the image eta. sharpen ,resize,crop the image
		sprintf_s(cmdstr, "magick %s%s -sharpen 2 -resize 400x400 -crop 200x60+70+30 -resize 400x400 -sharpen 10 %sname.jpg", inputImgDir.data(), fn.data(), outDir.data());
		system(cmdstr);
		sprintf_s(cmdstr, "magick %s%s -sharpen 2 -resize 400x400 -crop 200x80+70+160 -resize 400x400 -sharpen 10 %saddress.jpg", inputImgDir.data(), fn.data(), outDir.data());
		system(cmdstr);
		sprintf_s(cmdstr, "magick %s%s -sharpen 2 -resize 400x400 -crop 260x50+130+270 -resize 400x400 -sharpen 10 %sid.jpg", inputImgDir.data(), fn.data(), outDir.data());
		system(cmdstr);
		//begin to regonize the info
		sprintf_s(cmdstr, "cd /d \"%s\" & tesseract %sname.jpg %sname -l chi ", tessDir.data(), outDir.data(), outDir.data());
		system(cmdstr);
		sprintf_s(cmdstr, "cd /d \"%s\" & tesseract %saddress.jpg %saddress -l chi ", tessDir.data(), outDir.data(), outDir.data());
		system(cmdstr);
		sprintf_s(cmdstr, "cd /d \"%s\" & tesseract %sid.jpg %sid -l chi ", tessDir.data(), outDir.data(), outDir.data());
		system(cmdstr);
		//read data
		fin.open(outDir + "name.txt");
		string name;
		getline(fin, name);
		fin.close();
		fin.open(outDir + "id.txt");
		string id;
		getline(fin, id);
		fin.close();
		fin.open(outDir + "address.txt");
		string address;
		string addre;
		while (getline(fin, addre))
		{
			if(addre.empty())break;
			address += addre;
		}
		fin.close();
	 
		if (name.empty())name = "NULL";
		if (address.empty())address = "NULL";
		if (id.empty())id = "NULL";
		fout << trim(name) << "," << trim(address) << "," << trim(id) << "\n";
	}
	fout.close();
}

string & trim(string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
	return s;
}

 