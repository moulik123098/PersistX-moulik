#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <string>
#include <optional>

#include "Record.h"

static constexpr size_t PAGE_SIZE = 4096;
static constexpr size_t RECORD_SIZE = 64;
static constexpr size_t MAX_RECORDS = PAGE_SIZE / RECORD_SIZE;

class Page
{
private:
    std::vector<Record> slots;
    size_t usedSpace;
    int pageId;

public:
    Page(int id);

    bool insert(std::string &key, std::string &value);
    std::optional<std::string> search(std::string &key);
    bool remove(std::string &key);
    void display();

    int getPageId()
    {
        return pageId;
    }

    size_t getUsedSpace()
    {
        return usedSpace;
    }

    size_t getFreeSpace()
    {
        return PAGE_SIZE - usedSpace;
    }

    size_t getRecordCount()
    {
        return slots.size();
    }

    bool isFull()
    {
        return usedSpace + RECORD_SIZE > PAGE_SIZE;
    }

    std::vector<Record>& getSlots(){
        return slots;
    }
};

#endif