#include <cstring>
#include <iostream>

template <typename T>
class MyVector {
   public:
    MyVector(uint64_t size = 2) : size_(size), data_(new T[size]) {}
    ~MyVector() { delete[] data_; }

    void push_back(T value) {
        if (capacity_ < size_) {
            data_[capacity_++] = value;
        } else {
            size_ *= 2;
            T* tmp = new T[size_];
            for (uint64_t i = 0; i < size_ / 2; ++i) {
                tmp[i] = data_[i];
            }
            tmp[size_ / 2] = value;
            std::swap(data_, tmp);
            delete tmp;
        }
    }

    uint64_t size() { return capacity_; }

    T& operator[](uint64_t ind) {
        if (ind < size_) {
            return data_[ind];
        }
    }

   private:
    uint64_t size_ = 2;
    uint64_t capacity_ = 0;
    T* data_;
};

struct Pattern {
    Pattern(char* v, uint64_t size) : value(new char[size]) { strcpy(value, v); }
    Pattern() = default;
    char* value;
    uint64_t count = 0;
    MyVector<uint64_t> lenghts{};
};

const char* kInput = "--input";
const char* kFilterdb = "--filterdb";

struct Arguments {
    char* input = nullptr;
    char* filterdb = nullptr;
    ~Arguments() {
        delete[] input;
        delete[] filterdb;
    }
};

bool DefineArgs(int argc, char** argv, Arguments& args) {
    if (argc != 5) {
        return false;
    }

    for (int i = 1; i < 4; ++i) {
        if (strcmp(kInput, argv[i]) == 0 && (args.input == nullptr)) {
            args.input = new char[strlen(argv[i + 1])]{};
            strcpy(args.input, argv[i + 1]);
        }
        if (strcmp(kFilterdb, argv[i]) == 0) {
            args.filterdb = new char[strlen(argv[i + 1])]{};
            strcpy(args.filterdb, argv[i + 1]);
        }
    }
    if (args.input == nullptr || args.filterdb == nullptr) {
        return false;
    }
    return true;
}

bool FillPattern(Arguments& args, MyVector<Pattern>& patterns) {  // заполнение паттернов
    FILE* in = fopen(args.input, "r");
    if (in == nullptr) {
        return false;
    }
    char buf[1024]{};

    while (fgets(buf, sizeof(buf), in) != NULL) {
        uint64_t size = 0;
        for (int i = 0; i < 1024 && buf[i] != 0; ++i) {
            ++size;
        }
        char* value = new char[size];
        strcpy(value, buf);
        patterns.push_back({value, size});
        delete value;

        for (int i = 0; i < size; ++i) {
            buf[i] = 0;
        }
    }
}

int ReadWord(char* buf, int buf_size, char* word, int word_size, int ind) {
    int size = 0;
    for (int i = ind; i < buf_size && buf[i] != 0 && i < word_size; ++i) {
        word[i] = buf[i];
        ++size;
    }
    return size;
}

bool CleanWord(char* word, int word_size) {
    for (int i = 0; i < word_size; ++i) {
        word[i] = 0;
    }
}

bool CheckFilter(char* word, int size, MyVector<Pattern>& patterns) {
    Pattern p;
    bool find = true;
    ;
    for (int i = 0; i < patterns.size(); ++i) {  // считывание паттернов
        p = patterns[i];

        for (int j = 0; j < size - (strlen(p.value)); ++j) {  // позиция для поиска подстрок
            find = true;
            for (int k = 0; k < strlen(p.value); ++k) {  // сравнение символов j + k
                if (word[j + k] != p.value[k]) {
                    find = false;
                }
            }
            if (find) { //  нашли подстроку
                ++patterns[i].count; // увеличение счетчика
                patterns[i].lenghts.push_back(size);
            }
        }
    }
}

bool ReadFile(Arguments& args, MyVector<Pattern>& patterns) {  //  обработка файла с готовыми паттернами

    FILE* in = fopen(args.input, "r");

    // имя выходного файла
    char* name = new char[strlen(args.input) + 1];
    strcpy(name, args.input);
    name[strlen(args.input)] = '1';
    FILE* out = fopen(name, "w");


    FILE* in = fopen(args.input, "r");
    if (in == nullptr) {
        return false;
    }
    char buf[1024]{};
    char word[128]{};

    int size;
    int ind = 0;

    while (fgets(buf, sizeof(buf), in) != NULL) {
        size = ReadWord(buf, 1024, word, 128, ind); // (тут шляпа, которая считывает только одно слово (не успела пофиксить))

        if (!CheckFilter(word, size, patterns)){
            fwrite(word, sizeof(word), size, out);            
        }
    }

}


Pattern findMax( MyVector<Pattern>& patterns) {
    Pattern max_p = patterns[0];
    for (int i = 1; i < patterns.size(); ++i) {
        if (patterns[i].count > max_p.count) {
            max_p = patterns[i];
        }
    }
    return max_p;

}


bool StartApp(int argc, char** argv) {
    Arguments args{};
    if (!DefineArgs(argc, argv, args)) {
        return false;
    }

    // printf("%s, %s", args.input, args.filterdb);

    MyVector<Pattern> patterns;
    if (!FillPattern(args, patterns)) {
        return false;
    }

    if (!ReadFile(args, patterns)) {
        return false;
    }

    Pattern max_p = findMax(patterns);
    std::cout << max_p.count << std::endl;

    // поиск медианы



}

int main(int argc, char** argv) { std::cout << StartApp(argc, argv) << std::endl; }
