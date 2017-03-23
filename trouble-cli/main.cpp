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

/* perf_event_open syscall wrapper */
#include<JsonTest.h>

using namespace std;

static long
sys_perf_event_open(struct perf_event_attr *event,
                    pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, event, pid, cpu, group_fd, flags);
}

static inline pid_t gettid()
{
    return syscall(SYS_gettid);
}

void do_compute(int repeat) {
    volatile int x = 0;
    while(x < repeat) {
        x++;
    }
}

void read_data(QVector<int> &v, QVector<int> &idx)
{
    volatile int b = 0;
    (void) b;
    int size = idx.size();
    for (int i = 0; i < (1 << 20); i++) {
        b = v[idx[i % size] % v.size()];
    }
}

class Sample {
public:
    bool slow;
    qint64 delta;
    qint64 inst;
    qint64 cpu;
    qint64 miss;
    qint64 page;
    qint64 switches;
    qint64 bus;
    qint64 pref;

    friend QDebug operator<< (QDebug dbg, const Sample model) {
        dbg << "(" << model.slow << model.delta << "," << model.inst << "," << model.cpu << "," << model.miss << "," << model.page;
        return dbg;
    }
};

void write_to_file(Sample s){
    QFile file("new_version.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        //out << "bool, time, instructions, cpu, miss, page \n";
        out << s.slow << "," << s.delta << "," << s.inst << "," << s.cpu << "," << s.miss << "," << s.page << "\n";
        file.close();
    }
};

void write_samples(QList<Sample> samples){
    QFile file("jsoncpp.csv");
       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           QTextStream out(&file);
           out << "bool, time, instructions, cpu, miss, page \n";
           for (const Sample &s: samples) {
               out << s.slow << "," << s.delta << "," << s.inst << "," << s.cpu << "," << s.miss << "," << s.page << "\n";
           }
           file.close();
       }
};


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    perf_event_attr attr_inst, attr_cpu, attr_miss, attr_page, attr_switches, attr_pref, attr_bus;

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

    memset(&attr_page, 0, sizeof(attr_page));
    attr_page.size = sizeof(attr_page);
    attr_page.type = PERF_TYPE_SOFTWARE;
    attr_page.config = PERF_COUNT_SW_PAGE_FAULTS;

    memset(&attr_switches, 0, sizeof(attr_switches));
    attr_switches.size = sizeof(attr_switches);
    attr_switches.type = PERF_TYPE_SOFTWARE;
    attr_switches.config = PERF_COUNT_SW_CONTEXT_SWITCHES;

    memset(&attr_pref, 0, sizeof(attr_pref));
    attr_pref.size = sizeof(attr_pref);
    attr_pref.type = PERF_TYPE_HW_CACHE;
    attr_pref.config = PERF_COUNT_HW_CACHE_OP_PREFETCH;


    memset(&attr_bus, 0, sizeof(attr_bus));
    attr_bus.size = sizeof(attr_bus);
    attr_bus.type = PERF_TYPE_HARDWARE;
    attr_bus.config = PERF_COUNT_HW_BUS_CYCLES;

    pid_t tid = gettid();
    int fd_inst = sys_perf_event_open(&attr_inst, tid, -1, -1, 0);
    int fd_cpu = sys_perf_event_open(&attr_cpu, tid, -1, -1, 0);
    int fd_miss = sys_perf_event_open(&attr_cpu, tid, -1, -1, 0);
    int fd_page = sys_perf_event_open(&attr_page, tid, -1, -1, 0);
    int fd_switches = sys_perf_event_open(&attr_switches, tid, -1, -1, 0);
    int fd_pref = sys_perf_event_open(&attr_pref, tid, -1, -1, 0);
    int fd_bus = sys_perf_event_open(&attr_bus, tid, -1, -1, 0);

    int n = 10000;
    QElapsedTimer timer;
    QList<Sample> samples;

    bool debug = false;
    freopen("jsoncpp.csv","w",stdout);
    cout << "time" << "," << "inst" << "," << "cpy cycles" << "," << "misses" << "," << "pages" << "," << "switches" << "," << "bus" << "," << "pref" << "\n";

    for (int i = 0; i < n; i++) {
        //qDebug() << "before";
        cout << "0";

        Sample sample;
        JsonTest* json = new JsonTest();
        json->setUp();

        qint64 ret = 0;
        qint64 val_inst0, val_inst1;
        qint64 val_cpu0, val_cpu1;
        qint64 val_miss0, val_miss1;
        qint64 val_page0, val_page1;
        qint64 val_switches0, val_switches1;
        qint64 val_pref0, val_pref1;

        if(debug)
            cout <<" 1";
        ret |= read(fd_inst, &val_inst0, sizeof(val_inst0));
        ret |= read(fd_cpu, &val_cpu0, sizeof(val_cpu0));
        ret = read(fd_miss, &val_miss0, sizeof(val_miss0));
        ret = read(fd_miss, &val_miss0, sizeof(val_miss0));
        ret = read(fd_page, &val_page0, sizeof(val_page0));
        ret = read(fd_switches, &val_switches0, sizeof(val_switches0));
        read(fd_pref, &val_pref0, sizeof(val_pref0));

        bool slow = false; //version 3.0

        assert(ret > 0);
        timer.restart();
        json->read();
        qint64 delta = timer.nsecsElapsed() / 1000;
        if(debug)
            cout << "2";
        //do_compute(work[i % work.size()] * scale);
        ret |= read(fd_inst, &val_inst1, sizeof(val_inst1));
        ret |= read(fd_cpu, &val_cpu1, sizeof(val_cpu1));
        ret |= read(fd_miss, &val_miss1, sizeof(val_miss1));
        ret |= read(fd_page, &val_page1, sizeof(val_page1));
        ret = read(fd_switches, &val_switches1, sizeof(val_switches1));
        ret = read(fd_pref, &val_pref1, sizeof(val_pref1));

        if(debug)
            cout << "3";

        //qDebug() << "after" << delta;

        sample.slow = slow;
        sample.delta = delta;
        sample.inst = (val_inst1 - val_inst0) / 1000;
        sample.cpu = (val_cpu1 - val_cpu0) / 1000;
        sample.miss = (val_miss1 - val_miss0);
        sample.page = (val_page1 - val_page0);
        sample.switches = (val_switches1 - val_switches0);
        sample.pref = (val_pref1 - val_pref0);

        if(debug)
            cout << "4";

        cout << sample.delta << "," << sample.inst << "," << sample.cpu << "," << sample.miss << "," << sample.page << "," << sample.switches << "," << sample.bus << "," << sample.pref << "\n";

    }


    return 0;
}
