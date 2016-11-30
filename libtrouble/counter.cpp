#include <QMap>
#include <sys/syscall.h>
#include <unistd.h>

#include "counter.h"

// for PERF_TYPE_HW_CACHE, the config is a bitmask
// lowest 8 bits: cache type
// bits 8 to 15: cache operation
// bits 16 to 23: cache result
#define CACHE_L1D_READ              (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_L1D_WRITE             (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_WRITE << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_L1D_PREFETCH          (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_L1I_READ              (PERF_COUNT_HW_CACHE_L1I | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_L1I_PREFETCH          (PERF_COUNT_HW_CACHE_L1I | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_LLC_READ              (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_LLC_WRITE             (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_WRITE << 8| PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_LLC_PREFETCH          (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_L1D_READ_MISS         (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_L1D_WRITE_MISS        (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_WRITE << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_L1D_PREFETCH_MISS     (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_L1I_READ_MISS         (PERF_COUNT_HW_CACHE_L1I | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_L1I_PREFETCH_MISS     (PERF_COUNT_HW_CACHE_L1I | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_LLC_READ_MISS         (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_LLC_WRITE_MISS        (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_WRITE << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_LLC_PREFETCH_MISS     (PERF_COUNT_HW_CACHE_LL  | PERF_COUNT_HW_CACHE_OP_PREFETCH << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
#define CACHE_BRANCH_READ           (PERF_COUNT_HW_CACHE_BPU | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16)
#define CACHE_BRANCH_READ_MISS      (PERF_COUNT_HW_CACHE_BPU | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)

struct Events {
    const char *name;
    quint32 type;
    quint64 id;
};

static const Events eventlist[] = {
    { "alignment-faults" , PERF_TYPE_SOFTWARE, PERF_COUNT_SW_ALIGNMENT_FAULTS },
    { "branch-instructions" , PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS },
    { "branch-load-misses", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS },
    { "branch-loads", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ },
    { "branch-mispredicts", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS },
    { "branch-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES },
    { "branch-predicts", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ },
    { "branch-read-misses", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS },
    { "branch-reads", PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ },
    { "branches", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS },
    { "bus-cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES },
    { "cache-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES },
    { "cache-references", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES },
    { "context-switches", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES },
    { "cpu-clock", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_CLOCK },
    { "cpu-cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES },
    { "cpu-migrations", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_MIGRATIONS },
    { "cs", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES },
    { "cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES },
    { "emulation-faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_EMULATION_FAULTS },
    { "faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS },
    { "idle-cycles-backend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND },
    { "idle-cycles-frontend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND },
    { "instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS },
    { "l1d-cache-load-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS },
    { "l1d-cache-loads", PERF_TYPE_HW_CACHE, CACHE_L1D_READ },
    { "l1d-cache-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH_MISS },
    { "l1d-cache-prefetches", PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH },
    { "l1d-cache-read-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS },
    { "l1d-cache-reads", PERF_TYPE_HW_CACHE, CACHE_L1D_READ },
    { "l1d-cache-store-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS },
    { "l1d-cache-stores", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE },
    { "l1d-cache-write-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS },
    { "l1d-cache-writes", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE },
    { "l1d-load-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS },
    { "l1d-loads", PERF_TYPE_HW_CACHE, CACHE_L1D_READ },
    { "l1d-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH_MISS },
    { "l1d-prefetches", PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH },
    { "l1d-read-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS },
    { "l1d-reads", PERF_TYPE_HW_CACHE, CACHE_L1D_READ },
    { "l1d-store-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS },
    { "l1d-stores", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE },
    { "l1d-write-misses", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS },
    { "l1d-writes", PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE },
    { "l1i-cache-load-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS },
    { "l1i-cache-loads", PERF_TYPE_HW_CACHE, CACHE_L1I_READ },
    { "l1i-cache-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH_MISS },
    { "l1i-cache-prefetches", PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH },
    { "l1i-cache-read-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS },
    { "l1i-cache-reads", PERF_TYPE_HW_CACHE, CACHE_L1I_READ },
    { "l1i-load-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS },
    { "l1i-loads", PERF_TYPE_HW_CACHE, CACHE_L1I_READ },
    { "l1i-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH_MISS },
    { "l1i-prefetches", PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH },
    { "l1i-read-misses", PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS },
    { "l1i-reads", PERF_TYPE_HW_CACHE, CACHE_L1I_READ },
    { "llc-cache-load-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS },
    { "llc-cache-loads", PERF_TYPE_HW_CACHE, CACHE_LLC_READ },
    { "llc-cache-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH_MISS },
    { "llc-cache-prefetches", PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH },
    { "llc-cache-read-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS },
    { "llc-cache-reads", PERF_TYPE_HW_CACHE, CACHE_LLC_READ },
    { "llc-cache-store-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS },
    { "llc-cache-stores", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE },
    { "llc-cache-write-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS },
    { "llc-cache-writes", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE },
    { "llc-load-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS },
    { "llc-loads", PERF_TYPE_HW_CACHE, CACHE_LLC_READ },
    { "llc-prefetch-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH_MISS },
    { "llc-prefetches", PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH },
    { "llc-read-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS },
    { "llc-reads", PERF_TYPE_HW_CACHE, CACHE_LLC_READ },
    { "llc-store-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS },
    { "llc-stores", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE },
    { "llc-write-misses", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS },
    { "llc-writes", PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE },
    { "major-faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MAJ },
    { "migrations", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_MIGRATIONS },
    { "minor-faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MIN },
    { "page-faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS },
    { "stalled-cycles-backend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND },
    { "stalled-cycles-frontend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND },
    { "task-clock", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_TASK_CLOCK },
    {   nullptr, PERF_TYPE_MAX, 0 }
};

static inline pid_t gettid()
{
    return syscall(SYS_gettid);
}

/* perf_event_open syscall wrapper */
static long
sys_perf_event_open(struct perf_event_attr *event,
                    pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, event, pid, cpu, group_fd, flags);
}

static QMap<QString, int> my_map{{"a", 1}, {"b", 2}, {"c", 3}};

Counter::Counter(QObject *parent) : QObject(parent)
{
    memset(&m_attr, 0, sizeof(m_attr));
    m_attr.size = sizeof(struct perf_event_attr);
    m_attr.disabled = true;
    m_attr.pinned = true;
}

long Counter::value()
{

}

int Counter::open()
{

}

int Counter::close()
{

}
