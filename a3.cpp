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
    Node *next;
    // constructors
    Node(const string &sender_name, Rank rank, const string &text, Node *nxt = nullptr) : ann(sender_name, rank, text), next(nxt) {}
    Node(const Announcement &other, Node *nxt = nullptr) : ann(other), next(nxt) {}
    Node(string line, Node *nxt = nullptr) : ann(line), next(nxt) {}
};

class Queue_LL : public Queue_base<Announcement>
{
private:
    int sz;
    Node *frnt;
    Node *back;

public:
    Queue_LL();
    ~Queue_LL();

    int size() const { return sz; }
    void enqueue(const Announcement &item);
    const Announcement &front() const;
    void dequeue();
};
// removes the item at the front of the queue
void Queue_LL::dequeue()
{
    if (frnt == nullptr)
    {
        throw runtime_error("dequeue: queue is empty");
    }
    Node *temp = frnt->next;
    delete frnt;
    frnt = temp;
}

const Announcement &Queue_LL::front() const
{
    if (frnt == nullptr && back != nullptr)
    {
        throw runtime_error("frnt == nullptr && back != nullptr");
    }
    if (frnt == nullptr)
    {
        throw runtime_error("front: queue is empty");
    }
    return frnt->ann;
}

// add item to the back of the queue
void Queue_LL::enqueue(const Announcement &item)
{
    Node *temp = new Node(item, nullptr);
    if ((frnt == nullptr) && (back == nullptr))
    {
        back = temp;
        frnt = back;
        return;
    }
    back->next = temp;
    back = temp;
}

Queue_LL::Queue_LL() : sz(0), frnt(nullptr), back(nullptr)
{
}

Queue_LL::~Queue_LL()
{
    while (frnt != nullptr)
    {
        Node *temp = frnt->next;
        delete frnt;
        frnt = temp;
    }
}

class JingleNet
{
private:
    Queue_LL Santa;
    Queue_LL Reindeer;
    Queue_LL Elf2;
    Queue_LL Elf1;
    Queue_LL Snowman;
    // now the 4 methods for 4 commands
    void send(const string &, const Rank &, const string &);
    void remove_all(const string &);
    void promote_announcements(const string &);
    void announce(const int &);
    // helper functions
    void remove_from_queue(Queue_LL& q, const string& val);
    void promote_announcement_from_queue(const string&, Queue_LL&, Queue_LL& );

public:
    // point of entry of the commands
    void entry(const string &);
    JingleNet();
    ~JingleNet();
};

JingleNet::JingleNet()
{
}

JingleNet::~JingleNet()
{
}

void JingleNet::entry(const string &command)
{
    if (command.find("SEND") != string::npos)
    {
        string cmnd(command);
        string arr[3];
        cmnd.erase(0, 5);
        for (int i = 0; i < 2; i++)
        {
            int index = cmnd.find_first_of(" ");
            arr[i] = cmnd.substr(0, index);
            cmnd.erase(0, index + 1);
        }
        send(arr[0], to_rank(arr[1]), cmnd);
        
    }
    else if (command.find("REMOVE_ALL") != string::npos)
    {
        string c(command);
        c.erase(0, 11);
        remove_all(c);
    }
    else if (command.find("PROMOTE_ANNOUNCEMENTS") != string::npos)
    {
        string c(command);
        c.erase(0, 22);
        promote_announcements(c);
    }
    else if (command.find("ANNOUNCE") != string::npos)
    {
        string c(command);
        c.erase(0, 9);
        int n = stoi(c);
        announce(n);
    }
    else
    {
        throw runtime_error("invalid command: in the else block");
    }
}

void JingleNet::send(const string &username, const Rank &rank, const string &message)
{
    Announcement temp(username,rank,message);
    if (rank == Rank::SNOWMAN)
    {
        Snowman.enqueue(temp);
    }
    else if (rank == Rank::ELF1)
    {
        Elf1.enqueue(temp);
    }
    else if (rank == Rank::ELF2)
    {
        Elf2.enqueue(temp);
    }
    else if (rank == Rank::REINDEER)
    {
        Reindeer.enqueue(temp);
    }
    else if (rank == Rank::SANTA)
    {
        Santa.enqueue(temp);
    }
    else
    {
        throw runtime_error("invalid rank: in the else block");
    }
    
    
    
    
}

void JingleNet::remove_all(const string& username)
{
    remove_from_queue(Santa,username);
    remove_from_queue(Reindeer,username);
    remove_from_queue(Elf2,username);
    remove_from_queue(Elf1,username);
    remove_from_queue(Snowman,username);
}

void JingleNet::remove_from_queue(Queue_LL& q, const string& val)
{
    Queue_LL temp;
    while(q.size() != 0)
    {
        Announcement t = q.front();
        q.dequeue();
        temp.enqueue(t);
    }
    while (temp.size() != 0)
    {
        Announcement t = temp.front();
        temp.dequeue();
        if (t.get_sender_name() == val)
        {
            continue;
        }
        q.enqueue(t);
    }
    
}


void JingleNet::promote_announcements(const string & username)
{
    promote_announcement_from_queue(username,Reindeer,Santa);
    promote_announcement_from_queue(username,Elf2,Reindeer);
    promote_announcement_from_queue(username,Elf1,Elf2);
    promote_announcement_from_queue(username,Snowman,Elf1);
}
void JingleNet::promote_announcement_from_queue(const string& username, Queue_LL& q1, Queue_LL& q2)
{
    Queue_LL temp;
    while (q1.size() != 0)
    {
        Announcement t = q1.front();   
        q1.dequeue();
        if (t.get_sender_name() != username)
        {
            temp.enqueue(t);
            continue;
        }
        q2.enqueue(t);
        
    }
    while (temp.size() != 0)
    {
        Announcement t = temp.front();
        temp.dequeue();
        q1.enqueue(t);
    }
    
    
    
}


void JingleNet::announce(const int & count)
{
    int temp = 0;
    while (Santa.size() != 0 && temp != count)
    {
        Announcement a = Santa.front();
        jnet.announce(a);
        Santa.dequeue();
        temp++;
    }
    while (Reindeer.size() != 0 && temp != count)
    {
        Announcement a = Reindeer.front();
        jnet.announce(a);
        Reindeer.dequeue();
        temp++;
    }
    while (Elf2.size() != 0 && temp != count)
    {
        Announcement a = Elf2.front();
        jnet.announce(a);
        Elf2.dequeue();
        temp++;
    }
    while (Elf1.size() != 0 && temp != count)
    {
        Announcement a = Elf1.front();
        jnet.announce(a);
        Elf1.dequeue();
        temp++;
    }
    while (Snowman.size() != 0 && temp != count)
    {
        Announcement a = Snowman.front();
        jnet.announce(a);
        Snowman.dequeue();
        temp++;
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
    string line;
    JingleNet jinglenet;
    while (getline(ifile,line))
    {
        jinglenet.entry(line);
    }
    ifile.close();
    

    return 0;
}