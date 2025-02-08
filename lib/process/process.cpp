#include "process.hpp"

#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <parsing.hpp>

using namespace process;

int32_t process::Process(int32_t argc, char** argv) {
    parsing::Args args{};
    bool state = parsing::Parse(args, argc, argv);
    if (!state) {
        return EXIT_FAILURE;
    }

    if (args.input == nullptr || args.filterdb == nullptr) {
        std::cout << "Args are incomplete!" << std::endl;
        return EXIT_FAILURE;
    }

    FILE* words_file = fopen(args.input, "r");

    if (!words_file) {
        std::cout << "Can't open the words file " << words_file << std::endl;
        return EXIT_FAILURE;
    }

    FILE* filters_file = fopen(args.filterdb, "r");

    if (!filters_file) {
        std::cout << "Can't open filters file " << filters_file << std::endl;
        return EXIT_FAILURE;
    }

    // todo

    fclose(words_file);
    fclose(filters_file);

    return EXIT_SUCCESS;
}