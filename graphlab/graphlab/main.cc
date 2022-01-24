#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#define MAXLENGTH 4294967295


int main(int argc, char *argv[])
{
    ifstream file(argv[1]);
    if(!file.is_open())
    {
        cout << "Fail to open file.";
    }
    uint32_t nodeNum;
    file>>nodeNum;
    uint32_t *DC = new uint32_t [nodeNum];
    double *CC = new double [nodeNum];
    uint32_t *BC = new uint32_t [nodeNum];
    uint32_t **dist = new uint32_t *[nodeNum];
    for(uint32_t i = 0; i < nodeNum; ++i)
    {
        DC[i] = 0;
        CC[i] = MAXLENGTH;
        BC[i] = 0;
        dist[i] = new uint32_t[nodeNum];
        for(uint32_t j = 0; j < nodeNum; ++j)
        {
            dist[i][j] = MAXLENGTH;
        }
    }
    char temp;
    uint32_t v1, v2, weight;
    while(!file.eof())
    {
        file>>v1;
        file>>temp;//读取逗号
        file>>v2;
        file>>temp;
        file>>weight;
        dist[v1][v2] = weight;
        dist[v2][v1] = weight;
    }
    /* 度中心性 */
    for(uint32_t i = 0; i < nodeNum; ++i)
    {
        for(uint32_t j = i + 1; j < nodeNum; ++j)
        {
            if(dist[i][j] == MAXLENGTH)
                continue;
            DC[i]++;
            DC[j]++;
        }
    }
    uint32_t max_dc = DC[0], max_dc_node = 0;
    for(uint32_t i = 1; i < nodeNum; ++i)
    {
        if(max_dc < DC[i])
        {
            max_dc = DC[i];
            max_dc_node = i;
        }
    }

    /* 最短距离 弗洛伊德算法*/
    for(uint32_t k = 0; k < nodeNum; ++k)
        for(uint32_t i = 0; i < nodeNum; ++i)
        {
            if(dist[i][k] == MAXLENGTH)
            {
                continue;
            }
            for(uint32_t j = 0; j < i; ++j)
                if(dist[i][k] + dist[k][j] < dist[i][j] && dist[k][j] != MAXLENGTH)
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    dist[j][i] = dist[i][j];
                }
        }
    /* 亲近中心性 */
    double nodeNum1 = nodeNum - 1;
    double max_cc = 0;
    uint32_t max_cc_node = 0;
    for(uint32_t i = 0; i < nodeNum; ++i)
    {
        double distSum = 0;
        for(uint32_t j = 0; j < nodeNum; ++j)
        {
            if(j == i)
                continue;
            distSum += dist[i][j];
        }
        //cout<<i<<' '<<distSum<<endl;
        CC[i] = (nodeNum1 / distSum);
        if(CC[i] > max_cc)
        {
            max_cc = CC[i];
            max_cc_node = i;
        }
    }

    /* 中介中心性 */
    for(uint32_t k = 0; k < nodeNum; ++k)
        for(uint32_t i = 0; i < nodeNum; ++i)
        {
            if(i == k)
                continue;
            for(uint32_t j = 0; j < nodeNum; ++j)
            {
                if(j == k || j == i)
                    continue;
                if(dist[i][k] + dist[k][j] == dist[i][j])
                {
                    ++BC[k];
                }
            }
        }
    uint32_t max_bc = 0, max_bc_node = 0;
    for(uint32_t i = 0; i < nodeNum; ++i)
    {
        if(BC[i] > max_bc)
        {
            max_bc = BC[i];
            max_bc_node = i;
        }
    }
    cout<<max_dc_node<<' '<<max_dc<<endl;
    cout<<max_cc_node<<' '<<max_cc<<endl;
    cout<<max_bc_node<<' '<<(max_bc / 2)<<endl;
    file.close();
    return 0;
}

