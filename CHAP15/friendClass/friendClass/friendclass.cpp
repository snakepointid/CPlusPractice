#include "friendclass.h"

void tv::settings()
{
	cout << "the state is: " << (state==on?"on":"off") << "\n";
	cout << "the mode is: " << (mode == antenna ? "antenna" : "cable") << "\n";
	cout << "the input is: " << (input == TV ? "TV" : "DVD") << "\n";
	cout << "the channel is: " << channel << "\n";
	cout << "the volume is: " << volume << "\n";
}

bool tv::volup()
{
	if (volume < maxVol)
	{
		volume++;
		return true;
	}
	else
		return false;
}

bool tv::voldown()
{
	if (volume>minVol)
	{
		volume--;
		return true;
	}
	else
		return false;
}

void tv::chanup()
{
	if (channel < maxchannel)
		channel++;
	else
		channel = 1;
}

void tv::chandown()
{
	if (channel>1)
		channel--;
	else
		channel = maxchannel;
}
