#include <iostream>
#include <fstream>

const unsigned char context_len = 129;

void CopyArr(char* line, char place[context_len]){
    for(int i = 0; line[i] != '\n'; ++i){
        place[i] = line[i];
    }
}

bool EqualArray(char* line_from_input, char* line_from_filter){
    while (*line_from_input == *line_from_filter){
        ++line_from_input;
        ++line_from_filter;
    }
    return *line_from_input == ' ' || *line_from_input == '\n' || *line_from_input == EOF;
}

class List{
public:
    char value[context_len]{'\0'};
    char counter = 0;
    List* next = nullptr;
};

bool CheckInput(char* string){
    for(int i = 0; i < 7; ++i){
        if(string[i] != "--input"[i]){
            return false;
        }
    }

    return true;
}

bool CheckFDB(char* string){
    for(int i = 0; i < 10; ++i){
        if(string[i] != "--filterdb"[i]){
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv){
    char* input = nullptr;
    char* filterdb = nullptr;
    for(int i = 1; i < argc; ++i){
        if(CheckFDB(argv[i])){
            if(argv[i][10] == '='){
                filterdb = argv[i]+ 11;
            }else if(argv[i][10] == '\0'){
                filterdb = argv[++i];
            }
        } else if(CheckInput(argv[i])){
            if(argv[i][7] == '='){
                input = argv[i]+ 8;
            }else if(argv[i][7] == '\0'){
                input = argv[++i];
            }
        }
    }
    std::cout << "input = " << input << std::endl << "filtredb = "<< filterdb << std::endl;

    FILE* i_file = fopen(input, "r");
    FILE* fdb_file = fopen(filterdb, "r");

    List filters;
    char context[context_len - 1];
    fgets(context, context_len - 1, fdb_file);
    CopyArr(context, filters.value);
    List* last = &filters;

    while (fgets(context, context_len - 1, fdb_file)){
        last->next = new List;
        last = last->next;
        CopyArr(context, last->value);
    }

    //all filters are in one-connected list
    
    FILE* word_counter = fopen(input, "r");
    unsigned long long wcount = 1ull;
    while (fgetc(word_counter) != EOF){
        ++wcount;
    }
    fclose(word_counter);

    char* file = new char[wcount]{'\0'}; // all_file_in_one_array
    
    char c = fgetc(i_file);
    for(int i = 0; c != EOF; ++i){
        file[i] = c;
        c = fgetc(i_file);
    }

    //check each string for filter and write
    //я не успел =(
    fclose(i_file);
    std::ofstream output;
    output.open(input);

    return 0;
}