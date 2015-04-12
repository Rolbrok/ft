#include <iostream>

std::string parseArguments(int argc, char* argv[])
{
    bool varParsed = false; 
    int nextArg = 0;

    std::string code = "";

    int i; 
    for (i = 1; i < argc; i++) {
        char* arg = argv[i];

        if (arg[0] != '-' && i == nextArg) {
            code += "*"+std::string(arg);
        } else if (i == nextArg) {
            code += "*true";
        } else if (arg[0] != '-') {
            code += "|special*"+std::string(arg);
        } 

        if (arg[0] == '-') {
            if (i != 1) code += "|";
            code += arg[1];
            nextArg = i+1;
        }
    }
    if (i == nextArg) {
        code += "*true";
    }

    return code;
}
