#pragma once

#include <cstdint>
#include <cstddef>

namespace parsing {

enum class EArgs {
    INPUT = 0,
    FILTERDB = 1
};

inline const size_t kArgsNum = 2;
inline const char* kArgsNames[kArgsNum] = {"--input", "--filterdb"}; 

struct Args {
    char* input;
    char* filterdb;
};

bool Parse(Args& args, int32_t argc, char** argv);

} // namespace parsing