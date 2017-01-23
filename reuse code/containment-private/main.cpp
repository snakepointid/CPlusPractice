#include<iostream>
#include "studentc.h"
using std::cout;
using std::endl;
using namespace cont;
void set(student &stu, int n);
const int pupils = 3;
const int lectures = 5;
void main()
 {
	int i;
	student students[pupils] = { student(lectures) , student(lectures) , student(lectures) };
	for (i = 0; i < pupils; i++)
		set(students[i], lectures);
	cout << "\nStudent List:\n";
	for (i = 0; i < pupils; ++i)
		cout << students[i].getName() << endl;
	cout << "\nResults:";
	cout << "below display this student's information\n";
	for (i = 0; i < pupils; ++i)
	{
		cout << endl << students[i];
		cout << "average: " << students[i].average() << endl;
	}
	cout << "Done.\n";
 
 }

void set(student & stu, int n)
{
 
	getline(cin, stu);
	cout << "please enter "<<n<<" quiz scores:\n";
	for (int i = 0; i < n; i++)
		cin >> stu[i];
	while (cin.get() != '\n')
		continue;

}
