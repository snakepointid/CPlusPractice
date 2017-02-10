#include <iostream>
#include <omp.h>

using namespace std;
int test(int x) { return x; }();
int main()
{
	int max = 100;
	int &rmax = max;
	auto test= [&]() {
		int x = rmax;
		return x;
	}();
	cout << test[] << endl;
	cout << test();
}

