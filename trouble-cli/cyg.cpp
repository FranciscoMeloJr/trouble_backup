#include <stdio.h>
#include <time.h>
#include <dlfcn.h>
#include <sys/types.h>
#include "cyg.h"
#include <iostream>
using namespace std;
void __attribute__ ((constructor)) trace_begin (void)
{
    //printf("Begin \n");
}

void __attribute__ ((destructor)) trace_end (void)
{
     //printf("trace_end\n");
}

void __cyg_profile_func_enter (void *func,  void *caller)
{
    //printf("Entry %p\n",func);
    cout << "Entry "  << endl;
        //tracepoint(lttng_ust_cyg_profile, func_entry, func, caller);
    //func_count++;

}

void __cyg_profile_func_exit (void *func, void *caller)
{

//    cout << "Exit " << func << endl;
        //tracepoint(lttng_ust_cyg_profile, func_exit, func, caller);
    //func_count++;
}
