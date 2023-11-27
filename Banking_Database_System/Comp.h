#ifndef COMP_H
#define COMP_H

#include "BaseClass.h"

// We use a CRC-16 hash so the table size is 2^16
#define COMP_HASH_TABLE_SIZE 32768
#define HASH_NOT_FOUND -1

class Comp : public BaseClass {
public:
    Comp();
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

#endif // COMP_H
