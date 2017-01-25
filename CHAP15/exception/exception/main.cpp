#include "trycatch.h"
//#include<cstdlib>
double hmean(double x, double y);
double gmean(double x, double y);
void main()
{
	double x;
	double y;
	double ho;
	double go;
	cout << "please enter two numbers\n";
	while (cin >> x >> y)
	{
		try
		{
			ho = hmean(x, y);
			go = gmean(x, y);
			cout << "the hmean is:" << ho << " and the gmean is :" << go << endl;
			cout << "please enter another two numbers<enter 'q' to quit>\n";
		}
		catch (bad_hmean &bh)
		{
			bh.msg();
			cout << "please enter another two numbers<enter 'q' to quit>\n";
			continue;
		}
		catch (bad_gmean &bg)
		{
			bg.msg();
			break;
		}
	}
	
}

double hmean(double x, double y)
{
	if (x == -y)
	{
		throw bad_hmean(x, y);
	}
	return 2 * x*y / (x + y);
}

double gmean(double x, double y)
{
	if (x < 0 || y < 0)
		throw bad_gmean(x, y);
	return std::sqrt(x *y);
}
