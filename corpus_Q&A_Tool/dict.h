// Do NOT add any other includes
#pragma once
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
// class to store the occurence of a pragraph
// ...............................................

class ParagOccur {
public:
    class Node {
    public:
        string key;
        
        long double count;
        
        int height;
        
        Node* left;
        
        Node* right;

        Node(const string& k , double add) : key(k), count(add), height(1), left(NULL), right(NULL) {}
    };
    
    long double num_nodes=0;
    
    long double dense_count=0;

    Node* root;
    
    int height(Node* node);
    
    int balanceFactor(Node* node);
    
    void updateHeight(Node* node);
    
    Node* rotateRight(Node* y);
    
    Node* rotateLeft(Node* x);
    
    Node* insert_help(Node* node, const string& key,long double additive);
    
    vector<ParagOccur::Node*> Traversal(Node* node);
    
    void debug_traversal(Node* node);
    
    ParagOccur();
    
    void insert(const string& key, long double add);
    
    Node* search_help(Node * root,string key);

    Node* search_key(string key);
    
    vector<ParagOccur::Node*> inorderTraversal();
};

// tries and trie node
//..................................................

// end of word class(to store the words and their occurences)
// .....................

class EndWord {    
public:
    string word;
    
    //long long count=0;
    
    //long long general_count=0;
    
    long double score=0;
    
    ParagOccur* occur;
    
    EndWord(string word);
    
    void updateScore(int del);
    
    // Comparison operators
    bool operator==(const EndWord& other) const;

    bool operator!=(const EndWord& other) const ;

    bool operator<(const EndWord& other) const ;

    bool operator>(const EndWord& other) const ;

    bool operator<=(const EndWord& other) const ;

    bool operator>=(const EndWord& other) const ;
    // Add other functions as needed

};
// class for storing number of words in a paragraph
// Fuck hashing
// ..................................................

class WordCountVector{
    public:
        long double num_para;
        
        vector<vector<vector<ParagOccur>>> wordCounts;
        
        WordCountVector(int bookCount) {
            num_para=0;
        // Initialize the 3D vector with zeros
            wordCounts.resize(bookCount);}
        
        void setWordCount(int book, int page, int paragraph, string word);
        
        long double getWordCount(int book, int page, int paragraph) const;
        ~WordCountVector() {
        // No need to manually deallocate memory for vectors
        // If you had raw pointers or other dynamically allocated memory, you'd deallocate it here
    }
};
// trienode
// ...........................................

class TrieNode {
public:
    char val;
    
    EndWord* endWord;
    
    vector<TrieNode*> charArr;

    TrieNode();
    ~TrieNode();
    TrieNode(char c);
    // Add other functions as needed

};

/// trie
/// ..............................................


class Trie {    
public:
    TrieNode * root;
    WordCountVector word_count;
    
    Trie();  // Constructor
    void addWord(string& text, int page, int paragraph_no, int book_code, int sentence_no);
    
    //void generateWord(string& text, long long gen_count);
    
    EndWord* search_word(string word);

    ~Trie();  // Destructor

};

// main class
// ...........................


class Dict {
private:
    // You can add attributes/helper functions here
public: 
    Trie store;// need to ask on piazza if we can add any function on public of dict
    bool is_sep(char c);
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};