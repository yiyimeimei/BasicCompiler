#include<iostream>
#include<unordered_map>
//#include<ext/hash_map>
//#include<hash_map>
//#include<vector>
//#include<list>
using namespace std;
template<typename K>
struct listNode
{
    K value;
    listNode *pre;
    listNode *next;
};
template<typename K>
class LimSet {
private:
    /*struct listNode
    {
        K value;
        listNode *pre;
        listNode *next;
    };*/
    int size, cap;
    unordered_map <K,listNode<K>*> elementSet;
    listNode<K> *head;
    listNode<K> *tail;


public:
    explicit LimSet(int capacity)
    {
        head=new listNode<K>;
        tail=new listNode<K>;
        head->pre=nullptr;
        head->next=tail;
        tail->pre=head;
        tail->next=nullptr;
        size = 0;
        cap = capacity;
    }

    bool in(const K& key)
    {
        typename unordered_map<K,listNode<K>*>::const_iterator got=elementSet.find(key);
        if(got==elementSet.end())//not found
        {
            return 0;
        }
        else//found
        {
            //cout<<"in find "<<key<<endl;
            listNode<K> *p=got->second;
            if(p!=tail)
            {
                p->pre->next=p->next;
                p->next->pre=p->pre;
                p->pre=head;
                p->next=head->next;
                head->next=p;
                p->next->pre=p;
            }
            return 1;
        }
    }

    void add(const K& key)
    {
        typename unordered_map<K,listNode<K>*>::const_iterator got=elementSet.find(key);//判断元素是否重复
        if(got==elementSet.end())//不存在
        {
            //cout<<"element doesn't exist"<<endl;
            if(size<cap)
            {
                //cout<<"not delete element"<<endl;
                listNode<K> *p=new listNode<K>;
                p->value=key;
                p->pre=head;
                p->next=head->next;
                head->next=p;
                p->next->pre=p;
                elementSet.insert(pair<K,listNode<K>*>(key,p));
            }
            else if(size==cap)
            {
                //cout<<"delete element"<<endl;
                //删除最久没有使用的元素
                listNode<K> *p=tail->pre;
                p->pre->next=tail;
                tail->pre=p->pre;
                if(!elementSet.erase(p->value))
                {
                    //cout<<"delete fail"<<p->value<<endl;
                    return;
                }
                //cout<<"add delete success "<<p->value<<"  "<<key<<endl;
                --size;
                //加入最新add的元素到顺序表里
                p->value=key;
                p->pre=head;
                p->next=head->next;
                head->next=p;
                p->next->pre=p;
                elementSet.insert(pair<K,listNode<K>*>(key,p));
            }
            ++size;
        }
        else//已经存在
        {
            //cout<<"element exist"<<endl;
            listNode<K> *p=got->second;
            //更新它到最新的位置
            if(p!=tail)
            {
                p->pre->next=p->next;
                p->next->pre=p->pre;
                p->pre=head;
                p->next=head->next;
                head->next=p;
                p->next->pre=p;
            }
        }
    }

    void remove(const K& key)
    {
        //在set中查找
        typename unordered_map<K,listNode<K>*>::const_iterator got=elementSet.find(key);
        if(got==elementSet.end())
        {
            return;
        }
        else
        {
            listNode<K> *p=got->second;
            //删除此元素在顺序表的位置
            if(p!=tail)
            {
                p->pre->next=p->next;
                p->next->pre=p->pre;
                delete p;
            }
            elementSet.erase(key);
            --size;
        }
        //cout<<"delete success"<<key<<endl;
    }
};


/*int main()
{
    LimSet<int> test(5);
    test.add(1);
    test.add(2);
    test.add(3);
    test.remove(4);
    cout<<test.in(3)<<endl;
    test.add(4);
    test.add(5);
    cout<<test.in(1)<<endl;
    test.add(6);

    cout<<test.in(2)<<endl;
    test.remove(2);
    cout<<test.in(2)<<endl;
    return 0;
}*/
