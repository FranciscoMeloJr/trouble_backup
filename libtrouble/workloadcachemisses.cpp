#include "workloadcachemisses.h"

#include <sys/mman.h>
#include <unistd.h>

#include <QDebug>

//Constructor:
WorkloadCacheMisses::WorkloadCacheMisses(int s) : sz(s)
{
    qDebug() << " idx ";
    init();
    idx_rnd = QVector<int>(sz);

    idx_lin = QVector<int>(sz);
    buf_large = QVector<int>(sz);
    buf_small = QVector<int>(10);

    for (int i = 0; i < sz; i++) {
        idx_rnd[i] = i;
        idx_lin[i] = i;
    }
    std::random_shuffle(idx_rnd.begin(), idx_rnd.end());

}

//This function set_up the data
void WorkloadCacheMisses::set_up(int occurrences)
{
    jump = occurrences;
}
//This function runs the workload:
void WorkloadCacheMisses::run()
{
    set_up(6);
    //0.166666667 %
    for (int i = 0; i < n; i++) {
        bool slow = (i % jump) == 0;
        if (slow) {
                  read_data(buf_large, idx_rnd);
        } else {
                  read_data(buf_small, idx_lin);
        }
    }
}

//This function reset the workload:
void WorkloadCacheMisses::reset()
{
    qDebug() << "Reset";
}

//Function to read the data
void WorkloadCacheMisses::read_data(QVector<int> &v, QVector<int> &idx)
{

    volatile int b = 0;
    (void) b;
    int size = idx.size();
    for (int i = 0; i < (1 << 20); i++) {
            b = v[idx[i % size] % v.size()];
    }
}

//This function set the size:
void WorkloadCacheMisses::set_size(int s)
{
    sz = s;
}

//This function set the size:
void WorkloadCacheMisses::set_times(int n_times)
{
    n = n_times;
}
//This function initiate the class
void WorkloadCacheMisses::init()
{

}
