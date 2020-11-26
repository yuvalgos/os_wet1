
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */

#include "signals.h"
#include <iostream>

void catch_stp(int sig_num)
{
   std::cout << "catch_stp" << std::endl;
}
void catch_int(int sig_num)
{
   std::cout << "catch_int" << std::endl;
}
