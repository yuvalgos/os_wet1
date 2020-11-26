#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <map>

#include "Job.h"

extern std::map<int, Job> jobs;
extern Job fg_job;

void catch_stp(int sig_num);
void catch_int(int sig_num);
#endif

