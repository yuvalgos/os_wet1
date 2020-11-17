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

#define MAX_LINE_SIZE 80
#define MAX_ARG 20

class Job 
{
public:
	std::string name;
	int pid;
	std::chrono::system_clock::time_point insertion_time;
	bool isStopped;

	Job(std::string name, int pid, bool isStopped = false) :
	 name(name), pid(pid), insertion_time(std::chrono::system_clock::now()),
	 isStopped(isStopped) {}; 

    Job(const Job& j) :
	 name(j.name), pid(j.pid), insertion_time(j.insertion_time),
	 isStopped(j.isStopped) {}; 

    Job(): name(""), pid(-1), insertion_time(std::chrono::system_clock::now()),
    isStopped(false){};

	int getTime()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now() - insertion_time).count();
	}
};

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, std::map<int, Job> jobs);
int ExeCmd(std::map<int, Job> jobs, char* lineSize, char* cmdString, std::list<std::string> command_history);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

