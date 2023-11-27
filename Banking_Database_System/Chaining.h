#ifndef CHAINING_H
#define CHAINING_H

#include "BaseClass.h"

#define HASH_TABLE_SIZE 65536
#define HASH_NOT_FOUND -1

class Chaining : public BaseClass {
public:
    Chaining();
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
    uint16_t crc16(std::string id);
};

#endif // CHAINING_H
