#pragma once

#include <cstddef>

namespace linkedlist {

struct Node {
    const char* str;
    Node* next;
};

class LinkedList {
public:
LinkedList()
    : root_(new Node({"", nullptr}))
    , size_(0) {}
~LinkedList();
LinkedList(const LinkedList& other) = delete;
LinkedList(LinkedList&& other) noexcept = delete;
LinkedList& operator=(LinkedList& other) = delete;
LinkedList& operator=(LinkedList&& other) noexcept = delete;

void Add(char* element);
void PrintByFilter(char** filters, size_t num_filters);

private:
Node* root_;
size_t size_;
};

} // namespace doublyll