#include<iostream>
#include<vector>
#include<string>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::getline;
struct review
{
	string name;
	int rating;
};
bool fillReviews(review &rr);
void showReviews(review &rr);
void main()
{
	review tmp;

	vector<review> books;
	while (fillReviews(tmp))
		books.push_back(tmp);
	auto irp = books.begin();
	cout << "rating\tbook\n";
	for (irp; irp != books.end(); irp++)
		showReviews(*irp);
	auto oldlist(books);
	cout << "erases the first two book\n";
	books.erase(books.begin(), books.begin() + 2);
	cout << "rating\tbook\n";
	for (irp = books.begin(); irp != books.end(); irp++)
		showReviews(*irp);
	cout << "swap back\n";
	books.swap(oldlist);
	cout << "rating\tbook\n";
	for (irp = books.begin(); irp != books.end(); irp++)
		showReviews(*irp);
}

bool fillReviews(review & rr)
{
	 
	cout << "please enter your book title<'quit' to quit>:\n";
	getline(cin, rr.name);
	if(rr.name=="quit")
		return false;
	cout << "please enter your book rating:\n";
	cin >> rr.rating;
	if (!cin)
		return false;
	while (cin.get()!='\n')
		continue;
	return true;
}

void showReviews(review & rr)
{
	cout << rr.rating << "\t" << rr.name << endl;
}
