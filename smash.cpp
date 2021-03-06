/*	smash.cpp
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"

#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <map>

#include "Job.h"

#define MAX_LINE_SIZE 1000
#define MAXARGS 255

std::map<int, Job> jobs;
Job fg_job;

char* L_Fg_Cmd;
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   
	std::list<std::string> command_history;

	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here

	struct sigaction act_stp;
	struct sigaction act_int;

	act_stp.sa_handler = &catch_stp;
	act_int.sa_handler = &catch_int;

	sigaction(SIGTSTP, &act_stp, NULL);
	sigaction(SIGINT, &act_int, NULL);

	/************************************/

	/************************************/
	// Init globals 
	
	/************************************/
	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';

	while (1)
	{
		std::cout << "smash > ";
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';

		if(cmdString[0] == '\0') continue;

		// background command	
		if(!BgCmd(lineSize, jobs)) continue; 

		// built in commands
		ExeCmd(jobs, lineSize, cmdString, command_history);
		
		//insert to command history:
		std::string cmd(cmdString);
		command_history.push_back(cmd);
		if(command_history.size() > 50)
		{
			command_history.pop_front();
		}

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

