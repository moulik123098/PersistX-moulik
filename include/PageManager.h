#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <vector>
#include <string>
#include <optional>

#include "Page.h"
#include "DiskManager.h"
#include "BufferPool.h"

class QueryEngine;

class PageManager {
private:
    std::vector<int> pageIds;
    int nextPageId;
    DiskManager disk;
    BufferPool buffer;
    QueryEngine* qe;

    Page& allocatePage();

public:
    PageManager();

    void insert(const std::string& key, const std::string& value);
    std::optional<std::string> search(const std::string& key);
    bool remove(const std::string& key);

    void display();
    Page* getPage(int pageId);

    size_t pageCount()  { return pageIds.size(); }
    size_t recordCount();

    void flushAll()           { buffer.flushAll(); }
    void displayBufferStats() { buffer.displayStats(); }

    BufferPool& getBuffer()        { return buffer; }
    std::vector<int>& getPageIds() { return pageIds; }

    void setQueryEngine(QueryEngine* engine) { qe = engine; }
};

#endif