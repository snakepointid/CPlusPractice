#include"studentp.h"
namespace prit
{
	ostream & student::arr_out(ostream & os) const
	{

		os << "his scores are: \n";
		int scoreNum = arryDB::size();
		int i;
		if (scoreNum > 0)
		{
			for (i = 0; i < scoreNum; i++)
			{
				os << arryDB::operator[](i) << " ";
				if (i % 5 == 4)
					os << endl;
			}
			if (i % 5 != 0)
				os << endl;
		}
		else
		{
			os << "there are no scores for this students \n";
		}
		return os;
	}

	double student::average() const
	{
		if (arryDB::size() > 0)
			return arryDB::sum() / arryDB::size();
		else
			return 0;
	}



	const istream & operator >> (istream & is, student & stu)
	{
		cout << "now,using the private inheritance way,please enter the student's name:\n";
		is >> (string &)stu;
		return is;
	}

	const istream & getline(istream & is, student & stu)
	{
		cout << "now,using the private inheritance way,please enter the student's name:\n";
		getline(is, (string &)stu);
		return is;
	}

	const ostream & operator<<(ostream & os, student & stu)
	{
		os << "his name is: " << (string &)stu << endl;
		stu.arr_out(os);
		return os;
	}

}
