#include <stdlib.h>
#include <stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include <cstring>
#include<sstream>
using namespace std;
string & trim(string &s, char p);
string & trim(string &s);
int main()
{
	//list all of the images
	string inputImgDir = "clean_pictures/";
	char cmdstr[256];
	sprintf_s(cmdstr, "python rotate_image.pyc");
	system(cmdstr);
	sprintf_s(cmdstr, "MD saved");
	system(cmdstr);
	sprintf_s(cmdstr, "cd /d \"%s\" & dir/on/b>../saved/picture_name_list.txt", inputImgDir.data());

	system(cmdstr);
	//get the image file name
	vector<string>imgfns;
	ifstream finpc;
	ifstream fin;
	ofstream fout;
	fout.open("saved/PeopleInfo.txt");
	fout << "contract_no,name,address,id\n";
	finpc.open("saved/picture_name_list.txt");	
	string fn;
	while (getline(finpc, fn))
	{
		//preprocess the image eta. sharpen ,resize,crop the image
		sprintf_s(cmdstr, "magick clean_pictures/%s -sharpen 2 -resize 400x400 -crop 200x60+60+30 -resize 400x400 -sharpen 10 saved/name.png", fn.data());
		system(cmdstr);
		sprintf_s(cmdstr, "magick clean_pictures/%s -sharpen 2 -resize 400x400 -crop 200x80+60+160 -resize 400x400 -sharpen 10 saved/address.png", fn.data());
		system(cmdstr);
		sprintf_s(cmdstr, "magick clean_pictures/%s -sharpen 2 -resize 400x400 -crop 260x60+120+270 -resize 400x400 -sharpen 10 saved/id.png", fn.data());
		system(cmdstr);
		//begin to regonize the info
		sprintf_s(cmdstr, "tesseract saved/name.png saved/name -l chi ");
		system(cmdstr);
		sprintf_s(cmdstr, "tesseract saved/address.png saved/address -l chi ");
		system(cmdstr);
		sprintf_s(cmdstr, "tesseract saved/id.png saved/id -l chi ");
		system(cmdstr);
		//read data
		fin.open("saved/name.txt");
		string name;
		getline(fin, name);
		fin.close();
		fin.open("saved/id.txt");
		string id;
		getline(fin, id);
		fin.close();
		fin.open("saved/address.txt");
		string address;
		string addre;
		while (getline(fin, addre))
		{
			if (addre.empty())break;
			address += addre;
		}
 
		fin.close();

		if (name.empty())name = " ";
		if (address.empty())address = " ";
		if (id.empty())id = " ";
		fout << trim(fn, '.') <<"," << trim(name) << "," << trim(address) << "," << trim(id) << "\n";
	}
	fout.close();
	finpc.close();
}

string & trim(string &s, char p)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(p, index)) != string::npos)
		{
			s.erase(index, 10);
		}
	}
	return s;
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

