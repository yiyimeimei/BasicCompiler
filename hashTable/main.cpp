#include <iostream>
#include <cmath>
#define SIZE 100
using namespace std;

struct LNode
{
    int key, value;
    LNode *next;
    LNode(int x, int y)
    {
        key = x;
        value = y;
        next = nullptr;
    }
};
class HashTable{
private:
    LNode **V;//存储value

    int size;

    int hash(const int key);
public:
    HashTable();

    void put(int key, int val);

    int get(const int key); // return -1 on searching failure

    void remove(int key);
};
HashTable::HashTable()
{
    size = SIZE;
    V = new LNode*[size];
    for(int i = 0; i < size; ++i)
        V[i] = new LNode(-1, -1);
}
int HashTable::hash(const int key)
{
    int half_size = size / 2;
    return key / half_size % half_size + half_size;
}
int HashTable::get(const int key)
{
    int hashResult = hash(key);
    LNode *p = V[hashResult];
    while(p != nullptr && p->key != key)
    {
        p = p->next;
    }
    if(p == nullptr)
        return -1;
    else
        return p->value;
}
void HashTable::put(const int key, const int val)
{
    int hashResult = hash(key);
    LNode *p = V[hashResult];
    while(p != nullptr && p->key != key)
    {
        p = p->next;
    }
    if(p == nullptr)
    {
        LNode *newNode = new LNode(key, val);
        newNode->next = V[hashResult]->next;
        V[hashResult]->next = newNode;
    }
    else
    {
        p->value = val;
    }
    return;
}
void HashTable::remove(int key)
{
    int hashResult = hash(key);
    LNode *p = V[hashResult];
    LNode *preP = new LNode(-1, -1);
    preP->next = p;
    while(p != nullptr && p->key != key)
    {
        p = p->next;
        preP = preP->next;
    }
    if(p == nullptr)
        return;
    else
    {
        preP->next = p->next;
        delete p;
        return;
    }
}
int main(){
    HashTable ht = HashTable();
    ht.put(1,1);
    ht.put(2,2);
    cout << ht.get(1)<<' ';
    cout << ht.get(3)<<' ';
    ht.put(2,1);
    cout << ht.get(2)<<' ';
    ht.remove(2);
    cout << ht.get(2)<<' ';
    ht.remove(3);
    cout << ht.get(3)<<' ';
}
