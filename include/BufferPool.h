#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <optional>
 
#include "Page.h"
#include "DiskManager.h"

static constexpr size_t BUFFER_POOL_CAPACITY = 10;

struct HeapEntry{
    int timestamp;
    int pageId;

    bool operator>(const HeapEntry& other) const {
        return timestamp > other.timestamp;
    }

};

class BufferPool{
private:

    std::unordered_map<int,Page> cache;
    std::unordered_map<int,bool> dirty;
    std::unordered_map<int,int> timestamps;
    std::priority_queue<HeapEntry, std::vector<HeapEntry>, std::greater<HeapEntry>> minHeap;

    int currentTime;
    size_t capacity;

    DiskManager &disk;

    void touch(int pageId);
    void evict();

public:

    BufferPool(DiskManager& disk, size_t capacity = BUFFER_POOL_CAPACITY);

    Page& getPage(int pageId);
    void markDirty(int pageId);
    bool inCache(int pageId);
    void flushAll();

    size_t cacheSize() { 
        return cache.size(); 
    }
    size_t cacheCapacity() {
        return capacity; 
    }
    void displayStats();

};

#endif