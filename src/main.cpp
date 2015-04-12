#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

void usage();

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

void help()
{
    usage();
    std::cout << "Options:" << std::endl;
    std::cout << "\t-c:\t\tCount characters." << std::endl;
    std::cout << "\t-w:\t\tCount words." << std::endl;
    std::cout << "\t-l:\t\tCount lines. (same as -d '\\n' -w)" << std::endl;
    std::cout << "\t-z:\t\tDisable multicharacters." << std::endl;
    std::cout << "\t-d (delimiter): Changes the delimiter (default=' ')." << std::endl;
    std::cout << "\t-f (LIST): \tDisplays every word at positions in the list." << std::endl;
    std::cout << "\t-n (LIST): \tSelect only the lines in the list.\n\t\t\t\t(same as 'ft -d '\\n' -f LIST')" << std::endl;
    std::cout << "LIST -> can be 1,2,3 or 1..3" << std::endl;
    exit(EXIT_FAILURE);
}

struct Arguments
{
    bool                count_chars =       false;
    bool                count_words =       false;
    bool                multi_chars =       false;
     
    std::vector<int>    pos_word;   
    std::vector<int>    pos_line;

    std::vector<std::string> filename;
    std::string         code =              ""; 

    char                delimiter =         ' ';
};

void updateArgs(Arguments& m_arg)
{
    std::stringstream first(m_arg.code);
    std::string currArg;

    while (std::getline(first, currArg, '|')) {        
        std::stringstream ss(currArg);
        std::string curr;
        std::getline(ss, curr, '*');
        if (curr == "special") {
            std::getline(ss, curr, '*');
            m_arg.filename.push_back(curr);    
        } else {
            char c = curr[0];
            if (c == 'h') help();
            else if (c == 'c') { 
                std::getline(ss, curr, '*');
                if (curr != "true") m_arg.filename.push_back(curr);
                m_arg.count_chars = true;            
            } else if (c == 'w') { 
                std::getline(ss, curr, '*');
                if (curr != "true") m_arg.filename.push_back(curr);
                m_arg.count_words = true;
            } else if (c == 'l') {
                std::getline(ss, curr, '*');
                if (curr != "true") m_arg.filename.push_back(curr);
                m_arg.count_words = true;
                m_arg.delimiter = '\n';
            } else if (c == 'z') {
                std::getline(ss, curr, '*');
                if (curr != "true") m_arg.filename.push_back(curr);
                m_arg.multi_chars = true;
            } else if (c == 'f' || c == 'n') {
                    std::getline(ss, curr, '*');
                    if (curr.empty()) {
                        std::cout << "The option -f needs an argument." << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    if (std::count(curr.begin(), curr.end(), ',') != 0) {
                        std::stringstream z(curr);
                        std::string currN;
                        while (std::getline(z, currN, ',')) {
                            if (c == 'f') m_arg.pos_word.push_back(atoi(currN.c_str()));
                            else if (c == 'n') m_arg.pos_line.push_back(atoi(currN.c_str()));
                        }
                    } else if (std::count(curr.begin(), curr.end(), '.') != 0) {
                        std::stringstream z(curr);
                        std::string currN;
                        std::getline(z, currN, '.');
                        int first = atoi(currN.c_str());
                        std::getline(z, currN, '.');
                        std::getline(z, currN, '.');
                        int second = atoi(currN.c_str());
                        for (; first != second+1 ; first++) {
                            if (c == 'f') m_arg.pos_word.push_back(first);
                            else if (c == 'n') m_arg.pos_line.push_back(first);
                        }
                    } else if (curr != "true") {
                        if (c == 'f') m_arg.pos_word.push_back(atoi(curr.c_str()));               
                        else if (c == 'n') m_arg.pos_line.push_back(atoi(curr.c_str()));
                    } else {
                        std::cout << "The option -" << c << " must have an argument." << std::endl;
                        exit(EXIT_FAILURE);
                    }
            } else if (c == 'd') {
                    std::getline(ss, curr, '*');
                    if (curr[0] == '\\' && curr.length() == 2) {
                        switch (curr[1])
                        {
                            case 't': m_arg.delimiter = '\t'; break;
                            case 'n': m_arg.delimiter = '\n'; break;
                            case '-': m_arg.delimiter = '-'; break;
                            default:
                                std::cout << "The delimiter must be a character/byte." << std::endl;
                                exit(EXIT_FAILURE);
                        }
                    } else if (curr.length() > 1 || curr.empty()) {
                        std::cout << "The delimiter must be a character/byte." << std::endl;
                        exit(EXIT_FAILURE);
                    } else {
                        m_arg.delimiter = curr[0];
                    }         
            } else {
                std::cout << "Unrecognized option: -" << curr << "\nTry using 'ft --help'." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}
void usage()
{
    std::cout << "Usage: ft OPTIONS... [FILE]..." << std::endl;
    std::cout << "Use 'ft -h' to display the help." << std::endl;
}

size_t CountChars(const char* str)
{
    int count = 0;
    while(str[count] != '\0') {
        count++;
    }
   
    return count;   
}

int CountWords(const char* str, char delim)
{
    int count = 0;
    std::stringstream ss(str);
    std::string a;
    while (std::getline(ss, a, delim)) {
        count++;
    }
    return count;
}

bool isNL(char c)
{
    if (c == '\n') return true;
    else return false;
}

std::string strip_nl(std::string word)
{
    word.erase(std::remove_if(word.begin(), word.end(), &isNL), word.end());
    return word;
}

std::string get_word(const char* str, char delim, std::vector<int> pos, bool multi)
{
    std::stringstream ss(str);
    std::string word = "";
    int count = 1;
    std::string result = ""; 
   
    while (std::getline(ss, word, delim)) {
        if (std::find(pos.begin(), pos.end(), count)!=pos.end()) {
            if (multi && !word.empty()) result += strip_nl(word) + delim;
            else if (!multi) result += strip_nl(word) + delim;
        }
        if (multi && !word.empty()) count++;
        else if (!multi) count++;
    }
    
    return result;
}

void exec_ft(const char* str, Arguments& args);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }
    
    Arguments args;
    args.code = parseArguments(argc, argv);
    updateArgs(args);

    std::string lines = "";

    if (args.pos_line.size() == 0 &&
        args.pos_word.size() == 0 &&
        ! args.count_chars &&
        ! args.count_words) {
        std::cout << "Try using with options." << std::endl;
        usage();
        exit(EXIT_FAILURE);
    }

    if (args.filename.size() == 0) {
        std::string currLine = "";

        while (std::getline(std::cin, currLine)) {
            lines += currLine + '\n';
        }        
        exec_ft(lines.c_str(), args);
    } else {
        std::ifstream file;
        for (size_t i = 0; i < args.filename.size(); i++) {
            std::string currLine = "";
        
            file.open(args.filename[i].c_str());
            if (file.is_open()) {
                while (std::getline(file, currLine)) {
                    lines += currLine + '\n';
                }
        
                if (!lines.empty()) exec_ft(lines.c_str(), args);     
            } else {
                std::cout << "Couldn't open " << args.filename[i] << ": No such file." << std::endl;
            }
            lines = "";
            file.close();
        }
    }

    return 0;
} 

std::string stripstr(std::string word, char delim)
{
    if (word[word.length()-2] == delim) word.erase(word.length()-2);
    return word;
}

void exec_ft(const char* str, Arguments& args)
{
    if (args.pos_line.size() != 0) {
        std::string line = get_word(str, '\n', args.pos_line, args.multi_chars);
        if (args.pos_word.size() != 0 || args.count_chars || args.count_words) {
            args.pos_line.erase(args.pos_line.begin(), args.pos_line.end());
            exec_ft(line.c_str(), args);
        } else {
            std::cout << stripstr(line, args.delimiter);
        }
    } else if (args.pos_word.size() != 0) {
        std::string line = get_word(str, args.delimiter, args.pos_word, args.multi_chars);
        size_t l = CountChars(line.c_str());
        if (args.delimiter != '\n' && l != 0) line += '\n';
        if (args.count_chars || args.count_words) {
            args.pos_word.erase(args.pos_word.begin(), args.pos_word.end());
            exec_ft(line.c_str(), args);
        } else {
            if (l != 0) {
                std::cout << stripstr(line, args.delimiter);
                std::cout << std::endl;
            }
        }
    } else if (args.count_chars && args.count_words) {
        std::cout << "c " << CountChars(str) << std::endl;
        std::cout << "w " << CountWords(str, args.delimiter) << std::endl;
    } else if (args.count_chars) {
        std::cout << CountChars(str) << std::endl;
    } else if (args.count_words) {
        std::cout << CountWords(str, args.delimiter) << std::endl;
    }
}
