#include <iostream>
#include <fstream>
#include "parse.h"

class vect {
private:
    char** data_;
    size_t capacity_;
    size_t size_;

    void reallocating() {
        size_t new_capacity_ = capacity_ * 2;
        char** new_data_ = new char*[new_capacity_];
        for (size_t i = 0; i < capacity_; i++) {
            new_data_[i] = data_[i];
        }
        for (size_t i = 0; i < capacity_; i++) {
            delete[] data_[i];
        }
        delete[] data_;
        data_ = new_data_;
        capacity_ = new_capacity_;
    }

public:
    vect() : data_(new char*[4]), capacity_(4), size_(0) {}

    vect(size_t cap) : data_(new char*[cap]), capacity_(cap), size_(0) {}

    vect (const vect& other) : data_(new char*[other.capacity_]), capacity_(other.capacity_), size_(other.size_) {
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }

    ~vect() {
        delete[] data_;
    }

    void push(char* val) {
        if (size_ == capacity_) {
            reallocating();
        }
        int val_len = string_size(val);
        int i = 0;
        for (; i < val_len; ++i) {
            data_[size_][i] = val[i];
        }
        data_[size_][i] = '\0';
        size_++;
    }

    size_t size() {
        return size_;
    }
};

void clear(char* buffer_word) {
    int clear_ind = 1024;
    while (clear_ind >= 0) {
        buffer_word[clear_ind] = 0;
        clear_ind--;
    }
}

int main(int argcc, char** argvv){
    int argc = 5;
    char* argv[5] = {"app.exe", "--input", "words.txt", "--filterdb", "filter.txt"};
    if (argc < 5) {
        std::cout << "not enough args" << std::endl;
        return -1;
    }
    bool words_file_flag = false;
    bool filter_file_flag = false;
    char words_file_name[256] = {};
    char filter_file_name[256] = {};
    Parse(words_file_flag, filter_file_flag, words_file_name, filter_file_name, argc, argv);
    
    if (!words_file_flag || !filter_file_flag) {
        std::cout << "incorrect args" << std::endl;
        return -1;
    }
    
    std::ifstream input_words_file;
    std::ifstream input_filter_file;
    std::ofstream output_file;
    input_words_file.open(words_file_name);
    input_filter_file.open(filter_file_name);
    
    if (!input_words_file.is_open() || !input_filter_file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    
    char buffer_string[1024] = {};
    char* buffer_word = {};
    vect vtr(1);
    vect result(1);

    while(!input_filter_file.eof() && input_filter_file.getline(buffer_string, 1024)) {
        int j = 0;
        while(buffer_string[j] != ' ') {
            buffer_word[j] = buffer_string[j];
            ++j;
        }
        buffer_word[j] = '\0'; 
        vtr.push(buffer_word);
    }
    
    while(!input_words_file.eof() && input_words_file.getline(buffer_string, 1024)) {
        int j = 0;
        
        while(buffer_string[j] != ' ') {
            buffer_word[j] = buffer_string[j];
            j++;
        }

        buffer_word[j] = '\0';
        int buffer_word_size = string_size(buffer_word);

        for (int j = 0; j < buffer_word_size; j++) {
            char* buff_buff_word;
            int tmp_j = j;
            for (;tmp_j < buffer_word_size; tmp_j++) {
                buff_buff_word[tmp_j] = buffer_word[j];
            }
            for (size_t i = 0; i < vtr.size(); i++) {
                // не успел доделать реализацию
            }
        }       

        clear(buffer_word);
    }
    return 0;
}
