#include "studentc.h"
namespace cont
{
	ostream & student::arr_out(ostream & os) const
	{

		os << "his scores are: \n";
		int scorenum = scores.size();
		int i;
		if (scorenum > 0)
		{
			for (i = 0; i < scorenum; i++)
			{
				os << scores[i] << " ";
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
		if (scores.size() > 0)
			return scores.sum() / scores.size();
		else
			return 0;
	}



	const istream & operator >> (istream & is, student & stu)
	{
		cout << "now,using the containment  way,please enter the student's name:\n";
		is >> stu.name;
		return is;
	}

	const istream & getline(istream & is, student & stu)
	{
		cout << "now,using the containment  way,please enter the student's name:\n";
		getline(is, stu.name);
		return is;
	}

	const ostream & operator<<(ostream & os, student & stu)
	{
		os << "his name is: " << stu.name << endl;
		stu.arr_out(os);
		return os;
	}

}
