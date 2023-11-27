#include "entry.h"
#include <iostream>
#include <string>
using namespace std;
SymEntry::SymEntry(){
this->key="";
this->left=nullptr;
this->right=nullptr;
this->val=nullptr;
}
SymEntry::SymEntry(string k, UnlimitedRational* v){
this->key=k;
this->left=nullptr;
this->right=nullptr;
this->val=v;
}
SymEntry::~SymEntry(){
    delete val;
    this->key="";
};