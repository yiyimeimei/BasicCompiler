#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define MAX_LENGTH 2147483647
int dx[4]={0,1,-1,0};
int dy[4]={1,0,0,-1};
/*void DFS(vector<vector<int>> &grid, int k, vector<vector<int>> &cost, vector<vector<int>> &step, vector<vector<bool>> &flag, int x, int y)
{
    flag[x][y] = true;//更新结点访问状态
    int row = grid.size();//n
    int col = grid[0].size();//m
    if(x == row - 1 && y == col - 1)
    {
        if(step[x][y] < shortestPath && cost[x][y] <= k)
            shortestPath = step[x][y];
        flag[x][y] = false;
        return;
    }
    for(int i = 0; i < 4; ++i)
    {
        int newx = x + dx[i];
        int newy = y + dy[i];
        if(newx < 0 || newx >= row || newy < 0 || newy >= col || flag[newx][newy])
            continue;
        if(grid[newx][newy])//有障碍物
        {
            if(cost[x][y]<k)
            {
                cost[newx][newy] = cost[x][y] + 1;//增加消除障碍数
                step[newx][newy] = step[x][y] + 1;//增加步数
                if(step[newx][newy] < shortestPath)
                    DFS(grid, k, cost, step, flag, newx, newy);
            }
        }
        else//没有障碍物
        {
            cost[newx][newy] = cost[x][y];//消除障碍数不变
            step[newx][newy] = step[x][y] + 1;//增加步数
            if(step[newx][newy] < shortestPath)
                DFS(grid, k, cost, step, flag, newx, newy);
        }
    }
    flag[x][y] = false;//还原该结点的访问状态
    return;
}*/
struct node
{
    int x;
    int y;
    int cost;//当前清除障碍数
    int step = MAX_LENGTH;//当前最短步数
    node(int a, int b, int c, int d):x(a), y(b), cost(c), step(d){}
};

int shortestPathInGrid(vector<vector<int>> &grid, int k)
{
    int row = grid.size();//n
    int col = grid[0].size();//m
    if(row <= 0 || col <= 0)
        return -1;
    if(row == 1 && col == 1)
        return 0;
    vector<vector<int>> flag(row);//标记一个结点是否已经被访问过，访问过为true，未访问过为false
    for (int i = 0; i < row; ++i)
    {
        flag[i].resize(col,0);
    }
    flag[0][0] = 1;
    queue<node> q;
    node start(0, 0, 0, 0);
    q.push(start);
    while(!q.empty())
    {
        node currentNode = q.front();
        q.pop();
        if(currentNode.x == row - 1 && currentNode.y == col - 1)
            return currentNode.step;
        for(int i = 0; i < 4; ++i)
        {
            int newx = currentNode.x + dx[i];
            int newy = currentNode.y + dy[i];
            if(newx < 0 || newx >= row || newy < 0 || newy >= col || flag[newx][newy] == 2)
                continue;
            if(grid[newx][newy])//有障碍物
            {
                if(currentNode.cost < k)
                {
                    flag[newx][newy] = 1;
                    node newNode(newx, newy, currentNode.cost + 1, currentNode.step + 1);
                    q.push(newNode);
                }
            }
            else
            {
                flag[newx][newy] = 1;
                node newNode(newx, newy, currentNode.cost, currentNode.step + 1);
                q.push(newNode);
            }
        }
        flag[currentNode.x][currentNode.y] = 2;
    }
    return -1;
}
int main()
{

    int r,c,k;
    cin>>r>>c>>k;
    vector<vector<int>> array(r);//创建一个有row行的二维数组arr
    for (int i = 0; i < r; i++)
    {
        array[i].resize(c);
    }
    for(int i=0;i<r;++i)
    {
        for(int j=0;j<c;++j)
            cin>>array[i][j];
    }
    cout<<shortestPathInGrid(array,k)<<endl;
    return 0;
}




struct point{
  int x,y,count;
  point(int nx,int ny,int n):x(nx),y(ny),count(n){}
};

int shortestPathInGrid1(vector<vector<int>> &grid, int k) {
  int n=grid.size();
  int m=grid[0].size();
  if(n<=0||m<=0)
    return -1;
  if(n==1&&m==1)
    return 0;
  int dx[4]={-1,0,1,0};
  int dy[4]={0,1,0,-1};
  vector<vector<int>> visited(n,vector<int>(m,-1));
  queue<point> q;
  visited[0][0]=k;
  q.push(point(0,0,0));
  int step=0;
  while(!q.empty()){
    step++;
    int size=q.size();
    for(int i=0;i<size;i++){
      point cur=q.front();
      q.pop();
      for(int j=0;j<4;j++){
        int newx=cur.x+dx[j];
        int newy=cur.y+dy[j];
        if(newx<0||newx>=n||newy<0||newy>=m)
          continue;
        if(newx==n-1&&newy==m-1)
          return step;
        int newcount=cur.count;
        if(grid[newx][newy]==1){
          if(cur.count>=k)
            continue;
          else
            newcount++;
        }
        if(visited[newx][newy]!=-1&&visited[newx][newy]>=k-newcount)
          continue;
        visited[newx][newy]=k-newcount;
        q.push(point(newx,newy,newcount));
      }
    }
  }

  return -1;
}
