#include<vector>
template<typename K, typename V>
struct Node{
    Node<K, V> *right,*down;   //向右向下足矣
    K key;
    V val;
    Node(Node<K, V> *right,Node<K, V> *down, K key, V val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
};

template<typename K, typename V>
class Skiplist {
public:
    Node<K, V> *head;
    int nodeSum=0;
    Skiplist()
    {
        head = new Node<K, V>();  //初始化头结点
    }

    int size()
    {
        return nodeSum;
    }

    V* get(const K& key)
    {
        Node<K, V> *p = head;
        while(p!=nullptr)
        {
            while(p->right && p->right->key < key)
            {
                p = p->right;
            }
            if(p->right!=nullptr && p->right->key==key)
            {
                return (&(p->right->val));
            }
            else
            {
                p = p->down;
            }
        }
        return nullptr;
    }

    void put(const K& key, const V& val)
    {
        vector<Node<K, V>*> pathList;    //从上至下记录搜索路径
        Node<K, V> *p = head;
        while(p)
        {
            while(p->right && p->right->key < key)
            {
                p = p->right;
            }
            pathList.push_back(p);
            p = p->down;
        }
        bool insertUp = true;
        Node<K, V>* downNode= nullptr;
        while(insertUp && pathList.size() > 0)//从下至上搜索路径回溯，50%概率
        {
            Node<K, V> *insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node<K, V>(insert->right, downNode, key, val); //add新结点
            downNode = insert->right;    //把新结点赋值为downNode
            insertUp = (rand()&1);   //50%概率
        }
        if(insertUp)//插入新的头结点，加层
        {
            Node<K, V> * oldHead = head;
            head = new Node<K, V>();
            head->right = new Node<K, V>(NULL, downNode, key, val);
            head->down = oldHead;
        }
        nodeSum++;
    }

    bool remove(const K& key)
    {
        bool emptyFlag=false;
        if(!nodeSum)
        {
            return false;
        }
        Node<K, V> *p = head;
        while(p!=nullptr)
        {
            while(p->right && p->right->key < key)
            {
                p = p->right;
            }
            if(p->right!=nullptr && p->right->key==key)//找到了
            {
                Node<K, V> *pRight = p->right;
                if(pRight->right==nullptr&&p==head)
                {
                    emptyFlag=true;
                }
                while(1)//delete node
                {

                    p->right=pRight->right;
                    Node<K, V> *uselessNode = pRight;
                    pRight=pRight->down;
                    delete uselessNode;
                    if(pRight==nullptr)
                    {
                        break;
                    }
                    p=p->down;
                    while(p->right!=pRight)
                    {
                        p=p->right;
                    }
                }
                if(emptyFlag)
                {
                    Node<K, V> *uselessNode = head;
                    head=head->down;
                    delete uselessNode;
                }
                nodeSum--;
                return true;
            }
            else
            {
                p = p->down;
            }
        }
        return false;

    }
};
