/* 519021910721 LuYimin */
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;
mutex mtx;
bool zeroFlag = true;
int nCount = 1;
class Triple
{
private:
    int n;
public:
    Triple(int x):n(x){}
    void zero()
    {
        while(1)
        {
            lock_guard<mutex> lck0(mtx);
            if(zeroFlag && nCount <= n)
            {
                cout<<0;
                zeroFlag = false;
            }
        }
    }
    void one()
    {
        while(1)
        {
            lock_guard<mutex> lck1(mtx);
            if(!zeroFlag && nCount % 3 == 1 && nCount <= n)
            {
                cout<<nCount;
                zeroFlag = true;
                ++nCount;
            }
        }
    }
    void two()
    {
        while(1)
        {
            lock_guard<mutex> lck2(mtx);
            if(!zeroFlag && nCount % 3 == 2 && nCount <= n)
            {
                cout<<nCount;
                zeroFlag = true;
                ++nCount;
            }
        }
    }
    void three()
    {
        while(1)
        {
            lock_guard<mutex> lck3(mtx);
            if(!zeroFlag && nCount % 3 == 0 && nCount <= n)
            {
                cout<<nCount;
                zeroFlag = true;
                ++nCount;
            }
        }
    }
};
int main()
{
    Triple t(10);
    vector<thread> threads;
    threads.emplace_back(&Triple::zero, t);
    threads.emplace_back(&Triple::one, t);
    threads.emplace_back(&Triple::two, t);
    threads.emplace_back(&Triple::three, t);
    for (auto& thread: threads)
        thread.join();
    return 0;
}
