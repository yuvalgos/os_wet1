#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int fg_job_pid = -1;

void catch_stp(int sig_num);
void catch_int(int sig_num);
#endif

