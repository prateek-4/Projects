#include "exprtreenode.h"
#include <iostream>
#include <string>
using namespace std;
ExprTreeNode::ExprTreeNode(){
    this->type="";
    this->val=nullptr;
    this->evaluated_value=new UnlimitedRational();//i decided to intialise the default values of expression tree nodes as 0
    this->left=nullptr;
    this->right=nullptr;
}
ExprTreeNode::ExprTreeNode(string t,UnlimitedInt * v){
    UnlimitedInt* temp=new UnlimitedInt(1);
    this->type=t;
    this->val = new UnlimitedRational (v,temp);
    this->left=nullptr;
    this->right=nullptr;
    this->evaluated_value=nullptr;
    delete temp;
}
ExprTreeNode::ExprTreeNode(string t, UnlimitedRational*v){
    this->type=t;
    this->val=v;
    this->left=nullptr;
    this->right=nullptr;
    this->evaluated_value=nullptr;

}
ExprTreeNode::~ExprTreeNode(){
    delete val;//should only delete pointers to value only
    //should not delete left and right pointers
    this->type="";
}