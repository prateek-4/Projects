/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"
#include <iostream>
using namespace std;
//Write your code below this line


//...........................................
// helper functions 
int height1(SymNode* root);
int get_balance(SymNode* root) {
    if(root==NULL)
        return 0;
    return height1(root->left)- height1(root->right);
}
SymNode* newNode(string key){
    SymNode* node = new SymNode(key);
    node->height=1;
    //cout<<"success making new node"<<endl;
    return node;
  
}
 
SymNode *rightRotate(SymNode *node) 
{ 
    SymNode *x = node->left; 
    SymNode *T2 = x->right; 
 
    // Perform the rotation 
    x->right =node; 
    node->left =T2; 
 
    // Update heights 
    node->height = max(height1(node->left), 
                    height1(node->right)) + 1; 
    x->height = max(height1(x->left), 
                    height1(x->right)) + 1; 
 
    // Return new root 
    return x; 
} 
 

SymNode *leftRotate(SymNode *node) 
{ 
    SymNode *x = node->right; 
    SymNode *T2 = x->left; 
 
    // Perform rotation 
    x->left = node; 
    node->right = T2; 
 
    // Update heights 
    node->height = max(height1(node->left), 
                    height1(node->right)) + 1; 
    x->height = max(height1(x->left), 
                    height1(x->right)) + 1; 
 
    // Return new root 
    return x; 
} 
SymNode* insert_help(SymNode* root , string key,int & size){//recursive function to insert a key and return root
    //cout<<"now using insert help"<<endl;
    if(root==NULL){
        size++;//at this stage the size is updated
        //cout<<"while insertion root is null"<<endl;
        return newNode(key);
    }
    if(key<root->key)
        { //  cout<<"key is smaller than the root"<<endl;
            SymNode* temp=insert_help(root->left,key,size);
            temp->par=root;
            root->left=temp;
        }
    else if(key>root->key)
        {//cout<<"key is larger than the key"<<endl;
            SymNode* temp=insert_help(root->right,key,size);
            temp->par=root;
            root->right=temp;
        }
    else if(key==root->key){
        //no need to update the size 
        return root;}
    //update the heights
    root->height = 1 + max(height1(root->left),height1(root->right));//be careful if either of the child is missing
    int bal= get_balance(root);
    // cout<<"balance is:"<<bal<<endl;
     // Left Left Case
    if (bal > 1 && key < root->left->key)
        {
        // cout<<"left left case"<<endl;
        return rightRotate(root);}
    
    // Left Right Case
    if (bal > 1 && key > root->left->key)
        {
        // cout<<"left right case"<<endl;
        root->left=leftRotate(root->left);
        return rightRotate(root);
        }
 

    // Right Right Case
    if (bal < -1 && key > root->right->key)
       {
        // cout<<"right right case"<<endl;
        return leftRotate(root);}
    
    // Right Left Case
    if (bal < -1 && key < root->right->key)
        {
        // cout<<"right left case"<<endl;
        root->right=rightRotate(root->right);
        return leftRotate(root);}
    
    /* return the (unchanged) node pointer */
    //cout<<"success insertion"<<endl;
    return root;

}

SymNode * min_value(SymNode* root) 
{ 
    SymNode* current = root; 

    while (current->left!= NULL) 
        current = current->left; 
     return current; 
} 
 
 
SymNode* delete_help(SymNode* root, string key,int & size,int & size1) 
{ 
     //cout<<"initialising delete_help"<<endl;
    // base case 
    if (root == NULL) {
        //cout<<"root is NULL"<<endl;
        return root; }
 
   
    if ( key < root->key ) 
        root->left = delete_help(root->left, key,size,size1); 
 
    else if( key > root->key ) 
        root->right = delete_help(root->right, key,size,size1); 
 
    else
    {   size--;
        // node with only one child or no child 
        if( (root->left == NULL) ||
            (root->right == NULL) ) 
        {    
            SymNode *temp = root->left ? 
                         root->left : 
                         root->right; 
 
            // No child case 
            if (temp == NULL) 
            { 
                temp = root; 
                root = NULL; 
            } 
            else // One child case 
            *root = *temp; // Copy the contents of 
                           // the non-empty child 
            delete temp; 
        } 
        else
        {   
            SymNode* temp = min_value(root->right); 

            root->key = temp->key; 
 
            root->right = delete_help(root->right, 
                                     temp->key,size1,size1); 
        } 
    } 
 
    
    if (root == NULL) 
        return root; 
 
    // update the heights
    root->height = 1 + max(height1(root->left), 
                           height1(root->right)); 
 
    // get balance todecide the case of rotation
    int balance = get_balance(root); 
 

 
    // Left Left Case 
    if (balance > 1 && get_balance(root->left) >= 0) 
        return rightRotate(root); 
 
    // Left Right Case 
    if (balance > 1 && get_balance(root->left) < 0) 
    { 
        root->left = leftRotate(root->left); 
        return rightRotate(root); 
    } 
 
    // Right Right Case 
    if (balance < -1 && get_balance(root->right) <= 0) 
        return leftRotate(root); 
 
    // Right Left Case 
    if (balance < -1 && get_balance(root->right) > 0) 
    { 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    } 
 
    return root; }
SymNode* search_t(SymNode*root,string k){
    if(root==NULL){
        //cout<<"root is null!!"<<endl;
        return NULL;}
    if(k<root->key){
        //cout<<"now searching left"<<endl;
        return search_t(root->left,k);}
    if(k>root->key){
        //cout<<"now searching right"<<endl;
        return search_t(root->right,k);}
    //cout<<"key found!!"<<endl;
    return root;
}
void delete_node(SymNode* root){//helper function to delete all the nodes of the tree
    if(root==NULL)
        return;
    
    if(root->left!=NULL)    
        delete_node(root->left);
    if(root->right!=NULL)
        delete_node(root->right);
    delete root;
}
 
void preorderTraversal(SymNode* root) {
    if (root != NULL) {
        cout << root->key << " ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
    
}



//.............................
// class functions



SymbolTable::SymbolTable(){
    this->size=0;
    this->root=NULL;
}

void SymbolTable::insert(string k){
    //cout<<"now using insert"<<endl;
    this->root=insert_help(root,k,size);
    //cout<<"success insertion"<<endl;
    // cout<<"preorder traversal no :"<<size<<endl;
    // preorderTraversal(root);
    //cout<<"success inserting : "<<k<<endl;
}   


void SymbolTable::remove(string k){
    int size1=0;//just a variable to help maintain the correct size
    this->root= delete_help(this->root,k,size,size1);
}

int SymbolTable::search(string k){
    //cout<<"now searching for ; "<<k<<endl;
    SymNode* temp =search_t(this->get_root(),k);
    if(temp==NULL){
        //cout<<"no node found with this key while searching for address"<<endl;
        return -2;}
    //cout<<"address found is "<<temp->address<<endl;
    return temp->address;
}

void SymbolTable::assign_address(string k,int idx){
    SymNode * temp =search_t(this->get_root(),k);
    if(temp==NULL){
        //cout<<"no node found with this key"<<endl;
        return;}
    //cout<<"assignment for var : "<<temp->key<<" and the assigned adress is : "<<idx<<endl;
    temp->address=idx;
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}

SymbolTable::~SymbolTable(){
    delete_node(root);
}
