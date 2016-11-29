#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QElapsedTimer>
#include <QVector>
#include <QFile>
#include <cassert>
#include <algorithm>

#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tp.h"
#include <math.h>

#include "counter.h"

//Class for the tests:
class Tests {
public:
    static void do_compute(int repeat) {
        volatile int x = 0;
        while(x < repeat) {
            x++;
        }
    }

    static void read_data(QVector<int> &v, QVector<int> &idx)
    {
        volatile int b = 0;
        (void) b;
        int size = idx.size();
        for (int i = 0; i < (1 << 20); i++) {
            b = v[idx[i % size] % v.size()];
        }
    }
    static int factorial(int n)
    {
      return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
    }

    //http://computing.dcu.ie/~humphrys/Notes/Neural/chaos.html
    static double sinoide_example(double x){

        return sin((1/x)*(1/(1-x)));
    }
};

class Sample {
public:
    bool slow;
    qint64 delta;
    qint64 inst;
    qint64 cpu;
    qint64 miss;
    friend QDebug operator<< (QDebug dbg, const Sample model) {
        dbg << "(" << model.slow << model.delta << "," << model.inst << "," << model.cpu << "," << model.miss << ")";
        return dbg;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QElapsedTimer timer;
    perf_event_attr attr_inst, attr_cpu, attr_miss;

    memset(&attr_inst, 0, sizeof(attr_inst));
    attr_inst.size = sizeof(attr_inst);
    attr_inst.type = PERF_TYPE_HARDWARE;
    attr_inst.config = PERF_COUNT_HW_INSTRUCTIONS;

    memset(&attr_cpu, 0, sizeof(attr_cpu));
    attr_cpu.size = sizeof(attr_cpu);
    attr_cpu.type = PERF_TYPE_SOFTWARE;
    attr_cpu.config = PERF_COUNT_SW_TASK_CLOCK;

    memset(&attr_miss, 0, sizeof(attr_miss));
    attr_miss.size = sizeof(attr_miss);
    attr_miss.type = PERF_TYPE_HW_CACHE;
    attr_miss.config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

    /*
    pid_t tid = gettid();
    int fd_inst = sys_perf_event_open(&attr_inst, tid, -1, -1, 0);
    int fd_cpu = sys_perf_event_open(&attr_cpu, tid, -1, -1, 0);
    int fd_miss = sys_perf_event_open(&attr_cpu, tid, -1, -1, 0);
    */

    int fd_inst, fd_cpu, fd_miss;

    int n = 50;
    QVector<Sample> samples(n);

    int sz = 100000*5;
    QVector<int> idx_rnd(sz); // 100 000 * sizeof(int) = 400 kio, L1 32kio, LLC 4mb
    QVector<int> idx_lin(sz);
    QVector<int> buf_large(sz);
    QVector<int> buf_small(10);
    for (int i = 0; i < sz; i++) {
        idx_rnd[i] = i;
        idx_lin[i] = i;
    }
    std::random_shuffle(idx_rnd.begin(), idx_rnd.end());


    for (int i = 0; i < n; i++) {
        //qDebug() << "before";
        timer.restart();

        qint64 ret = 0;
        qint64 val_inst0, val_inst1;
        qint64 val_cpu0, val_cpu1;
        qint64 val_miss0, val_miss1;

        ret |= read(fd_inst, &val_inst0, sizeof(val_inst0));
        ret |= read(fd_cpu, &val_cpu0, sizeof(val_cpu0));
        ret = read(fd_miss, &val_miss0, sizeof(val_miss0));
        assert(ret > 0);

        bool slow = (i % 6) == 0;
        if (slow) {
//            Tests::read_data(buf_large, idx_rnd);
            Tests::factorial(1000);

        } else {
//            Tests::read_data(buf_small, idx_lin);
          Tests::factorial(3);
        }

//        do_compute(work[i % work.size()] * scale);
        ret |= read(fd_inst, &val_inst1, sizeof(val_inst1));
        ret |= read(fd_cpu, &val_cpu1, sizeof(val_cpu1));
        ret |= read(fd_miss, &val_miss1, sizeof(val_miss1));

        qint64 delta = timer.nsecsElapsed() / 1000;
        //qDebug() << "after" << delta;
        samples[i].slow = slow;
        samples[i].delta = delta;
        samples[i].inst = (val_inst1 - val_inst0) / 1000;
        samples[i].cpu = (val_cpu1 - val_cpu0) / 1000;
        samples[i].miss = (val_miss1 - val_miss0);

    }

    //run through all of them:
    for (int i = 0; i < n; i++) {
        Sample s = samples[i];
        qDebug() << s;
    }

    //write the csv:
    QFile file("troubleSample.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Sample &s: samples) {
            out << s.slow << "," << s.delta << "," << s.inst << "," << s.cpu << "," << s.miss << "\n";
        }
        file.close();
    }

    tracepoint(cct, getinfo, 2, 2, "context");

    return 0;
}

