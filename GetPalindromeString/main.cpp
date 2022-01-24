#include <iostream>
#include <string.h>
using namespace std;

void getNexti(string str, int *next)
{
    int j = 0, k = -1;
    while(str[j] != '\0')
    {
        if(k == -1 || str[j] == str[k])
        {
            next[++j] = ++k;
        }
        else
            k = next[k];
    }
}
string GetPalindromeString(string s)
{
    if(s.length() == 0)
        return "";
    if(s.length() == 1)
        return s;
    int len = s.length();
    int *next = new int[len * 2 + 2];
    next[0] = -1;
    next[1] = 0;
    string r = "";
    for(int i = len -1; i >= 0; --i)
    {
        r += s[i];
    }
    string t = s + "%" + r;
    getNexti(t,next);
    string result = r;
    for(int i = next[len * 2 + 1]; i < len; ++i)
    {
        result += s[i];
    }
    delete []next;
    return result;
}





int main()
{
    string s;
    while(1)
    {
        cin>>s;
        cout << GetPalindromeString(s) << endl;
    }
    return 0;
}
