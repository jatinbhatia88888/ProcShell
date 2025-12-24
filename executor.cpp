#include "executor.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <vector>
#include <iostream>

void executeParsed(const ParsedLine& pl) {
    int prevPipe[2] = {-1, -1};
    std::vector<pid_t> children;

    for (size_t i = 0; i < pl.pipeline.size(); i++) {
        int pipefd[2];
        if (i + 1 < pl.pipeline.size())
            pipe(pipefd);

        pid_t pid = fork();

        if (pid == 0) {
            
            signal(SIGINT, SIG_DFL);

           
            if (prevPipe[0] != -1) {
                dup2(prevPipe[0], STDIN_FILENO);
                close(prevPipe[1]);
            }

            
            if (i + 1 < pl.pipeline.size()) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
            }

            
            if (!pl.pipeline[i].inputFile.empty()) {
                int fd = open(pl.pipeline[i].inputFile.c_str(), O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            
            if (!pl.pipeline[i].outputFile.empty()) {
                int fd = open(pl.pipeline[i].outputFile.c_str(),
                              O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            std::vector<char*> argv;
            for (auto &s : pl.pipeline[i].args)
                argv.push_back(const_cast<char*>(s.c_str()));
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp");
            _exit(1);
        }

        children.push_back(pid);

        if (prevPipe[0] != -1) {
            close(prevPipe[0]);
            close(prevPipe[1]);
        }

        if (i + 1 < pl.pipeline.size()) {
            prevPipe[0] = pipefd[0];
            prevPipe[1] = pipefd[1];
            close(pipefd[1]);
        }
    }

    
    if (!pl.background) {
        for (pid_t c : children)
            waitpid(c, nullptr, 0);
    }
}
