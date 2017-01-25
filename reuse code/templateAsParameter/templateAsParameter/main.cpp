#include "temPara.h"
void main()
{
	base<int>i1(3);
	base<int>i2(13);
	base<int>i3(23);
	base<double>d1(4);
	base<double>d2(6.6);
	counts();
	report(i1);
	report(i2);
	report(i3);
	report(d1);
	report(d2);
}