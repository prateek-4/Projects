// Do NOT add any other includes
#include "search.h"
#include <cmath>


// helper functions
// ..................................................

//......................................................

long double calculateNorm(const vector<long double>& vector) {
    long double sumOfSquares = 0.0;

    // Calculate the sum of squares of each element
    for (long double element : vector) {
        sumOfSquares += element * element;
    }

    // Take the square root to get the Euclidean norm
    long double norm = sqrt(sumOfSquares);

    return norm;
}
//

void SearchEngine::sort(vector<ParagOccur::Node*> &v) {

    mergesort(v);
}

// Implementation of merge sort
void SearchEngine::mergesort(vector<ParagOccur::Node*>& v) {
    if (v.size() <= 1) {
            return;
        }

        vector<ParagOccur::Node*> right(v.begin() + v.size() / 2, v.end());
        vector<ParagOccur::Node*> left(v.begin(), v.begin() + v.size() / 2);

        mergesort(left);
        mergesort(right);

        mergesortsub(v, left, right);
}

void SearchEngine::mergesortsub(vector<ParagOccur::Node*>& v, vector<ParagOccur::Node*>& a, vector <ParagOccur::Node*>& b) {
    v.clear();

        auto itLeft = a.begin();
        auto itRight = b.begin();

        while (itLeft != a.end() && itRight != b.end()) {
            if ((*itLeft)->count > (*itRight)->count) {
                v.push_back(*itLeft);
                ++itLeft;
            } else {
                v.push_back(*itRight);
                ++itRight;
            }
        }

        v.insert(v.end(), itLeft, a.end());
        v.insert(v.end(), itRight, b.end());
}

//...........................................................
SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    bank.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
}

vector<EndWord*> SearchEngine::word_results(string query){

    vector<EndWord*> result;
    string currword="";
    for(char ch: query){
        if(!bank.is_sep(ch))
            { 
                if(isalpha(ch))
                    currword+=tolower(ch);
                else
                    currword+=ch;
            
            }// we should not distinguish the upper and lower case letters
        else{
            if(!currword.empty()){
                //cout<<"now searching for the word "<<currword<<endl;
                EndWord* curr= bank.store.search_word(currword);
                if(curr)// in tf idf we dont train our dictionary so there can be empty words
                    result.push_back(curr);
                currword.clear();
            }
        }
    }
       if(!currword.empty()){
                result.push_back(bank.store.search_word(currword));
                currword.clear();
            } 
        return result;
}

vector<ParagOccur::Node*> SearchEngine::para_search_results(string pattern){
    // this function is the heart of the algorithm and most challenging to write .
    // it will contain all the calculations required to rankt the paragraphs 
    // it involves norm of a vector
    //............................................................

    vector<EndWord*> temp = word_results(pattern);

        //cout<<"step # error"<<endl;
    ParagOccur tree;
    
    vector<ParagOccur::Node*> temp2;
    
    for(EndWord* i: temp){// assuming the query isnt too long we can traverse it back
    
        temp2=i->occur->inorderTraversal();
        for(ParagOccur::Node* j: temp2){
            //cout<<"for the word "<<i->word<<" whose score is : "<<i->general_count<<" the count in paragraph "<<j->key<<" is "<<j->count<<endl;;
            tree.insert(j->key,1);    
        }
    
    }
    // just calculted how many documents are relevant to the search
    // this makes it simpler as we dont have to search among the other useless paragraphs


    // query vector
    // ......................................

    vector<long double> query_vec;// initialise the query vector
    for(EndWord* i:temp){
        double xi= log(static_cast<long double>(bank.store.word_count.num_para)/(i->occur->num_nodes+1));//idf value
        // double  tf=1/(static_cast<double>(temp.size())); // tf value  ***ASSUMING OCCURENCE OF THE WORD IN QUERY ONLY ONCE
                    query_vec.push_back(xi);
    }
    long double norm_query=calculateNorm(query_vec); 
    temp2.clear();// just thougth of using the same vector nothing special thought
    
    temp2=tree.inorderTraversal(); // extracting the relevant paragraphs
    //cout<<"now calculating dot product"<<endl;
    
    for(ParagOccur::Node* j : temp2){ // calculating the dot product
        long double dot=0;
        paragraph curr(j->key);// for extracting various parameters of the paragraph
        //cout<<"considering para : "<<to_string(curr.BookCode)+" , "+to_string(curr.PageNum)+" , "+to_string(curr.ParagraphNum)<<endl;
        for(EndWord* k : temp){
            //cout<<"number of paragraphs is "<<static_cast<double>(bank.store.word_count.num_para)<<endl;
            //cout<<"calculations being carried out for the word "<<k->word<<endl;
            double xi= log(static_cast<long double>(bank.store.word_count.num_para)/(k->occur->num_nodes+1));//idf value
            //cout<<"success calculating idf value "<<endl;

            long double num =0;// num of times the word occurs in the paragraph
            ParagOccur::Node* curr_word =(bank.store.word_count.wordCounts[curr.BookCode][curr.PageNum][curr.ParagraphNum].search_key(k->word));
            if(curr_word)
                num=curr_word->count;
            //cout<<"success calculating num"<<endl;
            long double den = (bank.store.word_count.getWordCount(curr.BookCode,curr.PageNum,curr.ParagraphNum));// num of words in the paragraph
            dot+=pow(xi,2)*(log(num/den)+1);
        }
        
        j->count=dot; // update the dot product
    }

    // NOW the final part of the calculation :: WARNING there might be unneccesaary calculations but i could not reduce the calculations further
    // the above block of processing the pargrapsh and calculating scores must be optimised later
    // .....................................................................................


    for(ParagOccur::Node* j : temp2){
         
        paragraph curr(j->key);// for extracting various parameters of the paragraph
        
        long double den = (bank.store.word_count.getWordCount(curr.BookCode,curr.PageNum,curr.ParagraphNum));// num of words in the paragraph
        
        vector<ParagOccur::Node*> words= bank.store.word_count.wordCounts[curr.BookCode][curr.PageNum][curr.ParagraphNum].inorderTraversal();// extracted the words

        vector<long double> para_norm;

        for(ParagOccur::Node* alp : words){
            
            long double num = (bank.store.word_count.wordCounts[curr.BookCode][curr.PageNum][curr.ParagraphNum].search_key(alp->key)->count);// num of times the word occurs in the paragraph
            
            EndWord* k = bank.store.search_word(alp->key);// the current word to be considered for calculating the xi values

            long double xi= static_cast<double>(bank.store.word_count.num_para)/k->occur->num_nodes;//idf value

            long double res = (num/den)*xi;// vector terms

            para_norm.push_back(res);

        }

        long double norm = calculateNorm(para_norm);

        j->count=j->count/(norm*norm_query);

    }
    
    // we have obtained the paragraphs now we have to sort them
    // ........................................................
    sort(temp2);
    // for(ParagOccur::Node* i: temp2)
    //     cout<<i->key<<" withe the count "<<i->count<<" , ";
    // cout<<endl;
    return temp2;
    // Now making of linked list will be done in qnatool.cpp since there only we can use sstream linrary
    // ;)
    //  we are gonna need a string seperator based on the delimeter "+"

}

Node* SearchEngine::search(string pattern, int& n_matches){
    return NULL;
    
    
}