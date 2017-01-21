#include<iostream>
#include "queue.h"
using namespace std;
 
void customer::setCustomer(long t)
{
	arrive_time = t;
	process_time = rand() % 3 + 1;
}

queue::queue(int qs):qsize(qs)
{
	front = rear = NULL;
	items = 0;
}

queue::~queue()
{
	Node *temp;
	while (front != NULL)
	{
		temp = front;
		front = front->next;
		delete temp;
	}
}

bool queue::isEmpty()
{
	return items == 0;
}

bool queue::isFull()
{
	return items == qsize;	 
}

int queue::queueCount()
{
	return items;
}

bool queue::addItem(const Item & item)
{
	if(isFull())
		return false;
	Node *add = new Node;
	add->item = item;
	add->next = NULL;
	items++;
	if (front == NULL)
		front = add;
	else
		rear->next = add;
	rear = add;
	return true;
}

bool queue::dropItem(Item &item)
{
	if (isEmpty())
		return false;	
	item = front->item; 
	items--;
	
	Node *temp = front;
	front = front->next;
	delete temp;
	if (items == 0)
		rear = NULL;
	return true;
}
