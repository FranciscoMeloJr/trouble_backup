#ifndef WORKLOADINSTRUCTIONS_H
#define WORKLOADINSTRUCTIONS_H

#include "workload.h"

class WorkloadInstructions : public Workload
{
public:
    WorkloadInstructions(int times = 10);
    void run();
    void reset();
    int factorial(int n);
    void set_times(int n);
    void slow();
    void fast();

private:
    int times;
};


#endif // WORKLOADINSTRUCTIONS_H
