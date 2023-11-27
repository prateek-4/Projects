
#include "Chaining.h"
using namespace std;

Chaining::Chaining() {
    // Create the hash table with HASH_TABLE_SIZE empty vectors
    vector<struct Account> empty;
    bankStorage2d.resize(HASH_TABLE_SIZE, empty);
}

//sort function
void Chaining::sort(vector<int> &v) {
    // We tested both sorts
    // quicksort(v, 0, v.size()-1);
    mergesort(v);
}

// Implementation of merge sort
void Chaining::mergesort(vector<int>& v) {
    // If the vector has only a single element we are done
    if(v.size()==1)
        return;
    // Split the vector into two halves
    vector<int>::iterator i,j,mid;
    i=v.begin();
    j=v.end();
    mid=i+(j-i)/2;
    vector<int> left(i,mid);
    vector<int> right(mid,j);
    // Sort the two halves
    mergesort(left);
    mergesort(right);
    // And merge the sorted halves
    mergesortsub(v, left, right);
}

void Chaining::mergesortsub(vector<int>& v, vector<int>& a, vector <int>& b) {
    vector<int> ::iterator i,j;
    i=a.begin();
    j=b.begin();

    // Clear the destination vector
    v.clear();
    // Merge until we hit the end of one of the halves
    // Note we are merging in descending order
    while(i < a.end() && j < b.end()) {
        if(*i < *j) {
            v.push_back(*j);
            j++;
        }
        else {
            v.push_back(*i);
            i++;
        }
    }

    // Append from whichever half still has data
    while (i < a.end()) {
        v.push_back(*i);
        i++;
    }
    while(j < b.end()) {
        v.push_back(*j);
        j++;
    }
}


void Chaining::createAccount(std::string id, int count) {
    // Get the hash
    int h = hash(id);
    // Append the account to the list at our hash
    bankStorage2d[h].push_back({id, count});
}

std::vector<int> Chaining::getTopK(int k) {
    // Copy all the balances to a temporary vector so we can sort them
    vector<int> balances;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        for (int j = 0; j < bankStorage2d[i].size(); j++)
            balances.push_back(bankStorage2d[i][j].balance);
    
    // Sort the array in descending order
    sort(balances);

    // And put the top k values in a new vector to be returned
    vector<int> topk;
    for (int i = 0; i < k && i < balances.size(); i++)
        topk.push_back(balances[i]);

    // Return the balances
    return topk;
}

int Chaining::getBalance(std::string id) {
    // Get the hash
    int h = hash(id);
    // If the hash isn't found return an error
    if (bankStorage2d[h].size() == 0)
        return HASH_NOT_FOUND;
    // Iterate through the ids in this hash. This is slow but easy.
    for (int i = 0; i < bankStorage2d[h].size(); i++)
        if (bankStorage2d[h][i].id == id)
            return bankStorage2d[h][i].balance;

    // We should never reach this point
    return HASH_NOT_FOUND;
}

void Chaining::addTransaction(std::string id, int count) {
    // Get the hash
    int h = hash(id);

    // Iterate through the ids in this hash. This is slow but easy.
    int hash_id;
    for (hash_id = 0; hash_id < bankStorage2d[h].size(); hash_id++)
        if (bankStorage2d[h][hash_id].id == id)
            break;

    // If the hash isn't found add the account
    if (hash_id == bankStorage2d[h].size()) {
        createAccount(id, count);
    }
    // Else update the balance
    else {
        bankStorage2d[h][hash_id].balance += count;
    }
}

bool Chaining::doesExist(std::string id) {
    // Get the hash
    int h = hash(id);
    // Iterate through the ids in this hash. This is slow but easy.
    for (int i = 0; i < bankStorage2d[h].size(); i++)
        if (bankStorage2d[h][i].id == id)
            return true;

    // If we got here the id wasn't found
    return false;
}

bool Chaining::deleteAccount(std::string id) {
    // Get the hash
    int h = hash(id);
    // If the hash isn't found return an error
    if (bankStorage2d[h].size() == 0)
        return false;

    // Iterate through the ids in this hash. This is slow but easy.
    for (int i = 0; i < bankStorage2d[h].size(); i++) {
        if (bankStorage2d[h][i].id == id) {
            bankStorage2d[h].erase(bankStorage2d[h].begin() + i);
            return true;
        }
    }

    // If we got here the id wasn't found
    return false;
}

int Chaining::databaseSize() {
    int num_accounts = 0;
    for (int i = 0; i < bankStorage2d.size(); i++)
        num_accounts += bankStorage2d[i].size();
    return num_accounts;
}

int Chaining::hash(string id) {
    // Return a CRC-CCITT checksum
    return ((int) crc16(id)) & 0xFFFF;
}

// CRC-16-CCITT
// x^16 + x^12 + x^5 + 1
// See https://en.wikipedia.org/wiki/Cyclic_redundancy_check
uint16_t Chaining::crc16(string id){
    uint8_t x;
    uint16_t crc = 0xFFFF;

    for (int i = 0; i < id.size(); i++){
        x = crc >> 8 ^ id[i];
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
    }
    return crc;
}