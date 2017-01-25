#pragma once
#include<iostream>
using std::cout;
using std::endl;
#ifndef FRIENDCLASS_H_
#define FRIENDCLASS_H_
class tv  
{
private:
	const int maxchannel;
	int state;
	int volume;
	int channel;
	int mode;
	int input;
public:
	friend class remote;
	enum{on,off};
	enum{minVol,maxVol=20};
	enum{TV,DVD};
	enum{antenna,cable};
	//constructor and desconstructor
	tv(int s=off,int mc=125):state(s), volume(10), channel(1), mode(TV), input(cable),maxchannel(mc){}
	//member methods
	void set_onoff() { state = (state == on) ? off : on; }
	void set_mode() { mode = (mode == DVD) ? TV : DVD; }
	void set_input() { input = (input == antenna) ? cable : antenna; }
	void settings();
	bool ison() { return state == on; }
	bool volup();
	bool voldown();
	void chanup();
	void chandown();
};
class remote
{
private:
	int mode;
public:
	remote(int m=tv::TV):mode(m){}
	void set_onoff(tv &tv) { tv.set_onoff(); }
	void set_mode(tv &tv) { tv.set_mode(); }
	void set_input(tv &tv) { tv.set_input(); }
	void settings(tv &tv) { tv.settings(); }
	bool volup(tv &tv) { return  tv.volup(); }
	bool voldown(tv &tv) { return tv.voldown(); }
	void chanup(tv &tv) { tv.chanup(); }
	void set_chan(tv &tv, int c) { tv.channel =c; }
	void chandown(tv &tv) { tv.chandown(); }
};
#endif // !FRIENDCLASS_H_
