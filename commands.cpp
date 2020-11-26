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
		for(auto job_it = jobs.begin(); job_it != jobs.end(); )
		{
			int status;
			pid_t pid = waitpid(job_it->first, &status, WNOHANG);
			if(pid == 0)
			{
				// process not exited and still exists
				job_it++;
			}
			else
			{
				job_it = jobs.erase(job_it);
			}
			
		}
		
		int i = 1;
		for(auto iter_jobs = jobs.begin(); iter_jobs != jobs.end(); iter_jobs++)
		{
			int status;
			pid_t pid = waitpid(iter_jobs->first, &status, WNOHANG | WUNTRACED);
			if(pid > 0)
			{
				bool is_stopped = WIFSTOPPED(status);
				iter_jobs->second.isStopped = is_stopped;
			}
			std::cout << "[" << i << "] " << iter_jobs->second.name 
				<< " : " << iter_jobs->second.pid << " " << iter_jobs->second.getTime() 
				<< " secs";

			if(iter_jobs->second.isStopped)
			{
				std::cout << " (Stopped)";
				kill((pid_t)(iter_jobs->first), SIGCONT);
				kill((pid_t)(iter_jobs->first), SIGTSTP);
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
		int job_idx = 0;
		if (num_arg == 0)
		{
			job_idx = jobs.size();	
		}
		else
		{
			job_idx = std::atoi(args[1]);
		}
		auto job_it = jobs.begin();
		for (int i = 1; i < job_idx; i++)
		{
			job_it++;
		}
		kill(job_it->first, SIGCONT);
		waitpid(job_it->first, NULL, 0);

	} 
	else if (!strcmp(cmd, "bg")) 
	{
		int job_idx = 0;
		if (num_arg == 0)
		{
			job_idx = jobs.size();	
		}
		else
		{
			job_idx = std::atoi(args[1]);
		}
		auto job_it = jobs.begin();
		for (int i = 1; i < job_idx; i++)
		{
			job_it++;
		}

		int status;
		bool is_stopped = false;
		pid_t pid = waitpid(job_it->first, &status, WNOHANG | WUNTRACED);
		if(pid > 0)
		{
			is_stopped = WIFSTOPPED(status);
		}

		if(!is_stopped)
		{
			std::cout << "smash error: > process not Stopped" << std::endl;
			return 0;
		}
		std::cout << job_it->second.name << std::endl;
		auto line_str = "kill -" + std::to_string(SIGCONT) + " " + std::to_string(job_idx);
		auto line = (char*)line_str.c_str();
		ExeCmd(jobs, line, line, command_history);
		return 0;
	}
	else if (!strcmp(cmd, "quit"))
	{
   		if(num_arg == 0)
		{
		   exit(0);
		}
		else if (!strcmp(args[1],"kill"))
		{
			int i=1;
			for(auto job_it = jobs.begin(); job_it != jobs.end(); job_it++)
			{	
				std::cout << "[" << i << "] " << job_it->second.name <<
				 " - Sending SIGTERM..." << std::flush;
				kill((pid_t)(job_it->first), SIGTERM);

				std::this_thread::sleep_for(std::chrono::milliseconds(5000));

				int status;
				pid_t pid = waitpid(job_it->first, &status, WNOHANG);
				if(pid == 0)
				{
					std::cout << " (5 sec passed) Sending SIGKILL...";
					if(kill((pid_t)(job_it->first), SIGKILL)<0)
					{
						std::cout << "Error sending SIGKILL" << std::endl;
					}
					else
					{
						std::cout << "Done" << std::endl;
					}		
				}
				else
				{
					std::cout << "Done" << std::endl;
				}
				
				i++;
			}
			exit(0);
		}
	} 
	else if (!strcmp(cmd, "kill")) 
	{
		// arg[1] = -signum	
		// arg[2] = job 
		
		int signum = -(std::atoi(args[1]));
		int job_num = std::atoi(args[2]);
		
		if(jobs.size() < (unsigned int)job_num)
		{
			std::cout << "smash error: > kill " << job_num 
			<< " –job does not exist" << std::endl;
			return 0;
		}

		auto iter_jobs = jobs.begin();
		for(int i = 1; i<job_num; i++)
		{
			iter_jobs ++;
		}

		if(kill((pid_t)(iter_jobs->first), signum) < 0 )
		{
			std::cout << "smash error: > kill " << job_num 
			<< " -cannot send signal" << std::endl;
			return 0;
		}
		else
		{
			std::cout << "signal " << strsignal(signum) << " was sent to pid " 
					  << iter_jobs->first << std::endl;
		}
				
		return 0;

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
	else if (!strcmp(cmd, "diff")) 
	{
		char buf1, buf2;
		int f1 = open(args[1], O_RDONLY);
		int f2 = open(args[2], O_RDONLY);
		int n1, n2;
        if(f1 == -1 || f2 == -1)
        {
            perror("FILE ERROR");
            exit(0);
        }
        else
        {
			n1=read(f1,&buf1,1);
			n2=read(f2,&buf2,1);
			while(n1 != 0 || n2 != 0 )
			{
				//std::cout << "buf1: " << buf1 << " buf2: " << buf2 << std::endl; 
				if(buf1 != buf2)
				{
					std::cout << 1 << std::endl;
					close(f1);
					close(f2);
					return 0;
				}
				n1=read(f1,&buf1,1);
				n2=read(f2,&buf2,1);
			}
			std::cout << 0 << std::endl;
			close(f1);
			close(f2);
			return 0;
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
					fg_job_pid = pID;
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
					setpgrp();
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

