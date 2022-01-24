#include<iostream>
#include<unordered_map>
#include<ext/hash_map>
#include<hash_map>
#include<vector>
using namespace std;
template<typename K>
class LimSet {
private:
    struct listNode
    {
        K value;
        listNode *pre;
        listNode *next;
    };
    int size, cap;
    //vector <K> elementUse;
    unordered_map <K,int> elementSet;
    int count=0;
    listNode *head;
    listNode *tail;


public:
    explicit LimSet(int capacity)
    {
        head=new listNode;
        tail=new listNode;
        head->pre=nullptr;
        head->next=tail;
        tail->pre=head;
        tail->next=nullptr;
        size = 0;
        cap = capacity;
    }

    bool in(const K& key)
    {
        typename unordered_map<K,int>::const_iterator got=elementSet.find(key);
        if(got==elementSet.end())//not found
        {
            return 0;
        }
        else//found
        {
            listNode *p=head->next;
            while(!p->next)
            {
                if(p->value==key)
                    break;
                p=p->next;
            }
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
        typename unordered_map<K,int>::const_iterator got=elementSet.find(key);//判断元素是否重复
        if(got==elementSet.end())//不存在
        {
            if(size<cap)
            {
                //elementUse.push_back(key);
                listNode *tmp=new listNode;
                tmp->value=key;
                tmp->pre=head;
                tmp->next=head->next;
                head->next=tmp;
                tmp->next->pre=tmp;
            }
            else if(size==cap)
            {
                --size;
                //删除最久没有使用的元素
                listNode *p=tail->pre;
                p->pre->next=tail;
                tail->pre=p->pre;
                //加入最新add的元素到顺序表里
                p->value=key;
                p->pre=head;
                p->next=head->next;
                head->next=p;
                p->next->pre=p;
            }
            elementSet.insert(pair<K,int>(key,count));
            ++count;
            ++size;
        }
        else//已经存在
        {
            listNode *p=head->next;
            //找到这个元素的位置
            while(!p->next)
            {
                if(p->value==key)
                    break;
                p=p->next;
            }
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
        if(!elementSet.erase(key))
        {
            cout<<0;
            return;
        }
        --size;
        listNode *p=head->next;
        //找到这个元素的位置
        while(!p->next)
        {
            if(p->value==key)
                break;
            p=p->next;
        }
        //删除此元素在顺序表的位置
        if(p!=tail)
        {
            p->pre->next=p->next;
            p->next->pre=p->pre;
            delete p;
        }
    }
};


int main()
{
    LimSet<int> test(5);
    test.add(1);
    test.add(2);
    test.add(3);
    test.remove(4);
    return 0;


}
