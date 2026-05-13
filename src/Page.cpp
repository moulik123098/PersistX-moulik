#include "Page.h"
#include <iostream>
#include <iomanip>

Page::Page(int id) : usedSpace(0), pageId(id) {}

bool Page::insert(const std::string& key, const std::string& value) {
    if (isFull()) {
        std::cout << "[PAGE FULL] Cannot insert \"" << key << "\". No space left.\n";
        return false;
    }
    for (auto& record : slots) {
        if (record.key == key) {
            record.value = value;
            std::cout << "[PAGE UPDATE] Key \"" << key << "\" updated to \"" << value << "\"\n";
            return true;
        }
    }
    slots.push_back({key, value});
    usedSpace += RECORD_SIZE;
    std::cout << "[PAGE INSERT] Key \"" << key << "\" = \"" << value << "\""
              << " | Used: " << usedSpace << "/" << PAGE_SIZE << " bytes\n";
    return true;
}

std::optional<std::string> Page::search(const std::string& key) const {
    for (const auto& record : slots)
        if (record.key == key) return record.value;
    return std::nullopt;
}

bool Page::remove(const std::string& key) {
    for (auto it = slots.begin(); it != slots.end(); ++it) {
        if (it->key == key) {
            slots.erase(it);
            usedSpace -= RECORD_SIZE;
            std::cout << "[PAGE REMOVE] Key \"" << key << "\" deleted."
                      << " | Used: " << usedSpace << "/" << PAGE_SIZE << " bytes\n";
            return true;
        }
    }
    std::cout << "[PAGE REMOVE] Key \"" << key << "\" not found.\n";
    return false;
}

void Page::display() const {
    std::cout << "  [Page " << pageId << "] "
              << slots.size() << "/" << MAX_RECORDS << " records | "
              << usedSpace << "/" << PAGE_SIZE << " bytes used | "
              << getFreeSpace() << " bytes free\n";
    std::cout << "  " << std::string(40, '-') << "\n";
    if (slots.empty()) { std::cout << "  (empty)\n"; return; }
    for (size_t i = 0; i < slots.size(); i++)
        std::cout << "  [" << std::setw(2) << i << "] "
                  << std::setw(16) << std::left << slots[i].key
                  << " => " << slots[i].value << "\n";
}