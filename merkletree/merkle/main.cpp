#include <iostream>
#include <vector>
using namespace std;

unsigned long hash64shift(unsigned long key)
{
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}
class Merkle_Tree
{
private:
    struct treeNode
    {
        unsigned long hash;
        treeNode *left;
        treeNode *right;
        treeNode *parent;
        treeNode(): hash(0),left(nullptr),right(nullptr),parent(nullptr){}
        treeNode(unsigned long value): hash(value),left(nullptr),right(nullptr),parent(nullptr){}
        treeNode(unsigned long value,treeNode *p1,treeNode *p2):hash(value),left(p1),right(p2),parent(nullptr){}
    };
    vector<treeNode*> leafNode;
    treeNode *root;




public:
    Merkle_Tree();//构造空树
    Merkle_Tree(vector<unsigned long>);//给定叶结点
    vector<unsigned long> getProof(unsigned long hash_value);
    void addTransaction(unsigned long);//插入新节点，更新hash值
    unsigned long getRootHash();
};


Merkle_Tree::Merkle_Tree()
{
    leafNode.clear();
    root=nullptr;
}
Merkle_Tree::Merkle_Tree(vector<unsigned long> leafHash)
{
    unsigned long i,len;
    len=leafHash.size();
    leafNode.clear();
    if(len==0)
    {
        root=nullptr;
    }
    else if(len==1)
    {
        treeNode* newNode=new treeNode(leafHash[0]);
        root=new treeNode(leafHash[0],newNode,nullptr);
        newNode->parent=root;
        leafNode.push_back(newNode);
    }
    else
    {
        vector<treeNode*> branch;
        branch.clear();
        treeNode* newNode;
        for(i=0;i<len;++i)
        {
            newNode=new treeNode(leafHash[i]);
            leafNode.push_back(newNode);
            branch.push_back(newNode);
        }
        unsigned long start=0;//每一层结点在branch中的起始位置
        for( ;len>1;len=(len+1)/2)//一层一层构造结点
        {
            for(i=0;i<len;i+=2)//遍历该层结点的上一层结点
            {
                if(i+1>len-1)//仅剩一个结点
                {
                    newNode=new treeNode(branch[start+i]->hash,branch[start+i],nullptr);
                    branch[start+i]->parent=newNode;
                    branch.push_back(newNode);
                }
                else
                {
                    newNode=new treeNode(hash64shift(branch[start+i]->hash+branch[start+i+1]->hash),branch[start+i],branch[start+i+1]);
                    branch[start+i]->parent=newNode;
                    branch[start+i+1]->parent=newNode;
                    branch.push_back(newNode);
                }
            }
            start+=len;
        }
        root=branch[branch.size()-1];
        branch.clear();
    }
}
unsigned long Merkle_Tree::getRootHash()
{
    if(root==nullptr)
    {
        return 0;
    }
    return root->hash;
}
void Merkle_Tree::addTransaction(unsigned long leafHash)
{
    treeNode* newNode=new treeNode(leafHash);
    if(root==nullptr)
    {
        root=new treeNode(leafHash,newNode,nullptr);
        newNode->parent=root;
        leafNode.push_back(newNode);
        return;
    }
    treeNode* lastNode=leafNode[leafNode.size()-1];
    treeNode* lastParent=lastNode->parent;
    leafNode.push_back(newNode);//更新叶结点链表
    while(lastParent!=nullptr)
    {
        if(lastParent->right==nullptr)//最右边父结点右儿子为空，直接添加
        {
            lastParent->right=newNode;
            newNode->parent=lastParent;
            lastParent->hash=hash64shift(lastParent->left->hash+leafHash);
            lastParent=lastParent->parent;
            while(lastParent!=nullptr)//向上更新hash值
            {
                if(lastParent->right==nullptr)
                {
                    lastParent->hash=lastParent->left->hash;
                }
                else
                {
                    lastParent->hash=hash64shift(lastParent->left->hash+lastParent->right->hash);
                }
                lastParent=lastParent->parent;
            }
            return;
        }
        else//最右边父结点右儿子不为空，此时newNode向上生长，继续检索该父结点的父结点
        {
            treeNode* newParent=new treeNode(leafHash,newNode,nullptr);
            newNode->parent=newParent;
            newNode=newParent;
            lastParent=lastParent->parent;
        }
    }
    //如果原先的二叉树已满，则找到root都无法找到右儿子为空的父结点，此时向上生长出一个新的root
    treeNode* newRoot=new treeNode(hash64shift(root->hash+leafHash),root,newNode);
    root->parent=newRoot;
    newNode->parent=newRoot;
    root=newRoot;
    return;
}
vector<unsigned long> Merkle_Tree::getProof(unsigned long hash_value)
{

    vector<unsigned long> path;
    path.clear();
    if(!leafNode.size())
    {
        return path;
    }
    treeNode* targetNode=nullptr;
    unsigned long len=leafNode.size();
    for(unsigned long i=0;i<len;++i)
    {
        if(leafNode[i]->hash==hash_value)
        {
            targetNode=leafNode[i];
            break;
        }
    }
    if(targetNode==nullptr)
        return path;

    treeNode* parent=targetNode->parent;
    while(parent!=nullptr)
    {
        if(parent->left==targetNode)//当前结点是父结点的左儿子
        {
            if(parent->right==nullptr)
            {
                path.push_back(0);
            }
            else
            {
                path.push_back(parent->right->hash);
            }
        }
        else//当前结点是父结点的右儿子
        {
            path.push_back(parent->left->hash);
        }
        targetNode=parent;
        parent=parent->parent;
    }
    return path;
}




/*int main()
{
    vector<unsigned long> hash;
    hash.clear();
    hash.push_back(15);
    Merkle_Tree kong(hash);
    cout<<kong.getRootHash()<<endl;
    vector<unsigned long> num;
    num.clear();
    num=kong.getProof(15);
    for(int i=0;i<num.size();++i)
    {
        cout<<num[i]<<endl;
    }
    kong.addTransaction(2);
    cout<<kong.getRootHash()<<endl;
    cout<<hash64shift(17)<<endl;
    num.clear();
    num=kong.getProof(15);
    for(int i=0;i<num.size();++i)
    {
        cout<<num[i]<<endl;
    }

    vector<unsigned long> hash;
    hash.clear();
    for(int i=1;i<=8;++i)
    {
        unsigned long x=hash64shift(i);
        hash.push_back(x);
        //cout<<x<<endl;
    }
    Merkle_Tree merkle(hash);
    cout<<"roothash:"<<merkle.getRootHash()<<endl;
    vector<unsigned long> num;
    num.clear();
    num=merkle.getProof(hash64shift(5));
    cout<<"getproof"<<endl;
    for(int i=0;i<num.size();++i)
    {
        cout<<num[i]<<endl;
    }
    cout<<"add"<<endl;
    //merkle.addTransaction(123456789);
    //merkle.addTransaction(321456789);
    //merkle.addTransaction(123654789);
    //merkle.addTransaction(123456987);
    //merkle.addTransaction(987654321);
    //merkle.getProof(123456789);

    num.clear();
    num=merkle.getProof(2);
    cout<<"getproof"<<endl;
    for(int i=0;i<num.size();++i)
    {
        cout<<num[i]<<endl;
    }

    cout<<"roothash:"<<merkle.getRootHash()<<endl;

    cout<<"end"<<endl;
    //cout<<hash64shift(3377197545+9)<<endl;
    //cout<<hash64shift(549100223+3377197545)<<endl;
    return 0;
}*/
