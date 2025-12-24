#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Command {
    std::vector<std::string> args;
    std::string inputFile;
    std::string outputFile;
};

struct ParsedLine {
    std::vector<Command> pipeline;
    bool background = false;
};

ParsedLine parseLine(const std::string& line);

#endif
