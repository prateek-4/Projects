// Do NOT add any other includes
#include "dict.h"
// Note:: for preprocessing before processing any of the corpus we should first use the csv data to update the general occurences of each word
// we can use different tyes of add funtion for that (one for updating the general count and other for just updating the count)

// Note: we should preprocess the corpus twice as the count of each word in the corpus cant be known without preproccesssing the document

// plan so far.............................................................
// step 1 : first somehow load the csv file to preprocess the general count for each word before preprocessing the corpus
// step 2 : next preprocess the corpus and update the score of each word
// step 3 : somehow store the paragraphs in sorted manner according to their score

// last step: after completing this program the  exe file from this program will be used for the query part of the qna tool

/// helper functions 
// ....................................

bool Dict::is_sep(char c){
    string st =".,-:!\"'()?[];@ ";
    size_t found=st.find(c);
    return (found==string::npos)?false:true;
}
/// 
//
EndWord::EndWord(string word) {
        this->occur=new ParagOccur();
        this->word=word;
        //this->count+=1;
        //this->score+=2/(general_count+1);
}

// void EndWord::updateScore(int del) {
//     count += del;
//     score += del / (1 + general_count);
//     // Implement other functions as needed
// }

// Comparison operators
    bool EndWord::operator==(const EndWord& other) const {
        return word == other.word;
    }

    bool EndWord::operator!=(const EndWord& other) const {
        return word != other.word;
    }

    bool EndWord::operator<(const EndWord& other) const {
        return word < other.word;
    }

    bool EndWord::operator>(const EndWord& other) const {
        return word > other.word;
    }

    bool EndWord::operator<=(const EndWord& other) const {
        return word <= other.word;
    }

    bool EndWord::operator>=(const EndWord& other) const {
        return word >= other.word;
    }
//............................................
void WordCountVector::setWordCount(int book, int page, int paragraph, string word) {
        // Resize vectors if needed to accommodate new page and paragraph numbers
        if (wordCounts.size() <= book) {
            wordCounts.resize(book + 1);
        }
        if (wordCounts[book].size() <= page) {

            wordCounts[book].resize(page + 1);
        }
        if (wordCounts[book][page].size() <= paragraph) {
            wordCounts[book][page].resize(paragraph + 1);
            //cout<<"new para added is "<<to_string(book)+" , "+to_string(page)+" , "+ to_string(paragraph)<<endl;
            num_para++;// incremment each time new paragraph is added
        }
        
        // Update the word count for a given book, page, and paragraph
        wordCounts[book][page][paragraph].insert(word,1);// this stores the words and the count of each word for later cosine similarity calculation
        //cout<<"success inserting into the paragarph "<<to_string(book)+" , "+to_string(page)+" , "+to_string(paragraph)<<endl;
        //cout<<"success inserting the word "<<word<<endl;
    }
long double WordCountVector::getWordCount(int book, int page, int paragraph) const {
        // Check if the requested indices are within bounds
        if (book < wordCounts.size() && page < wordCounts[book].size() && paragraph < wordCounts[book][page].size()) {
            // Get the word count for a given book, page, and paragraph
            return wordCounts[book][page][paragraph].dense_count;
        } else {
            // Indices out of bounds, return 0 or handle accordingly
            return 0;
        }
    }
// ........................................

TrieNode::TrieNode() : val('\0'), endWord(NULL), charArr(96, NULL) {}

TrieNode::TrieNode(char c) : val(c), endWord(NULL), charArr(96, NULL) {}
//...........................................

TrieNode::~TrieNode() {
    // Delete dynamically allocated TrieNode objects in charArr
    for (TrieNode* child : charArr) {
        delete child;
    }

    // Delete the endWord string if it's not NULL
    delete endWord;
}


Trie::Trie() : root(new TrieNode('<')), word_count(100) {}// asssuming no more than 100 books

Trie::~Trie() {
    // Call the destructor for the root node, which will recursively delete all nodes
    if(root){
        delete root;
    }
}

void Trie::addWord(string& text, int page, int paragraph_no, int book_code, int sentence_no) {
            TrieNode* curr = this->root;// to keep a track of the current node we are inserting
            
            
            for(char c: text){
                if(curr->charArr[c-'!']==NULL)// if the char is absent just create a new node to it
                    curr->charArr[c-'!']=new TrieNode(c);
                curr=curr->charArr[c-'!'];
            }
            
            // there can be words not in csv but in corpse
            if(curr->endWord==NULL)
                {curr->endWord=new EndWord(text);
                //curr->endWord->general_count=0;
                curr->endWord->score=1;
                }
            // ............................................................................

            //curr->endWord->updateScore(1);// this updates the count as well the score of each word
            curr->endWord->occur->insert(to_string(book_code)+"+"+to_string(page)+"+"+to_string(paragraph_no),1);
            word_count.setWordCount(book_code,page,paragraph_no,text);// updating the paragraph word count as we move along
}

EndWord* Trie::search_word(string word){
    // as discussed on piazza the word will always exist
    TrieNode *curr=root;
    for(char ch: word){
        if(!curr)
            return NULL;
        //cout<<"current char is "<<ch<<endl;
        curr=curr->charArr[ch-'!'];
    }
    return curr->endWord;
}

// Implement other functions and destructor as needed


// where we shall store all the occurence ofthe word
// ...........................................................


ParagOccur::Node* ParagOccur::insert_help(Node* node, const string& key, long double additive) {// IF WE NEED THE CLASS AGAIN TO STORE THE PARAGRAPHS AT LAST
    if (node == NULL){
            num_nodes++;// also keep a track of number of paragraph for tf idf calculatio
            dense_count++;// this keeps the track of number of words in the paragraph
            return new Node(key,additive);}

        if (key < node->key)
            node->left = insert_help(node->left, key, additive);
        else if (key > node->key)
            node->right = insert_help(node->right, key, additive);
        else
            {   dense_count++;
                node->count+=additive;} // Increment count for repeated insertion

        updateHeight(node);

        int balance = balanceFactor(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
}

vector<ParagOccur::Node*> ParagOccur::Traversal(Node* node) {
    vector<ParagOccur::Node*> res;
     if (node != NULL) {
        vector<ParagOccur::Node*> l=Traversal(node->left);// recuresively search left
        res.insert(res.end(),l.begin(),l.end());// and add the array to the result from the beginning
        res.push_back(node);// now add the current node
        vector<ParagOccur::Node*> r=Traversal(node->right);// recursively search the right
        res.insert(res.begin(),r.begin(),r.end());// add this vector to the end of the result
        }
    return res;// return the result
}

int ParagOccur::height(Node* node) {
    if (node == NULL)
            return 0;
        return node->height;
}

int ParagOccur::balanceFactor(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

void ParagOccur::updateHeight(Node* node) {
    if (node != NULL)
        node->height = 1 + max(height(node->left), height(node->right));
}

ParagOccur::Node* ParagOccur::rotateRight(Node* y) {
        Node* x = y->left;
        Node* T = x->right;

        x->right = y;
        y->left = T;

        updateHeight(y);
        updateHeight(x);

        return x;
}

ParagOccur::Node* ParagOccur::rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T = y->left;

        y->left = x;
        x->right = T;

        updateHeight(x);
        updateHeight(y);

        return y;
}

ParagOccur::ParagOccur() : root(NULL),num_nodes(0) {}

void ParagOccur::insert(const string& key, long double add) {
    root = insert_help(root, key,add);
    //cout<<"success updating the registrar : "<<key<<endl;
}

ParagOccur::Node* ParagOccur::search_help(Node * root, string key){
    if(root==NULL)
        {   //cout<<"root is null ! "<<endl;
            return NULL;}
    if(key>root->key)
       {    //cout<<"root is greater than key"<<endl; 
        return search_help(root->right,key);}
    if(key<root->key){
        //cout<<"root is less than key"<<endl;
        return search_help(root->left,key);}
    return root;
}

ParagOccur::Node* ParagOccur::search_key(string key){
    //cout<<"now searching for the word "<<key<<endl;
    return search_help(root,key);
}

void ParagOccur::debug_traversal(Node* node){

};
vector<ParagOccur::Node*> ParagOccur::inorderTraversal() {// weshall use this later to calculate the scores of the paragraphs
        return Traversal(root);
        }
// main class
// ......................................

Dict::Dict(){

    // Implement your function here    
}

Dict::~Dict(){
    // Implement your function here    
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string currword="";
    for(char ch: sentence){
        if(!is_sep(ch))
            { 
                if(isalpha(ch))
                    currword+=tolower(ch);
                else
                    currword+=ch;
            
            }// we should not distinguish the upper and lower case letters
        else{
            if(!currword.empty()){
                //cout<<"now inserting the word :"<<currword<<endl;
                store.addWord(currword,page,paragraph,book_code,sentence_no);
                currword.clear();
            }
        }
    }
    if(!currword.empty())
       {store.addWord(currword,page,paragraph,book_code,sentence_no);
       //cout<<"now inserting the word: "<<currword<<endl;
       }
       
}

int Dict::get_word_count(string word){
    // Implement your function here  
    return -1;
}

void Dict::dump_dictionary(string filename){
    // Implement your function here  
    return;
}

// to be handled later
// /..........................


   