#include "workloadcachemisses.h"

#include <sys/mman.h>
#include <unistd.h>

#include <QDebug>

//Constructor:
WorkloadCacheMisses::WorkloadCacheMisses(int s) : m_sz(s)
{
    qDebug() << " idx ";
    init();
    idx_rnd = QVector<int>(m_sz);

    idx_lin = QVector<int>(m_sz);
    buf_large = QVector<int>(m_sz);
    buf_small = QVector<int>(10);

    for (int i = 0; i < m_sz; i++) {
        idx_rnd[i] = i;
        idx_lin[i] = i;
    }
    std::random_shuffle(idx_rnd.begin(), idx_rnd.end());

}

//This function set_up the data
void WorkloadCacheMisses::set_up(int occurrences)
{
    m_jump = occurrences;
}
//This function runs the workload:
void WorkloadCacheMisses::run(int slowness_ratio)
{
    //testing with different n's
    int n = slowness_ratio;
    set_up(n);
    //0.166666667 %
    for (int i = 0; i < n; i++) {
        bool slow = (i % m_jump) == 0;
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
    m_sz = s;
}

//This function set the size:
void WorkloadCacheMisses::set_times(int n_times)
{
    m_n = n_times;
}
//This function initiate the class
void WorkloadCacheMisses::init()
{
    qDebug() << "Init";
}
