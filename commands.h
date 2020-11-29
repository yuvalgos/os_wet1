#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string>
#include <list>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include "Job.h"

#define MAX_LINE_SIZE 1000
#define MAX_ARG 255

extern Job fg_job;

int BgCmd(char* lineSize, std::map<int, Job>& jobs);
int ExeCmd(std::map<int, Job>& jobs, char* lineSize, char* cmdString, std::list<std::string> command_history);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

#endif

