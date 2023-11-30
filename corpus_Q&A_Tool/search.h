// Do NOT add any other includes
#pragma once
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
#include "dict.h"
#include <sstream>
using namespace std;

struct paragraph
{
    int BookCode;
    int PageNum;
    int ParagraphNum;
    paragraph(string date){
    istringstream dateStream(date);
    string token;
    getline(dateStream, token, '+'); // Extract book_code
    BookCode = stoi(token);

    getline(dateStream, token, '+'); // Extract page_num
    PageNum = stoi(token);

    getline(dateStream, token); // Extract paragraph_num
    ParagraphNum = stoi(token);
    }

    ~paragraph(){};
};

class SearchEngine {
private:
    // You can add attributes/helper functions here

public: 
    Dict bank;
    void sort(vector<ParagOccur::Node*> &v);
    void mergesort(vector<ParagOccur::Node*> & v);
    void mergesortsub(vector<ParagOccur::Node*>&v,vector<ParagOccur::Node*>&a,vector<ParagOccur::Node*>&b);
    vector<EndWord*> word_results(string query);
    vector<ParagOccur::Node*> para_search_results(string pattern);
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};