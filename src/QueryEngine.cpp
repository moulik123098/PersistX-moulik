#include "QueryEngine.h"
#include <iostream>
#include <iomanip>

QueryEngine::QueryEngine(BufferPool& buffer, std::vector<int>& pageIds)
    : buffer(buffer), pageIds(pageIds) {}
 
std::optional<std::string> QueryEngine::get(std::string& key) {
    for (int id : pageIds) {
        auto result = buffer.getPage(id).search(key);
        if (result) return result;
    }
    return std::nullopt;
}

std::vector<Record> QueryEngine::listAll() {
    std::vector<Record> results;
    for (int id : pageIds)
        for (auto& record : buffer.getPage(id).getSlots())
            results.push_back(record);
    return results;
}

std::vector<Record> QueryEngine::filterByPrefix(std::string& prefix) {
    std::vector<Record> results;
    for (int id : pageIds)
        for (auto& record : buffer.getPage(id).getSlots())
            if (record.key.rfind(prefix, 0) == 0)
                results.push_back(record);
    return results;
}

void QueryEngine::displayAll() {
    auto results = listAll();
    std::cout << "\n[QUERY] All records (" << results.size() << " total)\n";
    std::cout << std::string(40, '-') << "\n";
    if (results.empty()) { std::cout << "  (no records)\n"; return; }
    for (auto& r : results)
        std::cout << "  " << std::setw(16) << std::left << r.key
                  << " => " << r.value << "\n";
}

void QueryEngine::displayPrefix(std::string& prefix) {
    auto results = filterByPrefix(prefix);
    std::cout << "\n[QUERY] Prefix \"" << prefix
              << "\" (" << results.size() << " found)\n";
    std::cout << std::string(40, '-') << "\n";
    if (results.empty()) { std::cout << "  (no matches)\n"; return; }
    for (auto& r : results)
        std::cout << "  " << std::setw(16) << std::left << r.key
                  << " => " << r.value << "\n";
}