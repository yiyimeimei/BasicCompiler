#include <iostream>
#include <vector>
#define INF 2147483647
using namespace std;

/*
 * 输入一个无向图的所有边,以vector<vector<int>>表示,而边用长度为2的vector<int>表示,包含其两端顶点的编号.
 * 该图有n个节点和n个边,顶点编号从1到n. 它由一棵树以及一条附加的边组成.
 * 该附加边的两端点是点1到点n中的点,且两端点不相同,也不是树中已经存在的边.
 * 返回一条可以从图中删掉的边,用长度为2的vector<int>表示,使得该图成为一棵树.如果有多个边可以做为被删掉的边，那么返回输入中的最后一条可以删除的边
 *
 * 请完成findRedundantConnection函数的编写, 提供代码与运行截图,输出应当是3 4
 * 分析你的实现的时间复杂度,和代码/运行截图放在一起提交
 * 示例:
 */

void print_vec(const vector<int> &p) {
    for (auto &i:p)
        cout << i << ' ';
    cout << endl;
}

vector<int> findRedundantConnection(vector<vector<int>> &edges)
{
    int n = edges.size();
    int **edgeMap = new int*[n + 1];
    int **dist = new int*[n + 1];
    bool *vertexMark = new bool[n + 1];
    bool *edgeMark = new bool[n + 1];
    for(int i = 1; i <= n; ++i)
    {
        edgeMap[i] = new int[n + 1];
        dist[i] = new int[2];
        for(int j = 1; j <= n; ++j)
        {
            edgeMap[i][j] = INF;
        }
    }
    //存储图
    for(int i = 0; i < n; ++i)
    {
        int v1, v2;
        v1 = edges[i][0];
        v2 = edges[i][1];
        edgeMap[v1][v2] = i;
        edgeMap[v2][v1] = i;
    }
    //初始化
    for(int i = 1; i <= n; ++i)
    {
        dist[i][0] = edgeMap[1][i];
        dist[i][1] = 1;
        vertexMark[i] = false;
        edgeMark[i] = false;
    }
    vertexMark[1] = true;
    edgeMark[0] = false;
    int vertexCount = 1;
    while(vertexCount < n)
    {
        int min = INF;
        int k = -1;
        for(int i = 0; i <= n; ++i)
        {
            if(vertexMark[i] == 0 && min > dist[i][0])
            {
                min = dist[i][0];
                k = i;
            }
        }
        if(k == -1)
            break;
        edgeMark[min] = true;
        vertexMark[k] = true;
        ++vertexCount;
        for(int i = 1; i <= n; ++i)
        {
            if(vertexMark[i] == false && dist[i][0] > edgeMap[k][i])
            {
                dist[i][0] = edgeMap[k][i];
                dist[i][1] = k;
            }
        }
    }
    for(int i = n - 1; i >= 0; --i)
    {
        if(edgeMark[i] == false)
            return edges[i];
    }
    return edges[n - 1];
}

int main() {
    /*
     * 1 — 2 — 3
     *     |   |
     *     5 — 4
     */
    vector<vector<int>> edges = {{1, 2},
                                 {2, 3},
                                 {2, 5},
                                 {4, 5},
                                 {3, 4}
                                };
    vector<int> edge = findRedundantConnection(edges);
    print_vec(edge);
} // 应当输出3 4
