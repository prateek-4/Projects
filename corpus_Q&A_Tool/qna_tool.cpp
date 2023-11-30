#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-86.txt"
//#define CSV_FILE "unigram_freq.csv"


class stopwordsnode{
    public:
    string w;

    stopwordsnode(){
        w="";
    }

    stopwordsnode(string word){
        w=word;
    }


};

class stopwordshashtable{
    public:
    stopwordsnode hashtable[144];

    stopwordshashtable(){
        
    }

    /*~stopwordshashtable() {
        for (int i = 0; i < 144; i++) {
            hashtable[i].clear();  // Clear each vector
        }
        delete hashtable;
    }*/

    int hashfunction(string id) {
        unsigned h = 0x81; // Initialize with 20 bits set to 1

        for (char c : id) {
            // XOR the current hash value with the character code of the character c
            h ^= static_cast<unsigned>(c);
            // Multiply the result by the constant 0x104C11DB7 (273 * 2^19 + 1)
            h *= 0x104C11DB7;
            // Apply a 20-bit mask to truncate to 20 bits
            h &= 0x81;//covers 98236 cases of account id
        }

        return static_cast<int>(h);
    }

    void insertintotable(string word){
    int index=hashfunction(word);
    stopwordsnode node=stopwordsnode(word);
    if (hashtable[index].w==""){
        hashtable[index]=node;
    }
    else{
        int originalIndex = index;
        do {
            index = (index+1) % 144;
            if (index == originalIndex) {
                // Table is full, handle accordingly
                // You may want to resize the table or throw an exception
                break;
            }
        } while(hashtable[index].w!="");
        hashtable[index]=node;
    }
}



    void insertstopwords(){
        string stop_words[] = {
    "the", "to", "of", "and", "i", "in", "a", "that", "is", "it", "you", "not", "be", "have", 
    "for", "from", "as", "if", "will", "was", "this", "he", "we", "with", "on", "but", "are", 
    "by", "they", "letter", "my", "your", "do", "has", "1", "me", "should", "all", "at", "there", 
    "or", "would", "his", "no", "had", "their", "can", "which", "who", "may", "them", "so", "one", 
    "what", "gandhi", "an", "been", "2", "only", "any", "our", "india", "am", "about", "were", "him", 
    "also", "such", "when", "even", "must", "time", "people", "government", "other", "work", "views","mahatma", 
    "now", "vol", "3", "works", "collected", "some", "know", "indian", "mr", "up", "these", "us", "out", 
    "shall", "more", "its", "therefore", "could", "congress", "those", "gujarati", "here", "cannot", "take", 
    "say", "after", "give", "n", "very", "like", "come", "being", "said", "her", "go", "then", "good", 
    "non", "see", "before", "how", "much", "than", "made", "does", "c", "think", "make", "without", "many", "4", "own", "did", "well"
        };
        for(int i=0;i<132;i++){
            this->insertintotable(stop_words[i]);
        }
    }


    bool isStopword(string word){
    int index = hashfunction(word);
    if (hashtable[index].w == word){
        return true;
    }
    else{
        while(hashtable[index].w != word){
            index = (index+1) % 144;
            if(index == hashfunction(word)){
                return false;
            }
        }
        return true;
    }
}
/*
    bool isStopword(string word){
        int index = hashfunction(word);
        if (hashtable[index].w=="word"){
            return true;
        }
        else{
            while(hashtable[index].w!="word"){
                index=(index+1)%144;
                if(index==hashfunction(word)){
                    return false;
                }
            }
            return true;
        }
    }
*/

};

string QNA_tool::modify_query(string input_query) {
    stopwordshashtable hashi;
    hashi.insertstopwords();
    string currword = "";
    string modified_query = "";

    for (char ch : input_query) {
        if (Doc_tool.bank.is_sep(ch)) {
            if (!currword.empty()) {
                bool flag = false;
                if (hashi.isStopword(currword)) {
                    flag = true;
                }
                if (!flag) {
                    modified_query += currword + ' ';
                }
                currword.clear();
            }
            // Append separators to the modified_query directly
            modified_query += ch;
        } else {
            if (isalpha(ch)) {
                currword += tolower(ch);
            } else {
                currword += ch;
            }
        }
    }

    // Handle the last word after the loop
    if (!currword.empty()) {
        bool flag = false;
        if (hashi.isStopword(currword)) {
            flag = true;
        }
        if (!flag) {
            modified_query += currword + ' ';
        }
    }

    return modified_query;
}

QNA_tool::QNA_tool(){
    //cout<<"now training dictionary ! "<<endl;
    //train_dictionary(Doc_tool.bank.store,CSV_FILE); this implement does not uses csv file for genearl count
    feed_dictionary(FILENAME);
    cout<<"training success ! "<<endl;
    // Implement your function here  
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    Doc_tool.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    //cout<<"now serching for question "<<question<< " and ranking the top : "<<k<<endl;
    vector<ParagOccur::Node*> temp= Doc_tool.para_search_results(modify_query(question));
    //cout<<"success searching for the word"<<endl;
    if(temp.size()==0)
        return NULL;
    paragraph* temp1= new paragraph(temp[0]->key);
    //cout<<"we are now adding this para to top k "<<temp1->BookCode<<"+"<<temp1->PageNum<<"+"<<temp1->ParagraphNum<<endl;
    Node* head= new Node(temp1->BookCode,temp1->PageNum,temp1->ParagraphNum,0,0);
    Node* headpointer=head;

    for(int i=1;i<temp.size()&&i<k;i++){// only return top k results
        temp1= new paragraph(temp[i]->key);
        //cout<<"we are now adding this para to top k "<<temp1->BookCode<<"+"<<temp1->PageNum<<"+"<<temp1->ParagraphNum<<endl;
        head->right = new Node(temp1->BookCode,temp1->PageNum,temp1->ParagraphNum,0,0);
        head=head->right;
    }
    head->right=NULL;
    return headpointer;

    
}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}

// void QNA_tool::train_dictionary(Trie & store,string filename){// we will use the .csv file to train the dictionary
//            //cout<<"now openinin the csv file : "<<endl;
//            ifstream file(filename);


//     // Skip the first line (header)
//             string header;
//             getline(file, header);

//             string line;
//             while (getline(file, line)) {
//                 istringstream iss(line);
//                 string word;
//                 long long count;

//                 if (getline(iss, word, ',') && (iss >> count)) {
//                     store.generateWord(word,count);
//                     //cout<<"generate word success for word : "<<word<<endl;
//                 } 
//             }

//     file.close();}

void QNA_tool::feed_dictionary(string filename){
    ifstream inputFile(FILENAME);
    // open the file in read mode
    //...........................

    string tuple;
    string sentence;


    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        std::vector<int> metadata;    
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata.push_back(num);
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata.push_back(num);
            }
        }

        // Now we have the string in sentence
        // And the other info in metadata
        // Add to the dictionary
        //cout<<"now inserting the sentence : "<<sentence<<endl;
        // Insert in the dictionary
        Doc_tool.bank.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
    }

    inputFile.close();

}
void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res){

    std::ifstream inputFile(FILENAME);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[4];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph) &&
            (metadata[3] == sentence_no)
        ){
            res = sentence;
            return;
        }

        res = "$I$N$V$A$L$I$D$";
        return;
    }

}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY){

    // first write the 3 paragraphs into different files
    Node* traverse = root;
    int num_paragraph = 0;
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        ofstream outfile(p_file);
        string paragraph;
        get_paragraph(traverse->book_code, traverse->page, traverse->paragraph, traverse->sentence_no, paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "what is purpose of life";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // python3 <filename> API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += "paragraph_0.txt";
    command += " ";
    command += "paragraph_1.txt";
    command += " ";
    command += "paragraph_2.txt";
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}