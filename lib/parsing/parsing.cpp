#include "parsing.hpp"

#include <cstring>
#include <iostream>

using namespace parsing;

bool CmpArg(Args& args, int32_t argc, char** argv, int32_t& i) {
    bool is_found = false;
    int32_t arg_id;

    for (arg_id = 0; arg_id < kArgsNum; ++arg_id) {
        if (strcmp(argv[i], kArgsNames[arg_id]) == 0) {
            is_found = true;
            break;
        }
    }

    if (is_found && i + 1 >= argc) {
        std::cout << "No value for arg: " << argv[i] << std::endl;
        return false;
    } else if (!is_found) {
        std::cout << "No such argument: " << argv[i] << std::endl;
        return false;
    }

    if (static_cast<EArgs>(arg_id) == EArgs::INPUT) {
        args.input = argv[i + 1];
    } else if (static_cast<EArgs>(arg_id) == EArgs::FILTERDB) {
        args.filterdb = argv[i + 1];
    }

    ++i;

    return true;
}

bool parsing::Parse(Args& args, int32_t argc, char** argv) {
    for (int32_t i = 1; i < argc; ++i) {
        if (!CmpArg(args, argc, argv, i)) {
            return false;
        }
    }

    return true;
}