#include <iostream>

#include <parsing.hpp>

using namespace parsing;
int main(int32_t argc, char** argv){
    Args args{};
    Parse(args, argc, argv);

    std::cout << args.input << std::endl;
    std::cout << args.filterdb << std::endl;
}
