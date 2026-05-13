#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <utility>

#include "BufferPool.h"
#include "Record.h"

struct IndexEntry {
    int pageId;
    int slotIndex;
};

class QueryEngine {
private:
    BufferPool& buffer;
    std::vector<int>& pageIds;
    std::map<std::string, IndexEntry> Index;

    void BuildIndex();

public:
    QueryEngine(BufferPool& buffer, std::vector<int>& pageIds);

    void IndexInsert(const std::string& key, int pageId, int slotIndex);
    void IndexRemove(const std::string& key);
    void rebuild();

    std::optional<std::string> get(const std::string& key);
    std::vector<Record> listAll();
    std::vector<Record> filterByPrefix(const std::string& prefix);
    std::vector<Record> rangeQuery(const std::string& startKey, const std::string& endKey);

    void displayAll();
    void displayPrefix(const std::string& prefix);
    void displayRange(const std::string& startKey, const std::string& endKey);
    void displayIndex();
};

#endif