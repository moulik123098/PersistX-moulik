#include "PageManager.h"

#include <iostream>
#include<algorithm>

PageManager::PageManager() : nextPageId(0), disk("data") {
   
    pages = disk.readAllPages();
 
    if (pages.empty()) {
        std::cout << "[PAGE MANAGER] No existing data found. Starting fresh.\n";
        allocatePage();
    } else {
        
        for (auto& page : pages) {
            if (page.getPageId() >= nextPageId)
                nextPageId = page.getPageId() + 1;
        }
        std::cout << "[PAGE MANAGER] Restored " << pages.size()
                  << " page(s) from disk.\n";
    }
}

Page &PageManager::allocatePage()
{
    pages.emplace_back(nextPageId++);
    std::cout << "[PAGE MANAGER] Allocated Page " << (nextPageId - 1) << "\n";
    return pages.back();
}

void PageManager::insert(std::string& key, std::string& value) {
    if (pages.back().isFull()) {
        std::cout << "[PAGE MANAGER] Page " << pages.back().getPageId()
                  << " is full. Allocating new page.\n";
        allocatePage();
    }
 
    pages.back().insert(key, value);
    disk.writePage(pages.back()); 
}

std::optional<std::string> PageManager::search(std::string &key)
{

    for (auto &page : pages)
    {
        auto result = page.search(key);
        if (result)
            return result;
    }
    return std::nullopt;
}

bool PageManager::remove(std::string &key)
{
    for (auto &page : pages)
    {
        disk.writePage(page);
        if (page.remove(key))
            return true;
    }
    std::cout << "[PAGE MANAGER] Key \"" << key << "\" not found in any page.\n";
    return false;
}

void PageManager::display()
{
    std::cout << "\n[PAGE MANAGER] " << pages.size() << " page(s), "
              << recordCount() << " total record(s)\n";
    std::cout << std::string(42, '=') << "\n";

    for (auto &page : pages)
    {
        page.display();
        std::cout << "\n";
    }
}

Page *PageManager::getPage(int pageId)
{
    for (auto &page : pages)
    {
        if (page.getPageId() == pageId)
            return &page;
    }
    return nullptr;
}

size_t PageManager::recordCount()
{
    size_t total = 0;
    for (auto &page : pages)
    {
        total += page.getRecordCount();
    }
    return total;
}
