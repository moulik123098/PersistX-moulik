#include "PageManager.h"

#include <iostream>
#include<algorithm>

PageManager::PageManager() : nextPageId(0), disk("data"), buffer(disk) {
   
    auto pages = disk.readAllPages();
 
    if (pages.empty()) {
        std::cout << "[PAGE MANAGER] No existing data found. Starting fresh.\n";
        allocatePage();
    } else {
        
        for (auto& page : pages) {
            pageIds.push_back(page.getPageId());
            if (page.getPageId() >= nextPageId)
                nextPageId = page.getPageId() + 1;
        }
        std::cout << "[PAGE MANAGER] Restored " << pages.size()
                  << " page(s) from disk.\n";
    }
}

Page &PageManager::allocatePage()
{
    int id = nextPageId++;
    pageIds.push_back(id);
 
    
    Page newPage(id);
    disk.writePage(newPage);
 
    
    return buffer.getPage(id);
}

void PageManager::insert(std::string& key, std::string& value) {
    int lastId = pageIds.back();
    Page& lastPage = buffer.getPage(lastId);

    if (lastPage.isFull()) {
        std::cout << "[PAGE MANAGER] Page " << lastId
                  << " is full. Allocating new page.\n";
        allocatePage();
        lastId = pageIds.back();
    }
 
    buffer.getPage(lastId).insert(key, value);
    buffer.markDirty(lastId);
}

std::optional<std::string> PageManager::search(std::string &key)
{

    for (int id : pageIds) {
        
        auto result = const_cast<BufferPool&>(buffer).getPage(id).search(key);
        if (result) return result;
    }
    return std::nullopt;
}

bool PageManager::remove(std::string &key)
{
    for (int id : pageIds) {
        Page& page = buffer.getPage(id);
        if (page.remove(key)) {
            buffer.markDirty(id);
            return true;
        }
    std::cout << "[PAGE MANAGER] Key \"" << key << "\" not found in any page.\n";
    return false;
    }
}

void PageManager::display()
{
    std::cout << "\n[PAGE MANAGER] " << pageIds.size() << " page(s), "
              << recordCount() << " total record(s)\n";
    std::cout << std::string(42, '=') << "\n";

    for (int id : pageIds) {
        buffer.getPage(id).display();
        std::cout << "\n";
    }
}

Page *PageManager::getPage(int pageId)
{
    for (int id : pageIds) {
        if (id == pageId) return &buffer.getPage(pageId);
    }
    return nullptr;
}

size_t PageManager::recordCount()
{
    size_t total = 0;
    for (int id : pageIds)
    {
        total += buffer.getPage(id).getRecordCount();
    }
    return total;
}
