#include <iostream>
#include <cstring>

int string_size(char* str) {
    int len = 0;
    while(str[len] != '\0') {
        ++len;
    }
    ++len;
    return len;
}

void Parse(bool& words_file_flag, bool& filter_file_flag, 
    char* words_file_name, char* filter_file_name, int argc, char* argv[5]) {
    
        for (int i = 1; i < argc - 1; i++) {
            int j = 0;
            int len = string_size(argv[i + 1]);
            if (strncmp(argv[i], "--input", 7) == 0) {
                words_file_flag = true;                
                while(j < len) {
                    words_file_name[j] = argv[i][j];
                    j++;
                }
                words_file_name[j] = '\0';
            } 
            else if (strncmp(argv[i], "--filterdb", 10) == 0) {
                filter_file_flag = true;
                while(j < len) {
                    filter_file_name[j] = argv[i][j];
                    j++;
                }
                filter_file_name[j] = '\0';
            }
    }
}