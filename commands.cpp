//		commands.c
//********************************************
#include "commands.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char last_pwd[MAX_LINE_SIZE] = {0};

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(std::map<int, Job> jobs, char* lineSize, char* cmdString, std::list<std::string> command_history)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		bool is_print = false;
		if(std::string(args[1]) == "-") 
		{
			strcpy(args[1],last_pwd);
			is_print = true;
		}

		getcwd(last_pwd,MAX_LINE_SIZE);

		if(chdir(args[1]) < 0)
		{
			if(errno == ENOENT)
			{
				std::cout << "smash error: > \"" << args[1]
				 << "\" –No such file or directory" << std::endl;
			}
		}
		else if(is_print)
		{
			std::cout << getcwd(pwd,MAX_LINE_SIZE) << std::endl;
		}
		
	} 
	else if (!strcmp(cmd, "pwd")) 
	{
		if(getcwd(pwd,MAX_LINE_SIZE) != NULL)
		{
			std::cout << pwd << std::endl;
		}
	}
	else if (!strcmp(cmd, "history"))
	{
 		for(auto iter_command = command_history.begin(); iter_command !=command_history.end(); iter_command++ )
			std::cout << *iter_command << std::endl;
	}
	else if (!strcmp(cmd, "jobs")) 
	{
		int i = 1;
 		for(auto iter_jobs = jobs.begin(); iter_jobs != jobs.end(); iter_jobs++)
		{
			std::cout << "[" << i << "] " << iter_jobs->second.name 
				<< " : " << iter_jobs->second.pid << " " << iter_jobs->second.getTime() 
				<< " secs";

			if(iter_jobs->second.isStopped)
			{
				std::cout << " (Stopped)";
			}
			std::cout << std::endl;

			i++;
		}
	}
	else if (!strcmp(cmd, "showpid")) 
	{
		std::cout << "smash pid is "<< getpid() << std::endl;
	}
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	else if (!strcmp(cmd, "cp")) 
	{
		char buf;
		int sourcefile = open(args[1], O_RDONLY);
		int destfile, n;
        if(sourcefile == -1)
        {
            perror("SOURCE FILE ERROR");
            exit(0);
        }
        else
        {
            destfile = open(args[2],O_WRONLY | O_CREAT , 0641);
            if(destfile == -1)
            {
                perror("DESTINATION FILE ERROR");
                exit(0);
            }
            else
            {
                while((n=read(sourcefile,&buf,1)) != 0)
                {
                    write( destfile, &buf, 1 );
                }
                close(sourcefile);
                close(destfile);
            }
		}
	} 
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    switch(pID = fork()) 
	{
    		case -1: 
					
					perror("smash error: > ");
					break;
        	case 0 :
               		setpgrp();
					if(execv(cmdString, &args[1]) < 0)
					{
						perror("smash error: > ");
						exit(1);
					}
					break;

			default:
                	waitpid(pID, NULL, 0);
					break;
	}
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, std::map<int, Job>& jobs)
{

	char* cmd;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		int num_arg = 0;
		//bool illegal_cmd = false; // illegal command
		cmd = strtok(lineSize, delimiters);
		if (cmd == NULL)
			return 0; 
		args[0] = cmd;
		for (int i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
		}

		int pID;
   		switch(pID = fork()) 
		{
    		case -1: 
					perror("smash error: > ");
					break;
        	case 0 :
					if(execv(lineSize, &args[1]) < 0)
					{
						perror("smash error: > ");
						exit(1);
					}
			default:
					Job new_job = Job(lineSize, pID);
					jobs[pID] = new_job;
		}

		return 0;
	}

	return -1;
}

