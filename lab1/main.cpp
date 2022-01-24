#include<iostream>
#include<vector>
#include<map>
using namespace std;
template <typename K, typename V>
struct Dictionary
{
    vector<K> key;
    vector<V> value;
    int size()
    {
        return key.size();
    }
    bool put(K k, V v)
    {
       bool flag=true;
       for(int i=0;i<key.size();++i)
       {
            if(key[i]==k)
            {
                value[i]=v;
                flag=false;
                break;
            }
       }
       if(flag)
       {
           key.push_back(k);
           value.push_back(v);
       }
        return true;
    }
    V* get(K k)
    {
        for(int i=0;i<key.size();++i)
        {
             if(key[i]==k)
             {
                 return &(value[i]);
             }
        }
        return nullptr;
    }
    bool remove(K k)
    {
        for(int i=0;i<key.size();++i)
        {
             if(key[i]==k)
             {
                 key.erase(key.begin()+i);
                 value.erase(value.begin()+i);
                 return true;
             }
        }
        return false;

    }
    template <typename K1, typename V1>
    friend ostream& operator<<(ostream& os, const Dictionary<K1, V1>& dic);
};

template <typename K1, typename V1>
ostream& operator<<(ostream& os, const Dictionary<K1, V1>& dic)
{
    typedef typename map<K1,V1>::iterator iter;
    map<K1,V1> m;
    for(int i=0;i<dic.key.size();++i)
    {
        m.insert(pair<K1,V1>(dic.key[i],dic.value[i]));

    }
    iter it;
    for(it=m.begin ();it!=m.end ();++it)
    cout<<it->first<<" "<<it->second<<" ";
    return os;
}
