#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cctype>
#include <fstream>

const size_t kMaxContextLenght = 129;
const size_t kCapacityFactor = 2;

class Array {
    size_t size{};
    size_t capacity{1};
    uint32_t* array{};
private:
    void DoReallocation() {
        capacity *= kCapacityFactor;
        uint32_t* new_array = new uint32_t[capacity];
        for (size_t i = 0; i<size; i++) {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
    }
public:
    Array() {
        array = new uint32_t[capacity];
    }
    ~Array() {
        delete[] array;
    }
    Array(const Array& arr) = delete;
    Array& operator=(const Array& arr) = delete;
public:
    size_t GetSize() const {
        return size;
    }
    void PushBack(uint32_t value) {
        if (size == capacity) {
            DoReallocation();
        }
        array[size] = value;
        size++;
    }
    void Sort() {
        std::sort(array, array+size);
    }
    uint32_t operator[](size_t idx) {
        return array[idx];
    }
};

class Pattern {
    char pattern[kMaxContextLenght];
    Array contexts_lenght;
public:
    Pattern(char* string) {
        strcpy_s(pattern, kMaxContextLenght, string);
    }
    bool IsEqual(char* string) {
        for (size_t i = 0; i + strlen(pattern) <= strlen(string); i++) {
            bool flag = true;
            for (int j = 0; j<strlen(pattern); j++) {
                char lhs = tolower(pattern[j]);
                char rhs = tolower(pattern[i+j]);
                flag &= lhs == rhs;
            }
            if (flag) {
                contexts_lenght.PushBack(strlen(string));
                return true;
            }
        }
        return false;
    }
    const char* GetString() const {
        return pattern;
    }
    uint32_t GetMedian() {
        contexts_lenght.Sort();
        return contexts_lenght[contexts_lenght.GetSize() / 2];
    }
    uint32_t GetSizeArr() const {
        return contexts_lenght.GetSize();
    }
};

struct FilterNode {
    Pattern pattern;
    FilterNode* next{};
    FilterNode(char* string) : pattern(string) {}
};

class Filter {
    FilterNode* top{};
public:
    void AddPattern(char* string) {
        FilterNode* new_top = new FilterNode(string);
        new_top->next = top;
        top = new_top;
    }
    bool IsNeedDelete(char* string) {
        bool result = false;
        FilterNode* it = top;
        while(it) {
            result |= it->pattern.IsEqual(string);
            it = it->next;
        }
        return result;
    }
    void PrintPopular() const {
        FilterNode* popular = top;
        FilterNode* tmp = top;
        while(tmp) {
            if (popular == nullptr || popular->pattern.GetSizeArr() < tmp->pattern.GetSizeArr()) {
                popular = tmp;
            }
            tmp = tmp->next;
        }
        std::cout << popular->pattern.GetString() << " " << popular->pattern.GetMedian() << std::endl;
    }
    ~Filter() {
        while(top) {
            FilterNode* tmp = top;
            top = top->next;
            delete tmp;
        }
    }
};


struct Context {
    char context[kMaxContextLenght];
    Context* next{};
    Context(char* string) {
        strcpy_s(context, kMaxContextLenght, string);
    }
};

class Text {
    Context* head{};
    Context* tail{};
public:
    void AddContext(char* string) {
        Context* tmp = new Context(string);
        if (!tail) {
            tail = tmp;
        } else {
            tail->next = tmp;
            tail = tail->next;
        }

        if (!head) {
            head = tail;
        }
    }
    void Print(std::ostream& output) {
        Context* tmp = head;
        while(tmp) {
            output << tmp->context << " ";
            tmp = tmp->next;
        }
    }
};


struct Args {
    char* text_file{};
    char* filter_file{};
};

void Parse(Args& args, int argc, char** argv) {
    for (int i = 1; i<argc; i++) {
        if (strcmp("--input", argv[i]) == 0) {
            i++;
            args.text_file = new char[strlen(argv[i])+1];
            strcpy_s(args.text_file, strlen(argv[i])+1, argv[i]);
        } else if (strcmp("--filterdb", argv[i]) == 0) {
            i++;
            args.filter_file = new char[strlen(argv[i])+1];
            strcpy_s(args.filter_file, strlen(argv[i])+1, argv[i]);
        }
    }
}

int main(int argc, char** argv){
    Args args;
    Parse(args, argc, argv);

    Filter filter;
    char buf[kMaxContextLenght];
    std::ifstream input_filter(args.filter_file);
    while(input_filter >> buf) {
        filter.AddPattern(buf);
    }
    input_filter.close();

    Text text;
    std::ifstream input_txt(args.text_file);
    while (input_txt >> buf) {
        if (!filter.IsNeedDelete(buf)) {
            text.AddContext(buf);
        }
    }
    input_txt.close();

    std::ofstream output(args.text_file);
    text.Print(output);

    filter.PrintPopular();
}
