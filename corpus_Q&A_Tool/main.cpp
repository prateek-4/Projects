#include <iostream>
#include <stdexcept>
#include "qna_tool.h"
#include "Node.h"
using namespace std;
void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res);
int main(int argc, char* argv[]) {
    string op, input;
    int n;
    QNA_tool quizer;
    
    do {
        cout << "op(?,n) to find, q to quit): ";
        cin >> op;

        if (op == "?") {
            cout << "Enter the query to search: "<<endl;
            cin.ignore(); // Clear the buffer
            getline(cin, input);

            cout<<" how many results to find: "<<endl;
            cin>>n;
            cout << "Now searching for the query: " << input << endl;
            Node* curr;
            curr = quizer.get_top_k_para(input,n);
            while (curr) {
                string para;
                //get_paragraph(curr->book_code,curr->page,curr->paragraph,curr->sentence_no,para);
                cout << "The given pattern is found in: " <<"book no: "<<curr->book_code<< " page_no: "<<curr->page<<" paragraph_no: "<<curr->paragraph<<endl;
                curr = curr->right;
            }
            }
            else if (op != "q") {
                    cout << "Unknown operation: " << op << endl;
                }
                
            } while (op != "q");

            return 0;
}
