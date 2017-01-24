#include<iostream>
#include<string>
#include"temple.h"
using std::string;
int main()
{
	base<int, 10>sum;
	base<double,10>avg;
	base<base<int, 5>, 10>twoArr;
	for (int i = 0; i < 10; i++)
	{
		sum[i] = 0;
		for (int j = 0; j < 5; j++)
		{
			twoArr[i][j] = (i+1)*(j+1);
			sum[i] += twoArr[i][j];
		}
		avg[i] = sum[i] / 5;
	}
	for (int i = 0; i < 10; i++)
	{		 
		cout << "now handle " << i << " rows;" << " the sum of the row is:" << sum[i] << " the averge of the row is:" << avg[i] << endl;

	}
}
