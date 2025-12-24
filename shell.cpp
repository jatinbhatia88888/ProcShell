#include "shell.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>   

void Shell::run() {
    std::string line;

    while (true) {
      
        std::cout << "ProcShell> ";
        std::getline(std::cin, line);

        if (line.empty()) continue;
        if (line == "exit") break;

       
        std::stringstream ss(line);
        std::vector<char*> args;
        std::string token;

        while (ss >> token) {
            args.push_back(strdup(token.c_str()));
        }
        args.push_back(nullptr); 
        pid_t pid = fork();

        if (pid == 0) {
            
            execvp(args[0], args.data());
            perror("exec failed");
            exit(1);
        } else if (pid > 0) {
           
            wait(nullptr);
        } else {
            perror("fork failed");
        }

       
        for (char* arg : args) {
            free(arg);
        }
    }
}
