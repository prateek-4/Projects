// ---------------------------------------------------------------------
// Test UnlimitedInt
// ---------------------------------------------------------------------

#include <iostream>
#include <stdexcept>
#include "minheap.h"

using namespace std;
void printHeap(HeapNode* root) {
    if (root) {
        printHeap(root->left);
        cout << root->val << " ";
        printHeap(root->right);
    }
}
int main(int argc, char* argv[]) {
    string s1, op;
    MinHeap heap;
    
    do {
        cout << "Enter the operation i1 ";
        if (s1 == "q") break;
        cin >> op;
        cin >> s1;

        if (op == "push") {
            cout<<"now pushing : "<<stoi(s1)<<endl;
            heap.push_heap(stoi(s1));
            cout<<"success pushing : "<<s1<<endl;
        }
        else if (op == "get_min") {
            cout<<"min_element is: "<<heap.get_min()<<endl;
        }

        else if (op == "pop") {
            heap.pop();
            cout<<"success popping out"<<endl;
        }
        else {
            cout << "Unknown operator: " << op << endl;
        }
        
    } while (s1 != "q");

    return 0;
}