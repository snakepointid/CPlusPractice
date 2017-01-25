#include "friendclass.h"
void main()
{
	tv myTV;
	myTV.settings();
	myTV.voldown();
	cout << "------changed info----------\n";
	myTV.settings();
	remote myRem;
	myRem.chanup(myTV);
	cout << "------changed info----------\n";
	myRem.settings(myTV);
	myRem.set_chan(myTV,15);
	cout << "------changed info----------\n";
	myRem.settings(myTV);
}