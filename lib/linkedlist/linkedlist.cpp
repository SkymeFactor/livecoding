#include <cstring>
#include <iostream>

#include "linkedlist.hpp"

using namespace linkedlist;

LinkedList::~LinkedList() {
    Node* cur = root_;
    Node* next = cur->next;
    while (true) {
        delete cur;
        cur = next;

        if (cur == nullptr) {
            break;
        }

        next = cur->next;
    }
}

void LinkedList::Add(char* element) {
    Node* cur = root_;

    while (true) {
        if (strcmp(element, cur->str) == 0) {
            return;
        }
        
        if (cur->next == nullptr) {
            ++size_;
            cur->next = new Node;
            cur->next->str = element;
            return;
        }

        cur = cur->next;
    }
}

void LinkedList::PrintByFilter(char** filters, size_t num_filters) {
    Node* cur = root_;
    
    while (cur != nullptr) {
        bool has_match = false;
        for (size_t i = 0; i < num_filters; ++i) {
            if (strstr(cur->str, filters[num_filters]) != nullptr) {
                has_match = true;
                break;
            }
        }

        if (!has_match) {
            std::cout << cur->str << std::endl;
        }

        cur = cur->next;
    }
}