#include <iostream>
#include <vector>
using namespace std;
double trivialMedian(std::vector<int>& nums1, int lo1, int n1, std::vector<int>& nums2, int lo2, int n2)
{
    int hi1 = lo1 + n1, hi2 = lo2 + n2;
    std::vector<int> num;
    while((lo1 < hi1) && (lo2 < hi2))
    {
        num.push_back(nums1[lo1] <= nums2[lo2] ? nums1[lo1++] : nums2[lo2++]);
    }
    while(lo1 < hi1)
        num.push_back(nums1[lo1++]);
    while(lo2 < hi2)
        num.push_back(nums2[lo2++]);
    /*for(int i = 0; i < n1+ n2; ++i)
        cout<<num[i]<<endl;*/
    if((n1 + n2) % 2 == 1)
    {
        return (double) num[(n1 + n2) / 2];
    }
    else
    {
        return (double)(num[(n1 + n2) / 2] + num[((n1 + n2) / 2) - 1]) / 2;
    }
}

double median(std::vector<int>& nums1, int lo1, int n1, std::vector<int>& nums2, int lo2, int n2)
{
    //cout<<lo1<<" "<<n1<<endl;
    //cout<<lo2<<" "<<n2<<endl;
    if(n1 < n2)
        return median(nums2, lo2, n2, nums1, lo1, n1);
    if(n2 < 6)
        return trivialMedian(nums1, lo1, n1, nums2, lo2, n2);
    if(2 * n1 < n2)
        return median(nums1, lo1, n1, nums2, lo2, n2);
    int mi1 = lo1 + n1 / 2, mi2a = lo2 + (n1 - 1) / 2, mi2b = lo2 + n2 - 1 - n1 / 2;
    if(nums1[mi1] > nums2[mi2b])
        return median(nums1, lo1, n1 / 2 + 1, nums2, mi2a, n2 - (n1 - 1) / 2);
    else if(nums1[mi1] < nums2[mi2a])
        return median(nums1, mi1, (n1 + 1) / 2, nums2, lo2, n2 - n1 / 2);
    else
        return median(nums1, lo1, n1, nums2, mi2a, n2 - (n1 - 1) / 2 * 2);
}


double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
{
    int n1 = nums1.size();
    int n2 = nums2.size();
    return median(nums1, 0, n1, nums2, 0, n2);
}


int main()
{

    /*std::vector<int> num1(5);

    std::vector<int> num2(5);
    for(int i = 0 ;i<5;++i)
    {
        num1[i] = i+1;
        num2[i] = i+6;
        cout<<num1[i]<<" "<<num2[i]<<endl;
    }

    cout<<findMedianSortedArrays(num1, num2);*/
    cout<<sizeof (string);
    return 0;
}
