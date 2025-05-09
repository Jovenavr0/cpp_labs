#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "function.h"

void ParseArguments(int argc, char** argv, char** filename, char* delimiter, bool* input_count_lines, long long* user_count_lines,
                    bool* reverse) {

    char mas_for_delimiter[3];
    char mas_for_digit_in_lines[21];
    bool is_correct_open = false;
    const int COUT_SYMB_DELIMIER_LEHGTH_1 = 13;
    const int COUT_SYMB_DELIMIER_LEHGTH_2 = 14;
    std::ifstream file;

    for (int i = 1; i < argc; ++i){

        if (!strcmp(argv[i], "-l")){
            *input_count_lines = true;
            try {
                *user_count_lines = std::stoll(argv[i + 1]);
                ++i;
                if (*user_count_lines < 0) {
                    std::cerr <<  "The number of rows cannot be negative\n";
                    exit(EXIT_FAILURE);
                }
            }
            catch (std::invalid_argument) {
                std::cerr << argv[i + 1] << ": is not the number of rows" << '\n' << "Input data error\n";
                exit(EXIT_FAILURE);
            }
            catch (std::logic_error){
                std::cerr << "Please enter the number of lines\n";
                exit(EXIT_FAILURE);
            }
            continue;
        }

        else if (!strncmp("--lines=", argv[i], 8)){
            *input_count_lines = true;
            int len_argument = strlen(argv[i]);
            try {
                for (int j = 0; j < (len_argument - 8); ++j){
                    mas_for_digit_in_lines[j] = argv[i][8 + j];
                }
                *user_count_lines = std::stoll(mas_for_digit_in_lines);
            }
            catch (std::invalid_argument) {
                std::cerr << "Input data error" << '\n';
                for (int j = 0; j < (len_argument - 8); ++j){
                    std::cerr << mas_for_digit_in_lines[j];
                }
                std::cerr <<  ": is not the number of rows\n";
                exit(EXIT_FAILURE);
            }
            continue;
        }

        else if (!strcmp(argv[i], "-t") || (!strcmp(argv[i], "--tail"))){
            *reverse = true;
            continue;
        }

        else if (!strcmp(argv[i], "-d")){
            if ((i + 1) == argc) {
                std::cerr << "Please enter your delimiter\n";
                exit(EXIT_FAILURE);
            }
            int len_delimiter = strlen(argv[i + 1]);
            if (len_delimiter > 2 or len_delimiter == 0){
                std::cerr << "Please enter correct delimiter\n";
                exit(EXIT_FAILURE);
            }
            *delimiter = CreateDelimiter(argv[++i]);
            continue;
        }

        else if (!strncmp("--delimiter=", argv[i], 12)){
            if (strlen(argv[i]) == COUT_SYMB_DELIMIER_LEHGTH_1){
                mas_for_delimiter[0] = argv[i][COUT_SYMB_DELIMIER_LEHGTH_1 - 1];
                mas_for_delimiter[1] = '\0';
            }
            else if (strlen(argv[i]) == COUT_SYMB_DELIMIER_LEHGTH_2){
                mas_for_delimiter[0] = argv[i][COUT_SYMB_DELIMIER_LEHGTH_2 - 2];
                mas_for_delimiter[1]= argv[i][COUT_SYMB_DELIMIER_LEHGTH_2 - 1];
                mas_for_delimiter[2] = '\0';
            }
            else{
                std::cerr << "Please enter correct delimiter\n";
                exit(EXIT_FAILURE);
            }
            *delimiter = CreateDelimiter(mas_for_delimiter);
            continue;
        }

        else if ((argv[i][0] != '-') && (!is_correct_open)){
            *filename = argv[i];
            file.open(*filename);
            if(file.is_open()){
                is_correct_open = true;
            }
            else std::cerr << argv[i] << " - this command does not exist" << '\n';
            if((i + 1) == argc){
                if(!is_correct_open){
                    std::cerr << "FILENAME: " << &**filename << '\n';
                    std::cerr << "File opening error!";
                    exit(EXIT_FAILURE);
                }
            }
            continue;
        }
        else{
            std::cerr << argv[i];
            std::cerr << " - this command does not exist\n";
            exit(EXIT_FAILURE);
        }
    }
}

void PrintFile(char** filename, char* delimiter, long long* user_count_lines, long long* start_lines, bool* is_reverse,
               long long* all_count_lines){
    std::ifstream file(*filename);
    char symbol;
    long long now_lines = 1;
    if(*is_reverse){
        *start_lines = *all_count_lines - *user_count_lines + 1;
        if (*start_lines > *all_count_lines) exit(EXIT_FAILURE);
        if (*start_lines <= 0) *start_lines = 1;
    }
    while (now_lines != *start_lines){
        file.get(symbol);
        if (symbol == *delimiter) ++now_lines;
    }
    while ((file.get(symbol) && (*user_count_lines != 0))) {
        if (symbol != *delimiter)
            std::cout << symbol;
        else{
            std::cout << *delimiter;
            *user_count_lines = *user_count_lines - 1;
        }
    }
}

char CreateDelimiter(char* user_delimiter){
    char delimiter;
    if (user_delimiter[2] == '\0') {
        if (user_delimiter[1] == '\0') delimiter = user_delimiter[0];
        else if (user_delimiter[0] == '\\') {
            if (user_delimiter[1] == 't') delimiter = '\t';
            else if (user_delimiter[1] == 'n') delimiter = '\n';
            else if (user_delimiter[1] == 'r') delimiter = '\r';
            else if (user_delimiter[1] == 'v') delimiter = '\v';
            else if (user_delimiter[1] == 'b') delimiter = '\b';
            else if (user_delimiter[1] == 'f') delimiter = '\f';
            else if (user_delimiter[1] == '\'') delimiter = '\'';
            else if (user_delimiter[1] == '\"') delimiter = '\"';
            else if (user_delimiter[1] == '\\') delimiter = '\\';
            else {
                std::cerr << '\'' << user_delimiter << '\'' << " is not a valid delimiter" << '\n';
                std::cerr << "Please enter correct delimiter\n";
                exit(EXIT_FAILURE);
            }
        }
        else{
            std::cerr << '\'' << user_delimiter << '\'' << " is not a valid delimiter" << '\n';
            std::cerr << "Please enter correct delimiter\n";
            exit(EXIT_FAILURE);
        }
    }
    else if (user_delimiter[1] == '\0'){
        delimiter = user_delimiter[0];
    }
    else {
        std::cerr <<  "Please enter correct delimiter\n";
        exit(EXIT_FAILURE);
    }
    return delimiter;
}

long long CalcAllCountLines(char** filename, char* delimiter){
    long long all_count_lines = 1;
    char symbol;
    std::ifstream file(*filename);
    while (file.get(symbol)) {
        if (symbol == *delimiter){
            ++all_count_lines;
        }
    }
    return all_count_lines;
}

int main(int argc, char** argv) {

    long long user_count_lines = 0;
    long long start_lines = 1;
    long long all_count_lines;
    bool is_reverse = false;
    bool have_input_count_lines = false;
    char* filename;
    char delimiter = '\n';

    ParseArguments(argc, argv, &filename, &delimiter, &have_input_count_lines, &user_count_lines, &is_reverse);

    all_count_lines = CalcAllCountLines(&filename, &delimiter);

    if (!have_input_count_lines) {
        user_count_lines = all_count_lines;
    }

    PrintFile(&filename, &delimiter, &user_count_lines, &start_lines, &is_reverse, &all_count_lines);

    return 0;
}