// a3.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Aryan Seth
// St.# : 301576005
// Email: asa515@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Node
{
private:
public:
    Announcement ann;
    Node* next;
// constructors
    Node(const string &sender_name, Rank rank, const string &text, Node* nxt = nullptr) : ann(sender_name,rank,text) , next(nxt) {}
    Node(const Announcement &other, Node* nxt = nullptr) : ann(other) , next (nxt) {}
    Node(string line, Node* nxt = nullptr) : ann(line), next(nxt) {}
    

};

class Queue_LL : public Queue_base<Announcement>
{
private:


int sz;
Node* front;
Node* back;


public:
    Queue_LL();
    ~Queue_LL();

    int size() { return sz; }
    void enqueue(const Announcement &item);
};

// add item to the back of the queue
void Queue_LL::enqueue(const Announcement & item) 
{
    Node* temp = new Node(item,nullptr);
    if ((front == nullptr) && (back == nullptr))
    {
        back = temp;
        front = back;
        return;
    }
    back->next = temp;
    back = temp;
    
}



Queue_LL::Queue_LL() : sz(0), front(nullptr), back(nullptr)
{
}

Queue_LL::~Queue_LL()
{
    while (front != nullptr)
    {
        Node* temp = front->next;
        delete front;
        front = temp;    
    }
    
}



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // making the file stream of the argument given in the command line
    ifstream ifile(argv[1]);

    return 0;
}