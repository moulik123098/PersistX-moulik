#include <iostream>
#include <string>

#include "PageManager.h"
#include "QueryEngine.h"


void Menu()
{

    std::cout << "\n--- Page Manager ---\n";
    std::cout << "1. Insert record\n";
    std::cout << "2. Remove record\n";
    std::cout << "3. Get by key\n";
    std::cout << "4. List all record\n";
    std::cout << "5. Filter by prefix\n";
    std::cout << "6. Display all pages\n";
    std::cout << "7. Buffer Pool stats\n";
    std::cout << "8. Flush to disk\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter choice: ";
}

int main()
{

    PageManager pm;
    QueryEngine qe(pm.getBuffer(), pm.getPageIds());

    short choice;

    while (true)
    {

        Menu();
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cout << "Invalid input. Enter a number.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        std::string key, value,prefix;

        switch (choice)
        {

        case 1:
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Enter value: ";
            std::cin >> value;
            pm.insert(key, value);
            break;

        case 2:
            std::cout << "Enter key: ";
            std::cin >> key;
            pm.remove(key);
            break;

        case 3:
            std::cout << "Enter key: ";
            std::cin >> key;
            {
                auto result = qe.get(key);
                if(result) std::cout << "Found: " << *result << "\n";
                    else std::cout << "Not found.\n";
                }
                break;
            

        case 4:
            qe.displayAll();
            break;

        case 5:
        {
            std::cout << "Enter prefix: ";
            std::cin >> prefix;
            qe.displayPrefix(prefix);
            break;
        }

        case 6:
            pm.display();
            break;

        case 7:
                pm.displayBufferStats();
                break;

        case 8:
                pm.flushAll();
                break;

        case 9:
                pm.flushAll();  
                std::cout << "Exiting...\n";
                return 0;

        default:
            std::cout << "Invalid choice. Enter 1-5.\n";
        }
    }
}