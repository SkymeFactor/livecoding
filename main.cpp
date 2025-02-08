#include <iostream>
#include <fstream>

struct Settings {
    char* inputFile;
    char* filterFile;
    Settings() {
        inputFile = nullptr;
        filterFile = nullptr;
    }
};

struct Node {
    Node* next;
    Node* prev;
    char* line;

    Node() {
        next = nullptr;
        prev = nullptr;
        line = nullptr;
    }

    Node(const char* line_) {
        next = nullptr;
        prev = nullptr;
        line = new char[1024];
        for (int i = 0; i < strlen(line_); i++) {
            line[i] = line_[i];
        }
    }
};

struct StrList {
    Node* head;
    Node* tail;

    StrList() {
        head = nullptr;
        tail = nullptr;
    }

    void insert(char* line) {
        Node* newNode = new Node(line);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail -> next = newNode;
            newNode -> prev = tail;
            tail = newNode;
        }
    }
    void print() {
        Node* cur = head;
        while (cur != nullptr) {
            std::cout << cur -> line << std::endl;
            cur = cur -> next;
        }
    }
};

Settings ArguementParser(int argSize, char** args) {
    Settings res;
    if (argSize != 5) {
        std::cerr << "Кол-во аргументов должно быть 5" << std::endl;
        return Settings();
    }
    for (int i = 1; i < argSize - 1; i++) {
        char* curArg = args[i];
        if (strcmp(curArg, "--input") == 0) {
            res.inputFile = args[i + 1];
            i++;
        } else if (strcmp(curArg, "--filterdb") == 0) {
            res.filterFile = args[i + 1];
            i++; 
        } else {
            std::cerr << "Неизвестный аргумент: " << curArg << std::endl;
            return Settings();
        }
    }
    return res;
}

int main(int argSize, char** args){
    Settings settings = ArguementParser(argSize, args);
    if (settings.inputFile == nullptr || settings.filterFile == nullptr) {
        std::cerr << "Входной или файл фильтра не указан" << std::endl;
        return 0;
    }
    std::ifstream in(settings.inputFile);
    if (in.fail()) {
        std::cerr << "Ошибка открытия входного файла" << std::endl;
    }
    StrList list;
    char buff[1024];
    while (in.getline(buff, 1024)) {
        list.insert(buff);
    }
    in.close();

    std::ifstream fil("fil.txt");
    if (fil.fail()) {
        std::cerr << "Ошибка открытия файла фильтра" << std::endl;
    }

    StrList filters;
    while (fil.getline(buff, 1024)) {
        filters.insert(buff);
    }
    fil.close();

    StrList ansList;
    Node* curLine = list.head;
    while (curLine != nullptr) {
        Node* curFilter = filters.head;
        char* line = curLine -> line;
        while (curFilter != nullptr) {
            bool isSkip = false;
            char word[128];
            int k = 0;
            for (int i = 0; i < strlen(line); i++) {
                if (line[i] == ' ' || line[i] == '\n') {
                    k = 0;
                    i++;
                    if (!isSkip) {
                        std::cout << word << ' ';
                        memset(word, 0, sizeof(word));
                    }
                    continue;
                }
                word[k] = line[i];
                k++;
                if (strncmp(line, curFilter -> line, strlen(curFilter -> line)) == 0) {
                   isSkip = true; 
                }
            }

            curFilter = curFilter -> next;
        }
        curLine = curLine -> next;
    }
}
