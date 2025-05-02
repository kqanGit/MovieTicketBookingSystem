#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Node{
    IMovie data;
    Node *pNext;
};

class SList{    
private :
    int _n;
    Node* _pHead;
    Node* _pTail;
    static Node* CreateNode(const IMovie &data);
public:
    SList();
    ~SList();
    void AddTail(const IMovie& data);
    void Remove(const int& index);
};


class IMovieRepository{
    public:
    };
    

// SList
Node* SList::CreateNode(const IMovie &data){
    Node* newNode = new Node;
    newNode->data = data;
    newNode->pNext = nullptr;
    return newNode;
}

SList::SList() {
    _pHead = _pTail = nullptr;
    _n = 0;
}

SList::~SList(){
    if(_n != 0){
        Node* current = _pHead;
        while(current != nullptr){
            _pHead = _pHead->pNext;
            delete current;
            current = _pHead;
        }
    }
}


void SList::AddTail(const IMovie& data){
    Node* newNode = CreateNode(data);
    if(_pTail = nullptr){
        _pHead = _pTail = newNode;
        _n++;
        return;
    }

    _pTail->pNext = newNode;
    newNode = _pTail;
    _n++;
}


void SList::Remove(const int& index){
    if(index >= _n++ || index < 0){
        throw out_of_range("Out_of_range");
    }
    Node* find = _pHead;
    Node *pre = _pHead;
    int i = 0;
    while(i != index){
        pre = find;
        find = find->pNext;
    }
    pre->pNext = find->pNext;
    _n--;
    delete find;
}








