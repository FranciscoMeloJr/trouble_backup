#include "workloadinstructions.h"

#include <sys/mman.h>
#include <unistd.h>

#include <QDebug>

WorkloadInstructions::WorkloadInstructions(int)
{
}

//This function runs the workload:
void WorkloadInstructions::run()
{
    set_times(10);

    for (int i = 0; i < times; i++) {
        bool slow = (i % 6) == 0;
        if (slow) {
              this->slow();

        } else {
              this->fast();
        }
    }
}

//This function reset the workload:
void WorkloadInstructions::reset()
{
    qDebug() << "Reset";
}

int WorkloadInstructions::factorial(int n)
{
      return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void WorkloadInstructions::set_times(int t){
    times = t;
}

void WorkloadInstructions::slow(){
    this->factorial(1000);
}

void WorkloadInstructions::fast(){
    this->factorial(10);
}
