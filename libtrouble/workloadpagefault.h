#ifndef WORKLOADPAGEFAULT_H
#define WORKLOADPAGEFAULT_H

#include "workload.h"

class WorkloadPageFault : public Workload
{
public:
    WorkloadPageFault(int pages = 10);
    void run();
private:
    int m_pages;
};

#endif // WORKLOADPAGEFAULT_H
