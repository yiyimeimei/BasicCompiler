#include<iostream>
#include<map>
using namespace std;
template <typename K, typename V>
struct Dictionary
{
    Map<K,V> element;
    int size()
    {
        return element.size();
    }
    bool put(K k, V v)
    {
        element.insert(pair<K,V>(k,v));
        return true;
    }
    V* get(K k)
    {

            return nullptr;
    }
    bool remove(K k)
    {
       // return element.erase(k);
        Iterator<K> it=element.keySet().iterator();

        while (it.hasNext())
        {
            K key = it.next();
            if (key==k)
            {
                it.remove();
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
    map<K1,V1>::iterator  it;
        for (it = dic.element.begin(); it != dic.element.end(); ++it)
        {
            cout<<it->first<<" ";
            cout<<it->second<<" ";
        }
    return os;
}


