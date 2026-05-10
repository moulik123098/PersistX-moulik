#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <vector>
#include <string>
#include <optional>

#include "Page.h"
#include "DiskManager.h"
#include "BufferPool.h"

class PageManager
{
private:
    std::vector<int> pageIds;
    int nextPageId;
    DiskManager disk;
    BufferPool buffer;

    Page &allocatePage();

public:
    PageManager();

    void insert(std::string &key, std::string &value);
    std::optional<std::string> search(std::string &key);
    bool remove(std::string &key);

    void display();
    Page *getPage(int pageId);

    size_t pageCount()
    {
        return pageIds.size();
    }
    size_t recordCount();

    void flushAll(){
        buffer.flushAll();
    }
    void displayBufferStats(){
        buffer.displayStats();
    }

    BufferPool& getBuffer(){
        return buffer;
    }
    std::vector<int>& getPageIds(){
        return pageIds;
    }
};

#endif