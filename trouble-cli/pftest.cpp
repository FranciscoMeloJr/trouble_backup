#include <iostream>
#include <sys/time.h>
#include <time.h>
#include "pftest.h"

#define tic(start) clock_gettime(CLOCK_MONOTONIC, &start)
#define toc(end) clock_gettime(CLOCK_MONOTONIC, &end)
#define elapsed_nsec(start, end) (end.tv_nsec + 1E9 * end.tv_sec) - (start.tv_nsec + 1E9 * start.tv_sec)


using namespace std;

void pftest::write_func(int value)
{
        buffer[value] = value;
}

void pftest::init(int s){
    size = s;
    buffer = new int[size];

}
void pftest::write(int i){
    buffer[i] = i;
}

int pftest::execute(int s)
{
    int size = s;
    int* buffer = new int[size];

    for(int i = 0; i < size; i++)
    {
        buffer[i] = i;
        write_func(i);
    }

    delete buffer;
    return 0;
}
