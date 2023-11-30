/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"

//Write your code below this line
#include <iostream>

// class queue
//............


class queue {
public:
    int size;
    HeapNode ** data;
    int Front;
    int rear;
    queue(int k) {
        size=k;
        data=new HeapNode*[k];
        Front=-1;
        rear=-1;
    }
    ~queue() {
        delete[] data;
    }
    
    void push(HeapNode* value) {
        if (is_full())
            return;
        //if(front==rear)//only one element is present
        if(Front==-1&&rear==-1)
            Front=0;
        if(rear==size-1)
            rear=-1;
        rear++;
        data[rear]=value;
    }
    
    void pop() {
        if(empty())
            return ;
        else if(Front==rear)//only one element is present
            {Front =-1;rear=-1;}
        else if(Front==size-1){
            Front=0;}
        else{Front++;}
    }
    
    HeapNode* front() {
        if(empty()){
            return NULL;//can change accroding to thte data type 
        }
        return data[Front];
    }
    
    HeapNode* Rear() {
        if(empty())
            return NULL;
        return data[rear];  
    }
    
    bool empty() {
        return (Front==-1);
    }
    
    bool is_full() {
        return ((Front==0)&&(rear==size-1))||(rear==Front-1);
    }
};


//class functions
//................


MinHeap::MinHeap(){
    size = 0;
    root = nullptr;
}

void MinHeap::push_heap(int num){
    // Special case of an empty tree
    if (!root) {
        root = new HeapNode(num);
        size = 1;
        return;
    }
    // We need to do a level order traversal to find the next free
    // position i.e. the first node with either left or right free.
    HeapNode* parent = nullptr;
    queue q(500000);
    q.push(root);
 
    while (!q.empty()) {
        // If we found a suitable node exit the loop
        parent = q.front();
        q.pop();
        if (!parent->left || !parent->right) {
            break;
        }
 
        // Queue up the child nodes
        if (parent->left)
            q.push(parent->left);
        if (parent->right)
            q.push(parent->right);
    }

    // At this point parent is the node to add under
    HeapNode* child = new HeapNode(num);
    child->par = parent;
    if (!parent->left)
        parent->left = child;
    else
        parent->right = child;

    // Finally swap the child up as far as it needs to go
    while (parent && parent->val > child->val) {
        int i = parent->val;
        parent->val = child->val;
        child->val = i;
        child = parent;
        parent = parent->par;
    }

    size++;
}

int MinHeap::get_min(){
    if (root)
        return root->val;
    else
        return -1;
}

void MinHeap::pop(){
    // Special case of an empty tree
    if (!root) {
        return;
    }
    // Special case of just the root
    if (!root->left && !root->right) {
        delete root;
        root = nullptr;
        size = 0;
        return;
    }
    // Special case of just the root and the left node
    if (root->left && !root->right) {
        root->val = root->left->val;
        delete root->left;
        root->left = nullptr;
        size = 1;
        return;
    }

    // We need to do a level order traversal to find the last node and
    // the minimum node. Then we can put the last node in the min node,
    // the min node at the root, then delete the last node.
    HeapNode* last_node = nullptr;
    HeapNode* min_node = root->left;
    queue q(500000);
    q.push(root->left);
    q.push(root->right);
 
    while (!q.empty()) {
        HeapNode* n = q.front();
        q.pop();

        // Is this the last mode?
        if (!n->left || !n->right)
            last_node = n;
        // Is this the minimum node?
        if (n->val < min_node->val)
            min_node = n;
 
        // Queue up the child nodes
        if (n->left)
            q.push(n->left);
        if (n->right)
            q.push(n->right);
    }

    // Put the minimum at the root
    root->val = min_node->val;
    // Put the last node where the minimum was
    min_node->val = last_node->val;
    // And delete the last node
    if (last_node->par->left == last_node)
        last_node->par->left = nullptr;
    else if (last_node->par->right == last_node)
        last_node->par->right = nullptr;
    delete last_node;
}

MinHeap::~MinHeap(){

}