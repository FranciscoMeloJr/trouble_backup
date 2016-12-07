#ifndef WORKLOADCACHEMISSES_H
#define WORKLOADCACHEMISSES_H

#include "workload.h"
#include <QVector>

class WorkloadCacheMisses : public Workload
{
public:
    WorkloadCacheMisses(int s = 10000);
    void run();
    void reset();
    void read_data(QVector<int> &v, QVector<int> &idx);
    void init();
    void set_size(int s);
    void set_times(int n_times);

private:
    int cache_misses;
    int sz;
    int n = 100;
    QVector<int> idx_rnd;
    QVector<int> idx_lin;
    QVector<int> buf_large;
    QVector<int> buf_small;

};

#endif // WORKLOADCACHEMISSES_H

