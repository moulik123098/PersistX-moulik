#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <optional>

#include "Record.h"

class Database
{
private:
    std::vector<Record> records;

public:
    void insertRecord(std::string &key, std::string &value);
    std::optional<std::string> searchRecord(std::string &key);
    bool removeRecord(std::string &key);
    void displayRecords();
    size_t size();
};

#endif