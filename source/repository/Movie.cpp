#include "Movie.h"

Movie::Movie(int id, const string& title, const string& genre, int duration,
             const string& description, float rating)
    : id(id), title(title), genre(genre), duration(duration),
      description(description), rating(rating) {}

int Movie::getId() const { return id; }
string Movie::getTitle() const { return title; }
string Movie::getGenre() const { return genre; }
int Movie::getDuration() const { return duration; }
string Movie::getDescription() const { return description; }
float Movie::getRating() const { return rating; }
const vector<string>& Movie::getShowTimes() const { return showTimes; }
void Movie::addShowTime(const string& time) { showTimes.push_back(time); }

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

class IMovie{
};

class Movie:public IMovie{
private:
    int _id;
    string title;
    string genre;
    int duration;
    string description;
    float rating;
    vector<ShowTime> showTimes;

}

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

#endif // IMOVIE_H






