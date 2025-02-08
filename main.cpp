#include <fstream>
#include <iostream>
#include <optional>

struct Args {
    char* input = nullptr;
    char* filterdb = nullptr;
    char* output = "output.txt";

    bool Parse(int, char**);
    bool FilterInput();

    std::optional<bool> NeedFilter(const char*);
};

std::optional<bool> Args::NeedFilter(const char* context) {
    std::ifstream filter_file(filterdb);
    if (!filter_file.is_open()) {
        std::cerr << "Incorrect filterdb path\n";
        return std::nullopt;
    }

    const size_t BUFFER_SIZE = 1025;
    char buffer[BUFFER_SIZE];
    while (filter_file >> buffer) {
        if (strncmp(buffer, context, BUFFER_SIZE) == 0) {
            return true;
        }
    }

    return false;
}

bool Args::FilterInput() {
    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr << "Incorrect input path\n";
        return false;
    }
    std::ofstream output_file(output);
    if (!output_file.is_open()) {
        std::cerr << "Incorrect output path\n";
        return false;
    }

    const size_t BUFFER_SIZE = 1025;
    char buffer[BUFFER_SIZE];
    while (input_file >> buffer) {
        std::optional<bool> result = NeedFilter(buffer);
        if (result == std::nullopt) {
            return false;
        } else if (!result.value()) {
            output_file << buffer << '\n';
        }
    }

    return true;
}

bool Args::Parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--input", 7) == 0) {
            input = argv[++i];
        } else if (strncmp(argv[i], "--filterdb", 10) == 0) {
            filterdb = argv[++i];
        } else if (strncmp(argv[i], "--output", 8) == 0) {
            output = argv[++i];
        } else {
            std::cerr << "Incorrect argument\n";
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv){
    Args args;
    if (!args.Parse(argc, argv)) {
        return 0;
    }

    args.FilterInput();
    return 0;
}