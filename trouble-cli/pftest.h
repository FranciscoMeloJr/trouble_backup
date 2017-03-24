#ifndef PFTEST_H
#define PFTEST_H

class pftest
{
    int* buffer;
    int size;
public:
    void write_func(int value);
    void init(int s);
    int execute(int);
    void write(int);

};

#endif // PFTEST_H
