#include "evaluator.h"

#include <iostream>
#include <string>
using namespace std;
#define MAX_STACK_SIZE 100
UnlimitedInt* ONE = new UnlimitedInt(1);
UnlimitedInt* ZEROINT = new UnlimitedInt(0);
UnlimitedRational* ZERO = new UnlimitedRational(ZEROINT,ONE) ;
int presendence(string op){//code similar to infix to postfix evaluator
    if(op=="ADD"||"SUB")//+,- have lower priority than division, multiplication and modular divivision
        return 1;
    if(op=="MUL"||"DIV")
        return 2;//return value according to presendence
    return 0;
}
bool isOperator(string s) {
    char c=s[0];
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}
string type_op(string t){//for populating symbol tables
        if(t=="*")
            return "MUL";
        if(t=="-")
            return "SUB";
        if(t=="+")
            return "ADD";
        if(t=="/")
            return "DIV";
        if(t=="("||t==")")
            return "NULL";
         bool is_val = true;
        for (char c : t) {
            if (!(is_digit(c) || (c == '-' && is_val))) {
                is_val = false;
                break;
            }
        }
        if (is_val) {
            return "VAL";
        }
        return "VAR";
        
}
Evaluator::Evaluator(){
    symtable= new SymbolTable();
    //cout<<"symbol table made success"<<endl;
}
Evaluator::~Evaluator(){
//delete symtable;
for(ExprTreeNode* node:expr_trees){
    delete node;
}}
void eulerTraversal(ExprTreeNode* root) {//only for debugging
    if (root != nullptr) {
        std::cout << root->type;

        if (root->type == "VAL") {
            std::cout << "(" << root->val->get_p_str() << ")";
        }

        if (root->left != nullptr || root->right != nullptr) {
            std::cout << "(";
            eulerTraversal(root->left);
            std::cout << ",";
            eulerTraversal(root->right);
            std::cout << ")";
        }
    }
}
void postorder(ExprTreeNode* root)
{
    if (root!=nullptr)
    {
        postorder(root->left);
        postorder(root->right);
        cout << root->type<<" ";
    }
}

// Parse tree rules
// If the current token is a '(', add a new node as the left child of the current node, and descend to the left child.
// If the current token is a ')', go to the parent of the current node.
// If the current token is an operator, set the root value of the current node to the operator represented by the current token. Add a new node as the right child of the current node and descend to the right child.
// If the current token is a number, set the root value of the current node to the number and return to the parent.

// Helper function to find the parent of a node
static ExprTreeNode* nodeFindParent(ExprTreeNode* node, ExprTreeNode* root) {
    // Check if the current node is the parent
    if (root->left == node || root->right == node)
        return root;
    // Check the left branch
    if (root->left) {
        if (ExprTreeNode* n = nodeFindParent(node, root->left))
            return n;
    }
    // Check the right branch
    if (root->right) {
        if (ExprTreeNode* n = nodeFindParent(node, root->right))
            return n;
    }
    // Not found
    return nullptr;
}

// Helper function to check if a token is a number
static bool isNumber(string token) {
    // Obvious check
    if (token.size() == 0)
        return false;

    // A number can start with '-'
    int cur_char = 0;
    if (token[cur_char] == '-') {
        if (token.size() == 1)
            return false;
        cur_char++;
    }

    // All remaining characters must be numbers
    while (cur_char < token.size()) {
        if (token[cur_char] < '0' || token[cur_char] > '9')
            return false;
        cur_char++;
    }   

    // If we got here it's a number
    return true;
}

void Evaluator::parse(vector<string> code){
    // Root of the new tree
    ExprTreeNode* root = new ExprTreeNode();

    // There is an implicit opening bracket
    root->left = new ExprTreeNode();
    ExprTreeNode* cur_node = root->left;

    // Go through the tokens and build the tree
    for (string token: code) {
        // If the current token is a '(', add a new node as the left 
        // child of the current node, and descend to the left child.
        if (token == "(") {
            cur_node->left = new ExprTreeNode();
            cur_node = cur_node->left;
            continue;
        }

        // If the current token is a ')', go to the parent of the 
        // current node.
        if (token == ")") {
            cur_node = nodeFindParent(cur_node, root);
            continue;
        }

        // If the current token is an operator, set the root value of 
        // the current node to the operator represented by the current 
        // token. Add a new node as the right child of the current node 
        // and descend to the right child.
        if (token == ":=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "mod") {
            // Set the operator type
            if (token == ":=")
                cur_node->type = "ASS";
            else if (token == "+")
                cur_node->type = "ADD";
            else if (token == "-")
                cur_node->type = "SUB";
            else if (token == "*")
                cur_node->type = "MUL";
            else if (token == "/")
                cur_node->type = "DIV";
            else if (token == "mod")
                cur_node->type = "MOD";
            // Add the new right node
            cur_node->right = new ExprTreeNode();
            cur_node = cur_node->right;
            continue;
        }

        // If the current token is a number, set the root value of the 
        // current node to the number and return to the parent.
        if (isNumber(token)) {
            cur_node->type = "VAL";
            // Create the new UnlimitedRational
            UnlimitedInt* num = new UnlimitedInt(token);
            UnlimitedInt* den = new UnlimitedInt(1);
            cur_node->val = new UnlimitedRational(num, den);
            delete num;
            delete den;
            // Back to the parent
            cur_node = nodeFindParent(cur_node, root);
            continue;
        }

        // If we got this far the token is a variable. Treat this like a
        // number.
        cur_node->type = "VAR";
        cur_node->id = token;
        cur_node = nodeFindParent(cur_node, root);
    }

    // Add the tree to the list
    expr_trees.push_back(root);

    // Finished
    return;
}

UnlimitedRational* evaluate_expr_treenode(ExprTreeNode* node, SymbolTable* symtable) {
    if (!node) {
        return ZERO;
    }
    //if you reach a leaf node
    if(!node->left && !node->right){
        if(node->type=="VAL")
            return node->val;
        else
        {   //cout<<"var found!! "<<node->id<<endl;
            return symtable->search(node->id);
            
            }//if its a variable just look for it in symtable
    }
    // Recursively evaluate the left and right sides
    
    UnlimitedRational* lval=  evaluate_expr_treenode(node->left, symtable);
    

    
    UnlimitedRational* rval=  evaluate_expr_treenode(node->right, symtable);

    //cout<<"now evaluating the main node"<<endl;
    // Check if the node is a binary operator
    if (node->type == "MUL") {
        //cout<<"node type mul found"<<endl;
        return UnlimitedRational::mul(lval,rval);
    } else if (node->type == "DIV") {
        //cout<<"node type div found"<<endl;
        return UnlimitedRational::div(lval,rval);
    } else if (node->type == "ADD") {
        //cout<<"node type add found"<<endl;
        return UnlimitedRational::add(lval,rval);
    } 
        
    return UnlimitedRational::sub(lval,rval);
    

}

void Evaluator::eval(){
    if(expr_trees.empty())
        return ;
    ExprTreeNode* current=expr_trees.back();// last entry of the expression tree
    //populating the symbol table with the variable
    ExprTreeNode* var=current->left;
    //cout<<"success retrieving the variabel"<<endl;
    //symtable->insert(var->id,new UnlimitedRational());
    //cout<<"populated symbol table successfully"<<endl;
    ExprTreeNode* exp=current->right;
    //we dont have to extensively look for the case when the lhs is a val 
    //it is mentioned in the assignemt 
    //UnlimitedRational* var_val=symtable->search(var->id);//see if the current var exists in the table   
        //var->val=var_val;//set the value to whatever is found if even if its null
    UnlimitedRational*exp_val=evaluate_expr_treenode(exp,symtable);//set the value
    //thus updating the value of the the var in the symbol table
    //cout<<"exp val is: "<<exp_val->get_frac_str()<<endl;
    symtable->insert(var->id,exp_val);
    //cout<<"updated the value to "<<symtable->search(var->id)->get_frac_str()<<endl;
    //cout<<"updated var val success"<<endl;
    current->right=new ExprTreeNode("VAL",exp_val);
    expr_trees.pop_back();
    expr_trees.push_back(current);
    //cout<<"updated the tree success"<<endl;


    

};
