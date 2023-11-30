/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"
#include <iostream>
using namespace std;

//Write your code below this line

// #define print_vec(vec) \
//     do { \
//         cout << "["; \
//         for (const auto& element : vec) { \
//             cout << element << " "; \
//         } \
//         cout << "]" << endl; \
//     } while (0);

//just for debugging 

//...........................
// helper functions 

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
    return NULL;
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
Parser::Parser(){
    symtable= new SymbolTable();
}


void Parser::parse(vector<string> expression){
    // Root of the new tree
    //print_vec(expression)
    ExprTreeNode* root = new ExprTreeNode();

    // There is an implicit opening bracket
    root->left = new ExprTreeNode();
    ExprTreeNode* cur_node = root->left;

    // Go through the tokens and build the tree
    for (string token: expression) {
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
            if (token == ":=") {
                // Set the operator type
                cur_node->type = "ASS";
                // Add the new right node
                cur_node->right = new ExprTreeNode();
                cur_node = cur_node->right;
                continue;}
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
            cur_node->num = stoi(token);
            // Back to the parent
            cur_node = nodeFindParent(cur_node, root);
            continue;
        }

        // If we got this far the token is a variable. Treat this like a
        // number. or ret node
        else if(token=="ret"){//for  ret and del nodes
            cur_node->type="RET";
            //cout<<"ret type token is found"<<endl;
            cur_node = nodeFindParent(cur_node, root);
        }
        else if(token=="del"){//for  ret and del nodes
            cur_node->type="DEL";
            cur_node = nodeFindParent(cur_node, root);
        }
        else{cur_node->type = "VAR";
        cur_node->id = token;
        if(expression[0]!="del")
            symtable->insert(token);
        // if(expression[0]=="del")
        //     symtable->remove(token);
        //cout<<"Parsed var id is : "<<cur_node->id<<endl;
        cur_node = nodeFindParent(cur_node, root);}
    }

    // Add the tree to the list
    expr_trees.push_back(root);

}

Parser::~Parser(){
    delete symtable;
    for(auto i : expr_trees)
        delete i;
}