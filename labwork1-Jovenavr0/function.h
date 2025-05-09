#pragma once

void ParseArguments(int argc, char** argv, char** filename, char* delimiter, bool* input_count_lines, long long* user_count_lines,
                    bool* reverse);
char CreateDelimiter(char* user_delimiter);
long long CalcAllCountLines(char** filename, char* delimiter);
void PrintFile(char** filename, char* delimiter, long long* user_count_lines, long long* start_lines, bool* is_reverse,
               long long* all_count_lines);
