#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <string>
#include <optional>

#include "Record.h"

static constexpr size_t PAGE_SIZE   = 4096;
static constexpr size_t RECORD_SIZE = 64;
static constexpr size_t MAX_RECORDS = PAGE_SIZE / RECORD_SIZE;

class Page {
private:
    std::vector<Record> slots;
    size_t usedSpace;
    int pageId;

public:
    Page(int id);

    bool insert(const std::string& key, const std::string& value);
    std::optional<std::string> search(const std::string& key) const;
    bool remove(const std::string& key);
    void display() const;

    int    getPageId()      const { return pageId; }
    size_t getUsedSpace()   const { return usedSpace; }
    size_t getFreeSpace()   const { return PAGE_SIZE - usedSpace; }
    size_t getRecordCount() const { return slots.size(); }
    bool   isFull()         const { return usedSpace + RECORD_SIZE > PAGE_SIZE; }

    const std::vector<Record>& getSlots() const { return slots; }
};

#endif