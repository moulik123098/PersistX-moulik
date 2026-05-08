#include "BufferPool.h"

#include <iostream>

BufferPool::BufferPool(DiskManager &disk, size_t capacity)
    : disk(disk), capacity(capacity), currentTime(0) {}

void BufferPool::touch(int pageId)
{
    currentTime++;
    timestamps[pageId] = currentTime;
    minHeap.push({currentTime, pageId});
}

void BufferPool::evict() {
    while (!minHeap.empty()) {
        HeapEntry entry = minHeap.top();
        minHeap.pop();
 
        
        if (timestamps.count(entry.pageId) &&
            timestamps[entry.pageId] != entry.timestamp) {
            continue; 
        }
 
        
        int victimId = entry.pageId;
 
        if (dirty.count(victimId) && dirty[victimId]) {
            
            std::cout << "[BUFFER] Evicting dirty page " << victimId
                      << " — flushing to disk first.\n";
            disk.writePage(cache.at(victimId));
        } else {
            std::cout << "[BUFFER] Evicting clean page " << victimId
                      << " — no flush needed.\n";
        }
 
        cache.erase(victimId);
        dirty.erase(victimId);
        timestamps.erase(victimId);
        return;
    }
}

Page& BufferPool::getPage(int pageId) {
    if (cache.count(pageId)) {
        
        std::cout << "[BUFFER] Cache hit  — Page " << pageId << "\n";
        touch(pageId);
        return cache.at(pageId);
    }
 

    std::cout << "[BUFFER] Cache miss — Page " << pageId << " loading from disk.\n";
 
    if (cache.size() >= capacity) {
        std::cout << "[BUFFER] Cache full (" << capacity << " pages). Evicting LRU page.\n";
        evict();
    }
 
    
    auto pages = disk.readAllPages();
    for (auto& p : pages) {
        if (p.getPageId() == pageId) {
            cache.emplace(pageId, std::move(p));
            dirty[pageId] = false;
            touch(pageId);
            return cache.at(pageId);
        }
    }
 

    std::cerr << "[BUFFER ERROR] Page " << pageId << " not found on disk.\n";

    cache.emplace(pageId, Page(pageId));
    dirty[pageId] = false;
    touch(pageId);
    return cache.at(pageId);
}

void BufferPool::markDirty(int pageId) {
    dirty[pageId] = true;
}
 
bool BufferPool::inCache(int pageId) {
    return cache.count(pageId) > 0;
}

void BufferPool::flushAll() {
    std::cout << "[BUFFER] Flushing all dirty pages to disk.\n";
    int flushed = 0;
    for (auto& [pageId, page] : cache) {
        if (dirty.count(pageId) && dirty[pageId]) {
            disk.writePage(page);
            dirty[pageId] = false;
            flushed++;
        }
    }
    std::cout << "[BUFFER] Flushed " << flushed << " dirty page(s).\n";
}

void BufferPool::displayStats() {
    std::cout << "\n[BUFFER POOL STATS]\n";
    std::cout << "  Capacity : " << capacity << " pages\n";
    std::cout << "  Cached   : " << cache.size() << " pages\n";
    std::cout << "  Free     : " << (capacity - cache.size()) << " slots\n";
    std::cout << "  Pages in cache: ";
    for (const auto& [pageId, _] : cache) {
        std::cout << pageId;
        if (dirty.count(pageId) && dirty.at(pageId))
            std::cout << "(dirty) ";
        else
            std::cout << "(clean) ";
    }
    std::cout << "\n";
}