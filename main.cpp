#include <iostream>
#include <cstring>
#include <fstream>

struct STR {
    char str_[1024];
    STR* next = nullptr;
};


void Word(STR* tmp, int& ind) {
    int i = ind;
    while (tmp->str_[i] != '\0' || tmp->str_[i] != ' ') {
        ++i;
    }
    ind = i - 1;
}

void CheckLine(char* filt, STR* tmp) {
    int ind1 = 0;
    while (true) {
        int ind2 = ind1 + 1;
        if (ind2 > 1024) {
            return;
        }
        Word(tmp, ind2);
        if (ind2 > 1024) {
            return;
        }


        ind1 = ind2 + 2;
    }
}

int main(int argc, char** argv) {
    if (argc != 5) {
        return 1;
    }
    int i = 1;
    char* input;
    char* filterdb;
    while (i < 4) {
        if (strcmp(argv[i], "--input") == 0) {
            input = argv[i+1];
        } else if (strcmp(argv[i], "--filterdb") == 0) {
            filterdb = argv[i+1];
        }
        i += 2;
    }
    STR* head = new STR();
    STR* tmp = head;
    std::ifstream in_file (input, std::ios::binary);
    while (in_file >> (tmp->str_) ) {
        tmp->next = new STR();
        tmp = tmp->next;
    }

    std::ifstream filt_file (filterdb, std::ios::binary);
    char filt[1024];
    while (filt_file >> filt) {
        STR* tmp = head;
        while (tmp != nullptr) {
            CheckLine(filt, tmp);
            tmp = tmp->next;
        }       
    }
    
    std::ofstream ot_file ("output.txt", std::ios::binary);
    STR* tmp = head;
    while (tmp != nullptr) {
        ot_file << tmp->str_;
        tmp = tmp->next;
    } 

    STR* tmp = head;
    STR* prev;
    while (tmp != nullptr) {
        prev = tmp;
        tmp = tmp->next;
        delete prev;
    }
    return 0;
}
