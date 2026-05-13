#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <string>
#include <vector>
#include "Page.h"

class DiskManager {
private:
    std::string Dir;
    std::string getFilePath(int pageId) const;

public:
    DiskManager(const std::string& dir = "data");
    void writePage(const Page& page);
    std::vector<Page> readAllPages();
    void deletePage(int pageId);
};

#endif