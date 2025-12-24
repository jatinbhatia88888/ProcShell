#include "shell.h"
#include "parser.h"
#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>

void Shell::run() {
   
    signal(SIGINT, SIG_IGN);

    setenv("PATH", "/usr/bin:/bin:/mingw64/bin", 1);

    std::string line;

    while (true) {
        std::cout << "ProcShell> ";

        
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }

        if (line.empty())
            continue;

        ParsedLine pl = parseLine(line);

        if (!pl.pipeline.empty()) {
            auto &args = pl.pipeline[0].args;

           
            if (args[0] == "exit")
                break;

           
            if (args[0] == "cd") {

              
                if (args.size() == 1) {
                    const char *home = getenv("HOME");
                    if (!home) home = "/";
                    if (chdir(home) != 0)
                        perror("cd");
                }
               
                else if (chdir(args[1].c_str()) != 0) {
                    perror("cd");
                }

                continue;
            }
        }

        executeParsed(pl);
    }
}
