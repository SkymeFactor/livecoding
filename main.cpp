#include <iostream>
#include <cstdio>

const uint8_t kNumberOfFirstLetter = 33;

struct Node {
    Node* prev_;
    Node* next_;
    const char* data_;
    size_t count_;
    size_t size_;
    Node(const char* data, Node* prev, size_t size) {
        data_ = data;
        prev_ = prev;
        size_ = size;
        next_ = nullptr;
        count_ = 0;
    }
    Node() {
        Node* prev_ = nullptr;
        Node* next_ = nullptr;
        size_t count_ = 0;
        size_t size_ = 0;
    }
};


class FileManager {
private:
    const char* NameOfFile;
    Node* Start;
public:
    FileManager(const char* file_name) {
        NameOfFile = file_name;
    }

    bool GetFilter() { // функция создает односвязный список, содержащий все правила фильтрации
        Start = new Node();
        Node* current_node = Start;
        bool is_ok = 0;
        FILE* file = std::fopen(NameOfFile, "r");
        if (!file) {
            std::perror("File opening failed");
            return is_ok;
        }
        int c;
        size_t i = 0;

        while ((c = std::fgetc(file)) != EOF) {
            char filter[128];
            size_t size = 0;
            while ((c = std::fgetc(file)) != EOF && static_cast<char> (c) >= kNumberOfFirstLetter) {
                filter[i] = static_cast<char> (c);
                size++;
            }
            filter[i + 1] = 0;
            current_node->next_ = new Node(filter, current_node, size);
            current_node = current_node->next_;
        }
        
        if (std::ferror(file))
            std::puts("I/O error when reading");
        else if (std::feof(file)) {
            std::puts("End of file reached successfully");
            is_ok = 0;
        }
        
        return !is_ok;
    }

    bool Read() {
        Node* current_node = Start;
        bool is_ok = 0;
        FILE* file_input = std::fopen(NameOfFile, "r");
        FILE* file_output = std::fopen(NameOfFile, "w+");
        if (!file_input) {
            std::perror("File opening failed");
            return is_ok;
        }
        int c;
        int i = 0;
        while ((c = std::fgetc(file_input)) != EOF) {
            char word[128];
            size_t size = 0;
            while ((c = std::fgetc(file_input)) != EOF && static_cast<char> (c) >= kNumberOfFirstLetter) {
                word[i] = static_cast<char> (c);
                size++;
            }
            word[i + 1] = 0;
            if (!Find(word)) {
                std::fwrite(word, sizeof(word[0]), size, file_output);
            }
        }

        if (std::ferror(file_input))
            std::puts("I/O error when reading");
        else if (std::feof(file_input)) {
            std::puts("End of file reached successfully");
            is_ok = 0;
        }
        return !is_ok;
    }

    bool Find(const char* word) {
        Node* current_node = Start;
        bool finded = 0;
        while (current_node->next_ != nullptr) {
            if (!std::strncmp(word, current_node->data_, current_node->size_)) {
                finded = 1;
                current_node->count_ += 1;
                break;
            }
            current_node = current_node->next_;
        }
        return finded;
    }
};



int main(int argc, char** argv){
    const char* input = "--input";
    const char* filterdb = "--filterdb";
    const char* input_path;
    const char* filterdb_path;
    for (size_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], input)) {
            input_path = argv[i + 1];
            // std::cout << "red";
        }
        if (!strcmp(argv[i], filterdb)) {
            filterdb_path = argv[i + 1];
        }
    }
}
