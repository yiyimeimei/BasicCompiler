#include<QCoreApplication>
#include<iostream>
using namespace std;


struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
  };

void output(TreeNode *root)
{
    if(!root)
    {
        return;
    }
    cout<<root->val<<"...";
    cout<<"left";
    output(root->left);
    cout<<"right";
    output(root->right);
}






struct ListNode {
      int val;
      ListNode *pre;
      ListNode *next;
      ListNode() : val(0), pre(nullptr), next(nullptr) {}
      ListNode(int x) : val(x), pre(nullptr), next(nullptr) {}
      ListNode(int x, ListNode *pre, ListNode *next) : val(x), pre(pre), next(next) {}
};


ListNode *tail=nullptr;
ListNode *head=new ListNode(0);
TreeNode* sortedListToBST(ListNode* head)
{
    if(head==nullptr)
    {
        return nullptr;
    }
    if(head->next==nullptr&&head!=nullptr)
    {
        TreeNode *root=new TreeNode(head->val);
        return root;
    }
    ListNode *p1=head;
    ListNode *p2=head;
    ListNode *p3=head;
    while(p2!=nullptr&&p2->next!=nullptr)
    {
        p3=p1;
        p1=p1->next;
        p2=p2->next->next;
    }

    TreeNode *root=new TreeNode(p1->val);
    p3->next=nullptr;
    root->left=sortedListToBST(head);
    root->right=sortedListToBST(p1->next);
    return root;
}








void append(ListNode *p)
{
    ListNode *temp=head;
    while(temp->next!=nullptr)
    {
        temp=temp->next;
    }
    temp->next=p;
    p->pre=temp;
    return;
}




void Traverse(TreeNode *root)
{
    if(root->left)
    {
        Traverse(root->left);
    }
    else
    {
        return;
    }

    ListNode *p=new ListNode(root->val);
    append(p);

    if(root->right)
    {
        Traverse(root->right);
    }
    else
    {
        return;
    }

}
TreeNode* balanceBST(TreeNode* root) {
    Traverse(root);
    return sortedListToBST(head);

}




int main()
{
    ListNode *p1=new ListNode(9);
    ListNode *p2=new ListNode(5,nullptr,p1);
    ListNode *p3=new ListNode(0,nullptr,p2);
    ListNode *p4=new ListNode(-3,nullptr,p3);
    ListNode *p5=new ListNode(-10,nullptr,p4);
    ListNode *p6=new ListNode(-20,nullptr,p5);
    ListNode *head=p6;
    while(head)
    {
        cout<<head->val<<' ';
        head=head->next;
    }
    TreeNode *root=sortedListToBST(p6);
    output(root);
    return 0;
}
