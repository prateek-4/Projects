/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

//Write your code below this line

ExprTreeNode::ExprTreeNode(){
    this->id="";
    this->left=NULL;
    this->right=NULL;
    this->type="";
    this->num=-1;
}

ExprTreeNode::ExprTreeNode(string t,int v){
    this->type=t;
    this->left=NULL;
    this->right=NULL;
    this->num=v;
    this->id="";
}

ExprTreeNode::~ExprTreeNode(){

}

