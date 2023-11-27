#ifndef LINEARPROBING_H
#define LINEARPROBING_H

#include "BaseClass.h"

// The table must be able to hold up to 100000 accounts
#define LINEAR_HASH_TABLE_SIZE 100000
#define HASH_NOT_FOUND -1

class LinearProbing : public BaseClass {
public:
    LinearProbing();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;

private:
    // Probing methods
    int findFree(std::string id);
    int findID(std::string id);

    // We need our own sort function
    void sort(std::vector<int> &v);
    void mergesort(std::vector<int>& v);
    void mergesortsub(std::vector<int>& v, std::vector<int>& a, std::vector <int>& b);


    // CRC used in the hash
    uint32_t crc32(std::string id);
};

#endif // LINEARPROBING_H
