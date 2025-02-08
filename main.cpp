#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string.h>
#include <string>
#include <utility>

struct Args {
  char* input_path = nullptr;
  char* filter_path = nullptr;
};

bool cmpstr(char* l, char* r) {
  size_t ind = 0;
  while (l[ind] != '\0' && r[ind] != '\0') {
    if (!((l[ind] - 'a') == r[ind] - 'a' || l[ind] - 'a' == r[ind] - 'A' 
                                        || l[ind] - 'A' == r[ind] - 'A' 
                                        || l[ind] - 'A' == r[ind] - 'a') ) {
      return false;
    }
    ++ind;
  }
  return l[ind] == '\0' && r[ind] == '\0';
}
bool parse(int argc, char** argv, Args* args) {
  if (argc != 5) {
    return false;
  }
  if (!strcmp(argv[1], "--input")) {
    args->input_path = argv[2];
    args->filter_path = argv[4];
  }
  else {
    args->filter_path = argv[2];
    args->input_path = argv[4];
  }
  return true;
}
size_t count_words(char* path) {
  std::ifstream file(path);
  size_t words = 1;
  char* buffer = new char[129];
  while (!file.eof()){
    file.read(buffer, 129);
    for (size_t i = 0; i != 129; ++i) {
      if (buffer[i] == '\0') break;
      if (buffer[i] == ' ' || buffer[i] == '\n') {
        ++words;
      }
    }
  }
  return words;
}

void input_filter(std::ifstream* file, std::pair<size_t, char*>* filters) {
  size_t ind = 0;
  size_t ind_word = 0;
  char* buffer = new char[129];
  while (!file->eof()) {
    file->getline(buffer, 129);
    char* new_word = new char[129];
    ind_word = 0;
    for (size_t i = 0; i != 129; ++i) {
      if (buffer[i] == '\0') {
        new_word[ind_word] = buffer[i];
        filters[ind].second = new_word;
        filters[ind].first = 0;
        ++ind;
        ind_word = 0;
        break; 
      }
      if (buffer[i] == ' ' || buffer[i] == '\n') {
        new_word[ind_word] = '\0';
        filters[ind].second = new_word;
        filters[ind].first = 0;
        ind_word = 0;
        ++ind;
      }
      else {
        new_word[ind_word] = buffer[i]; 
        ++ind_word;
      }
    }
  }
}
bool find_filter(std::pair<size_t, char*>* filters, char* word, size_t size_filter) {
  for (size_t i = 0; i != size_filter; ++i) {
    if (cmpstr(filters[i].second, word)) {
      ++filters[i].first;
      return true;
    }
  }
  return false;
}
void ans(std::pair<size_t, char*>* filters, char* input_path,size_t size_filter) {
  size_t min_size = 2000;
  size_t max_size = 0;
  size_t ind_word = 0;
  std::ifstream file(input_path);
  std::ofstream output("out.txt");
  char* buffer = new char[1025];
  while (!file.eof()) {
    file.getline(buffer, 1025);
    char* word = new char[1025];
    ind_word = 0;
    for (size_t i = 0; i != 1025; ++i) {
      if (buffer[i] == '\0') {
        word[ind_word] = '\0';
        if (find_filter(filters, word, size_filter)) {
          if (ind_word < min_size && ind_word != 0) {
            min_size = ind_word;
          }
          if (ind_word > max_size && ind_word != 0) {
            max_size = ind_word;
          }
        } else {
          for (size_t in = 0;word[in] != '\0'; ++in) {
            output << word[in];
          }
          output << " ";
        }
        ind_word = 0;
        break; 
      }
      if (buffer[i] == ' ' || buffer[i] == '\n') {
        word[ind_word] = '\0';
        if (find_filter(filters, word, size_filter)) {
          if (ind_word< min_size && ind_word != 0) {
            min_size = ind_word;
          }
          if (ind_word > max_size && ind_word != 0) {
            max_size = ind_word;
          }
        } else {
          for (size_t in = 0;word[in] != '\0'; ++in) {
            output << word[in];
          }
          output << " ";
        }
        ind_word = 0;
      }
      else {
        word[ind_word] = buffer[i];
        ++ind_word;
      }
    }
  }
  size_t med = (max_size + min_size) / 2;
  char* max_word;
  size_t max_size_word = 0;
  for (size_t i = 0; i != size_filter; ++i) {
    if (filters[i].first > max_size_word) {
      max_size_word = filters[i].first;
      max_word = filters[i].second;
    }
  }
  size_t index = 0;
  while (max_word[index] != '\0') {
    std::cout << max_word[index];
    ++index;
  }
  std::cout << " " << med;
}

int main(int argc, char** argv){
    Args args;
    char** filter;
    if (!parse(argc,argv, &args)) {
      return 1;
    }
    std::ifstream filter_file(args.filter_path);
    size_t size_filter = count_words(args.filter_path);
    std::pair<size_t, char*>* filters = new std::pair<size_t, char*>[size_filter];
    input_filter(&filter_file, filters);
    ans(filters, args.input_path, size_filter);
    return 0;
}
