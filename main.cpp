#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_LINE_LENGTH = 1024;
const int MAX_FILTER = 128;
const int MAX_CONTEXT_LENGTH = 128;
const int MAX_WORDS = 10000;

void parseArgs(int argc, char* argv[], char* inputPath, char* filterdb, char* outputPath) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) std::strcpy(inputPath, argv[++i]);
        else if (strcmp(argv[i], "--filterdb") == 0 && i + 1 < argc) std::strcpy(filterdb, argv[++i]);
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) std::strcpy(outputPath, argv[++i]);
    }
}

char** readFilters(const char* filterdb, int& filterCount) {
    std::ifstream file(filterdb);
    if (!file) return nullptr;

    char** filters = new char*[MAX_FILTER];
    filterCount = 0;
    char line[MAX_FILTER];

    while (file.getline(line, MAX_FILTER) && filterCount < MAX_FILTER) {
        filters[filterCount] = new char[strlen(line) + 1];
        std::strcpy(filters[filterCount++], line);
    }
    
    file.close();
    return filters;
}

bool containsFilter(const char* word, char** filters, int filterCount, char*& matchedFilter) {
    for (int i = 0; i < filterCount; ++i) {
        if (strstr(word, filters[i])) {
            matchedFilter = filters[i];
            return true;
        }
    }
    return false;
}

void freeMemory(char** filters, int filterCount) {
    for (int i = 0; i < filterCount; ++i) delete[] filters[i];
    delete[] filters;
}

void processFile(const char* inputPath, const char* outputPath, char** filters, int filterCount) {
    std::ifstream inputFile(inputPath);
    std::ofstream outputFile(outputPath);
    if (!inputFile || !outputFile) return;

    char word[MAX_CONTEXT_LENGTH];
    int filterUsage[MAX_FILTER] = {0};
    int discardedSizes[MAX_WORDS];
    int discardedCount = 0;
    char discardedWords[MAX_WORDS][MAX_CONTEXT_LENGTH];
    int usedFilterIndex[MAX_WORDS];
    int usedFilterCount = 0;

    while (inputFile >> word) {
        char* matchedFilter = nullptr;
        if (containsFilter(word, filters, filterCount, matchedFilter)) {
            int len = strlen(word);
            discardedSizes[discardedCount] = len;
            std::strcpy(discardedWords[discardedCount], word);

            for (int i = 0; i < filterCount; ++i) {
                if (strcmp(filters[i], matchedFilter) == 0) {
                    filterUsage[i]++;
                    usedFilterIndex[discardedCount] = i;
                    break;
                }
            }
            discardedCount++;
        } else {
            outputFile << word << " ";
        }
    }
    inputFile.close();
    outputFile.close();

    int maxUsage = 0, mostUsedFilterIndex = -1;
    for (int i = 0; i < filterCount; ++i) {
        if (filterUsage[i] > maxUsage) {
            maxUsage = filterUsage[i];
            mostUsedFilterIndex = i;
        }
    }

    if (mostUsedFilterIndex == -1) return;

    int relevantSizes[MAX_WORDS];
    int relevantCount = 0;
    for (int i = 0; i < discardedCount; ++i) {
        if (usedFilterIndex[i] == mostUsedFilterIndex) {
            relevantSizes[relevantCount++] = discardedSizes[i];
        }
    }

    if (relevantCount > 0) {
        for (int i = 0; i < relevantCount - 1; ++i)
            for (int j = i + 1; j < relevantCount; ++j)
                if (relevantSizes[i] > relevantSizes[j])
                    std::swap(relevantSizes[i], relevantSizes[j]);

        int median = (relevantCount % 2 == 0) ? 
                     (relevantSizes[relevantCount / 2 - 1] + relevantSizes[relevantCount / 2]) / 2 
                     : relevantSizes[relevantCount / 2];

        std::cout << filters[mostUsedFilterIndex] << " " << median << std::endl;
    }
}

int main(int argc, char* argv[]) {
    char inputPath[MAX_LINE_LENGTH] = "";
    char filterdb[MAX_LINE_LENGTH] = "";
    char outputPath[MAX_LINE_LENGTH] = "";

    parseArgs(argc, argv, inputPath, filterdb, outputPath);

    int filterCount = 0;
    char** filters = readFilters(filterdb, filterCount);

    if (filters) {
        processFile(inputPath, outputPath, filters, filterCount);
        freeMemory(filters, filterCount);
    }

    return 0;
}
