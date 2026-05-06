#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <vector>
#include <string>
#include <optional>

#include "Page.h"
#include "DiskManager.h"

class PageManager
{
private:
    std::vector<Page> pages;
    int nextPageId;
    DiskManager disk;

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
        return pages.size();
    }
    size_t recordCount();
};

#endif