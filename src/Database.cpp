#include "Database.h"

#include <iostream>

void Database::insertRecord(std::string &key, std::string &value)
{
    for (auto &record : records)
    {
        if (record.key == key)
        {
            record.value = value;
            std::cout << "[UPDATE] Key \"" << key << "\" updated to \"" << value << "\"\n";
            return;
        }
    }
    records.push_back({key, value});
    std::cout << "[INSERT] Key \"" << key << "\" = \"" << value << "\"\n";
}

std::optional<std::string> Database::searchRecord(std::string &key)
{
    for (auto &record : records)
    {
        if (record.key == key)
            return record.value;
    }
    return "";
}

bool Database::removeRecord(std::string &key)
{
    for (auto it = records.begin(); it != records.end(); it++)
    {
        if (it->key == key)
        {
            records.erase(it);
            std::cout << "[REMOVE] Key \"" << key << "\" deleted.\n";
            return true;
        }
    }
    std::cout << "[REMOVE] Key \"" << key << "\" not found.\n";
    return false;
}

void Database::displayRecords()
{
    if (records.empty())
    {
        std::cout << "[DISPLAY] Database is empty.\n";
        return;
    }
    std::cout << "[DISPLAY] " << records.size() << " record(s):\n";
    for (size_t i = 0; i < records.size(); ++i)
    {
        std::cout << "  [" << i << "] " << records[i].key
                  << " => " << records[i].value << "\n";
    }
}

size_t Database::size()
{
    return records.size();
}