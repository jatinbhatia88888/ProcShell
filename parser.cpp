#include "parser.h"
#include <sstream>

ParsedLine parseLine(const std::string& line) {
    ParsedLine result;
    std::stringstream ss(line);
    std::string token;

    Command current;

    while (ss >> token) {

       
        if (!token.empty() && token.back() == '\r')
            token.pop_back();

        if (token == "|") {
            result.pipeline.push_back(current);
            current = Command();
        }
        else if (token == "<") {
            ss >> current.inputFile;
        }
        else if (token == ">") {
            ss >> current.outputFile;
        }
        else if (token == "&") {
            result.background = true;
        }
        else {
            current.args.push_back(token);
        }
    }

    if (!current.args.empty())
        result.pipeline.push_back(current);

    return result;
}
