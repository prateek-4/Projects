/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

//Write your code below this line
int height1(SymNode* node){
    if(node==NULL)
        return 0;
    return node->height;
}

SymNode::SymNode(){
    this->key="";
    this->height=-1;//for a null node the height is -1 and 0 for a leaf node
    this->par=NULL;
    this->left=NULL;
    this->right=NULL;

}

SymNode::SymNode(string k){
    this->key=k;
    this->height =0;
    this->par=NULL;
    this->left=NULL;
    this->right=NULL;
}

SymNode* SymNode::LeftLeftRotation() {//rotates to right
    SymNode* x = this->left;
    SymNode* T2 = x->right;

    // Perform rotation
    x->right = this;
    this->left = T2;

    // Update heights
    this->height = 1 + max(height1(this->left), height1(this->right));
    x->height = 1 + max(height1(x->left), height1(x->right));

    
    // Return new root
    return x;
}

SymNode* SymNode::RightRightRotation() {//rotates to left
    SymNode* y = this->right;
    SymNode* T2 = y->left;

    // Perform rotation
    y->left = this;
    this->right = T2;

    // Update heights
    this->height = 1 + max(height1(this->left), height1(this->right));
    y->height = 1 + max(height1(y->left), height1(y->right));

    // Return new root
    return y;
}


SymNode* SymNode::LeftRightRotation(){
    this->left=this->left->RightRightRotation();
   
    return this->RightRightRotation();
}

SymNode* SymNode::RightLeftRotation(){
    this->right=this->right->LeftLeftRotation();
    return this->LeftLeftRotation();
}

SymNode::~SymNode(){};

