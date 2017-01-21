#pragma once
#ifndef QUEUE_H_
#define QUEUE_H_
class customer
{
private:
	long arrive_time;
	int process_time;
public:
	customer() { arrive_time = process_time = 0; } 
	void setCustomer(long t);
	long when() { return arrive_time; } 
	int ptime() { return process_time; } 
};
typedef customer Item;
class queue
{
private:
	struct Node{Item item;Node *next;};
	enum { Q_size = 10};
	Node *front;
	Node *rear;
	int items;
	const int qsize;
	//robust copy constructor
	queue(const queue & q):qsize(0){}
	queue &operator=(const queue &q) { return *this; }
public:
	queue(int qs = Q_size);
	~queue();
	bool isEmpty();
	bool isFull();
	int queueCount();
	bool addItem(const Item &item);
	bool dropItem(Item &item);
};
#endif // !QUEUE_H_
