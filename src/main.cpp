#include <iostream>
#include <string>

#include "PageManager.h"

void Menu()
{

    std::cout << "\n--- Page Manager ---\n";
    std::cout << "1. Insert record\n";
    std::cout << "2. Search record\n";
    std::cout << "3. Remove record\n";
    std::cout << "4. Display all pages\n";
    std::cout << "5. Get page by ID\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter choice: ";
}

int main()
{

    PageManager pm;

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

        std::string key, value;

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
            {
                auto result = pm.search(key);
                if (result)
                    std::cout << "Found: " << *result << "\n";
                else
                    std::cout << "Key not found.\n";
            }
            break;

        case 3:
            std::cout << "Enter key: ";
            std::cin >> key;
            pm.remove(key);
            break;

        case 4:
            pm.display();
            break;

        case 5:
        {
            std::cout << "Enter Page ID: ";
            int pageId;
            std::cin>>pageId;
            Page* p = pm.getPage(pageId);
            if(p) p->display();
            else   std::cout << "Page " << pageId << " not found.\n";
            break;
        }

        case 6:
            std::cout << "Exiting.\n";
                return 0;
            break;

        default:
            std::cout << "Invalid choice. Enter 1-5.\n";
        }
    }
}