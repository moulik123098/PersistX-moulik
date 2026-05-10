#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <string>
#include <vector>
#include <optional>
 
#include "BufferPool.h"
#include "Record.h"

class QueryEngine{
private:

    BufferPool &buffer;
    std::vector<int> &pageIds;

public:

    QueryEngine(BufferPool& buffer, std::vector<int>& pageIds);

    std::optional<std::string> get(std::string& key);
    std::vector<Record> listAll();
    std::vector<Record> filterByPrefix(std::string& prefix);
    void displayAll();
    void displayPrefix(std::string& prefix);

};

#endif