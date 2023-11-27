//possible sources of error delete in symtable
#include "symtable.h"
#include <iostream>
#include <string>
using namespace std;
#define TABLE_SIZE 100000
SymbolTable::SymbolTable(){
    this->size=0;
    this->root=nullptr;
};
void delete_node(SymEntry * root){
    if(!root)
        return;
    delete_node(root->left);
    delete_node(root->right);
    delete root->val;
    delete root;    
}
SymbolTable::~SymbolTable(){
    delete_node(this->root);
    //this->size=0;
};
SymEntry* search_t(string key, SymEntry* v){
    //cout<<"now searching root!!"<<endl;
    if(!v){//cout<<"key not found"<<endl;
        return nullptr;}
    if(key==v->key)
    {//cout<<"key found!!"<<endl;
        return v;}
    else if(key<v->key){
        //cout<<"now searching left!!"<<endl;
        return search_t(key,v->left);
    }
    
    else {//cout<<"now searching right!!"<<endl;
        return search_t(key,v->right);};
}
UnlimitedRational*SymbolTable::search (string key){
    return search_t(key,this->root)->val;

}
void SymbolTable::remove(string key){
    SymEntry* find=search_t(key,this->root);
    if(find==NULL)//if the key doesnt exist do nothing
        return;
    if(find->left==NULL&&find->right==NULL){
        delete find->val;
        delete find;
    }
    if(find->left==NULL){//replace the node with the right node 
        SymEntry* temp=find->right;
        find->key=temp->key;
        find->val=temp->val;
        find->right=temp->right;
        find->left=temp->left;
        delete temp->val;//delete the extra copy of the right entry
        delete temp;
    }
    if(find->right==NULL){//replace the node with the left node 
        SymEntry* temp=find->left;
        find->key=temp->key;
        find->val=temp->val;
        find->right=temp->right;
        find->left=temp->left;
        delete temp->val;//delete the extra copy of the left entry
        delete temp;
    }
    else{//if both the child exists
        SymEntry* succpar=find->right;
        SymEntry* succ=find->right;
        while(succ->left!=NULL){//iteratively search for the succesor
            succpar=succ;
            succ=succ->left;
        }
        find->key=succ->key;
        find->val=succ->val;
        succpar->left=succ->right;//make the neccessary connections
        delete succ->val;
        delete succ;//delete extra copy of the temp
    }
    
}
SymEntry* SymbolTable::get_root(){
    return this->root;
}

int SymbolTable::get_size(){
    return this->size;
}
void SymbolTable::insert(string key,UnlimitedRational*v){
    //base case 
    if (this->root==nullptr){//if empty root is given
        //cout<<"insert at root success"<<endl;
        this->root=new SymEntry(key,v);
        
        this->size+=1;
        //cout<<"base case is called"<<endl;
        return ;
    }
    this->size+=1;//update the size parameter
    //else search if the key exists
        SymEntry* current=this->root;
        SymEntry* prev=this->root;
        while(current!=NULL&&current->key!=key){//search iteratively for the root
            if(current->key<key){
                //cout<<"searching right node to insert"<<endl;
                prev=current;
                current=current->right;}
            else   
                {   prev=current;
                    current=current->left;
                //cout<<"searching left node to insert"<<endl;
                }
        }
        //update the current to represent the key value even if the key does not exist it wont matter
        if(current==NULL){
            if(prev->key<key){
                prev->right=new SymEntry(key,v);
                }
            else   
                {   prev->left=new SymEntry(key,v);
                
                }
            //cout<<"inserted to null node success"<<endl;
            }
        else{//cout<<"insert to found node success"<<endl;
            current->val=v;}
}
