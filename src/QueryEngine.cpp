#include "QueryEngine.h"
#include <iostream>
#include <iomanip>

QueryEngine::QueryEngine(BufferPool& buffer, std::vector<int>& pageIds)
    : buffer(buffer), pageIds(pageIds) {
    BuildIndex();
}

void QueryEngine::BuildIndex() {
    Index.clear();
    std::cout << "[INDEX] Building index...\n";
    int total = 0;
    for (int id : pageIds) {
        Page& page = buffer.getPage(id);
        const auto& slots = page.getSlots();
        for (int i = 0; i < (int)slots.size(); i++) {
            Index[slots[i].key] = {id, i};
            total++;
        }
    }
    std::cout << "[INDEX] Built with " << total << " entries.\n";
}

void QueryEngine::rebuild() {
    std::cout << "[INDEX] Rebuilding index from scratch...\n";
    BuildIndex();
}

void QueryEngine::IndexInsert(const std::string& key, int pageId, int slotIndex) {
    Index[key] = {pageId, slotIndex};
    std::cout << "[INDEX] Added \"" << key << "\" → page " << pageId
              << " slot " << slotIndex << "\n";
}

void QueryEngine::IndexRemove(const std::string& key) {
    Index.erase(key);
    std::cout << "[INDEX] Removed \"" << key << "\" from index.\n";
}

std::optional<std::string> QueryEngine::get(const std::string& key) {
    auto it = Index.find(key);
    if (it == Index.end()) return std::nullopt;

    Page& page = buffer.getPage(it->second.pageId);
    const auto& slots = page.getSlots();
    int slot = it->second.slotIndex;

    if (slot < (int)slots.size() && slots[slot].key == key)
        return slots[slot].value;

    return page.search(key);
}

std::vector<Record> QueryEngine::listAll() {
    std::vector<Record> results;
    for (int id : pageIds)
        for (const auto& record : buffer.getPage(id).getSlots())
            results.push_back(record);
    return results;
}

std::vector<Record> QueryEngine::filterByPrefix(const std::string& prefix) {
    std::vector<Record> results;
    auto it = Index.lower_bound(prefix);
    while (it != Index.end() && it->first.rfind(prefix, 0) == 0) {
        Page& page = buffer.getPage(it->second.pageId);
        auto val = page.search(it->first);
        if (val) results.push_back({it->first, *val});
        ++it;
    }
    return results;
}

std::vector<Record> QueryEngine::rangeQuery(const std::string& startKey,
                                             const std::string& endKey) {
    std::vector<Record> results;
    auto it  = Index.lower_bound(startKey);
    auto end = Index.upper_bound(endKey);
    for (; it != end; ++it) {
        Page& page = buffer.getPage(it->second.pageId);
        auto val = page.search(it->first);
        if (val) results.push_back({it->first, *val});
    }
    return results;
}

void QueryEngine::displayAll() {
    auto results = listAll();
    std::cout << "\n[QUERY] All records (" << results.size() << " total)\n";
    std::cout << std::string(40, '-') << "\n";
    if (results.empty()) { std::cout << "  (no records)\n"; return; }
    for (const auto& r : results)
        std::cout << "  " << std::setw(16) << std::left << r.key
                  << " => " << r.value << "\n";
}

void QueryEngine::displayPrefix(const std::string& prefix) {
    auto results = filterByPrefix(prefix);
    std::cout << "\n[QUERY] Prefix \"" << prefix
              << "\" (" << results.size() << " found)\n";
    std::cout << std::string(40, '-') << "\n";
    if (results.empty()) { std::cout << "  (no matches)\n"; return; }
    for (const auto& r : results)
        std::cout << "  " << std::setw(16) << std::left << r.key
                  << " => " << r.value << "\n";
}

void QueryEngine::displayRange(const std::string& startKey,
                                const std::string& endKey) {
    auto results = rangeQuery(startKey, endKey);
    std::cout << "\n[QUERY] Range [\"" << startKey << "\" → \"" << endKey
              << "\"] (" << results.size() << " found)\n";
    std::cout << std::string(40, '-') << "\n";
    if (results.empty()) { std::cout << "  (no matches)\n"; return; }
    for (const auto& r : results)
        std::cout << "  " << std::setw(16) << std::left << r.key
                  << " => " << r.value << "\n";
}

void QueryEngine::displayIndex() {
    std::cout << "\n[INDEX] " << Index.size() << " entries (sorted)\n";
    std::cout << std::string(40, '-') << "\n";
    if (Index.empty()) { std::cout << "  (empty)\n"; return; }
    for (const auto& [key, entry] : Index)
        std::cout << "  " << std::setw(16) << std::left << key
                  << " → page " << entry.pageId
                  << " slot " << entry.slotIndex << "\n";
}