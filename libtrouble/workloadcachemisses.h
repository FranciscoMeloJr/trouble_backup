#ifndef WORKLOADCACHEMISSES_H
#define WORKLOADCACHEMISSES_H

#include "workload.h"
#include <QVector>

class WorkloadCacheMisses : public Workload
{
public:
    WorkloadCacheMisses(int s = 10000);
    void run(int slowness_ratio);
    void reset();
    void read_data(QVector<int> &v, QVector<int> &idx);
    void init();
    void set_up(int occurrences);
    void set_size(int s);
    void set_times(int n_times);

private:
    int m_cache_misses;
    int m_sz;
    int m_n = 100;
    int m_jump;
    QVector<int> idx_rnd;
    QVector<int> idx_lin;
    QVector<int> buf_large;
    QVector<int> buf_small;

};

#endif // WORKLOADCACHEMISSES_H

