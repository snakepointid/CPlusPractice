#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cctype>
#include<ctime>
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::ifstream;
using std::getline;
using std::tolower;
string * getWordList(string *wordlist,string &dir);
void playWordGuessGame(string *wordlist);
const int wordNum=26;
string &trim(string &s);
void main()
{
	string dir = "C:/Users/Administrator.NBJXUEJUN-LI/Desktop/project/CPlusPractice/inputFiles/wordlist.txt";
	string wordlist[wordNum];
	getWordList(wordlist, dir);
	playWordGuessGame(wordlist);
	
}

string * getWordList(string *wordlist,string & dir)
{
	 
	ifstream fin;
	string word;
	int count = 0;
	fin.open(dir);
	if (fin.is_open() == false)
	{
		cout << "there is no such files \n";
		exit(EXIT_FAILURE);
	}
	getline(fin, word, ',');
	while (fin)
	{
		wordlist[count++] = trim(word);
		getline(fin, word, ',');
	}
	cout << "Done \n";
	fin.close();
	return wordlist;
}

void playWordGuessGame(string * wordlist)
{
	char play;
	char guessLetter;
	int loc;
	std::srand(std::time(0));	 
	cout << "do you want to play this fantasitic game? y/n \n";
	cin >> play;
	while (tolower(play) == 'y')
	{
		int guess = 6;
		string targetWord = wordlist[std::rand() % wordNum];			 
		string guessWord(targetWord.size(), '_');
		string wrongWord;
		while (guess > 0 && targetWord != guessWord)
		{		
 
			cout << "you have guessed: "<<guessWord<<"\nguess letter:";
			cin >> guessLetter;
			if (guessWord.find(guessLetter) != string::npos ||
				wrongWord.find(guessLetter) != string::npos)
			{
				cout << "you have already guess that letter,please guess again\n";
				continue;
			}
			loc = targetWord.find(guessLetter);
	 
			if (loc == string::npos)
			{
				wrongWord += guessLetter;
				guess--;
				cout << "you guess a wrong letter, and you have " << guess << " chances left \n";
				continue;
			}
			while (loc != string::npos)
			{
				 
				guessWord[loc] = guessLetter;
				loc = targetWord.find(guessLetter, loc + 1);			 
			}
		}
		if (targetWord != guessWord)
			cout << "sorry,you failed!!!\nthe correct word is: "<<targetWord<<"\nyou guess   word is: "<<wrongWord<<endl ;
		else
			cout << "bravo job! you are good at guess word:  "<<targetWord;
		cout<<"\ndo you want to play again? y/n \n";
		cin >> play;
	}
}

string & trim(string & s)
{
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(' '));
	s.erase(s.find_last_not_of(' ')+1,s.size());
	return s;
}
