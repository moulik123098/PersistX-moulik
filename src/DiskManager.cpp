#include "DiskManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

DiskManager::DiskManager(const std::string& dir) : Dir(dir) {
    if (!fs::exists(Dir)) {
        fs::create_directory(Dir);
        std::cout << "[DISK] Created data directory: " << Dir << "\n";
    }
}

std::string DiskManager::getFilePath(int pageId) const {
    return Dir + "/page_" + std::to_string(pageId) + ".txt";
}

void DiskManager::writePage(const Page& page) {
    std::string path = getFilePath(page.getPageId());
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[DISK ERROR] Could not open file for writing: " << path << "\n";
        return;
    }
    file << page.getPageId() << "\n";
    for (const auto& record : page.getSlots())
        file << record.key << "|" << record.value << "\n";
    std::cout << "[DISK] Wrote page " << page.getPageId()
              << " (" << page.getRecordCount() << " records) to " << path << "\n";
}

std::vector<Page> DiskManager::readAllPages() {
    std::vector<Page> pages;
    if (!fs::exists(Dir)) return pages;

    std::vector<fs::path> files;
    for (auto& entry : fs::directory_iterator(Dir))
        if (entry.path().extension() == ".txt")
            files.push_back(entry.path());
    std::sort(files.begin(), files.end());

    for (auto& filePath : files) {
        std::ifstream file(filePath);
        if (!file.is_open()) continue;
        std::string line;
        if (!std::getline(file, line)) continue;
        int pageId = std::stoi(line);
        Page page(pageId);
        while (std::getline(file, line)) {
            size_t delimiter = line.find('|');
            if (delimiter == std::string::npos) continue;
            page.insert(line.substr(0, delimiter), line.substr(delimiter + 1));
        }
        std::cout << "[DISK] Loaded page " << pageId
                  << " (" << page.getRecordCount() << " records) from "
                  << filePath.filename() << "\n";
        pages.push_back(std::move(page));
    }
    return pages;
}

void DiskManager::deletePage(int pageId) {
    std::string path = getFilePath(pageId);
    if (fs::exists(path)) {
        fs::remove(path);
        std::cout << "[DISK] Deleted page file: " << path << "\n";
    }
}