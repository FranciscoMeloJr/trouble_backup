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

namespace QTest {

enum QBenchmarkMetric {
    FramesPerSecond,
    BitsPerSecond,
    BytesPerSecond,
    WalltimeMilliseconds,
    CPUTicks,
    InstructionReads,
    Events,
    WalltimeNanoseconds,
    BytesAllocated,
    CPUMigrations,
    CPUCycles,
    BusCycles,
    StalledCycles,
    Instructions,
    BranchInstructions,
    BranchMisses,
    CacheReferences,
    CacheReads,
    CacheWrites,
    CachePrefetches,
    CacheMisses,
    CacheReadMisses,
    CacheWriteMisses,
    CachePrefetchMisses,
    ContextSwitches,
    PageFaults,
    MinorPageFaults,
    MajorPageFaults,
    AlignmentFaults,
    EmulationFaults
};

}

struct Events {
    unsigned offset;
    quint32 type;
    quint64 event_id;
    QTest::QBenchmarkMetric metric;
};

static const char eventlist_strings[] =
    "alignment-faults\0"
    "branch-instructions\0"
    "branch-load-misses\0"
    "branch-loads\0"
    "branch-mispredicts\0"
    "branch-misses\0"
    "branch-predicts\0"
    "branch-read-misses\0"
    "branch-reads\0"
    "branches\0"
    "bus-cycles\0"
    "cache-misses\0"
    "cache-references\0"
    "context-switches\0"
    "cpu-clock\0"
    "cpu-cycles\0"
    "cpu-migrations\0"
    "cs\0"
    "cycles\0"
    "emulation-faults\0"
    "faults\0"
    "idle-cycles-backend\0"
    "idle-cycles-frontend\0"
    "instructions\0"
    "l1d-cache-load-misses\0"
    "l1d-cache-loads\0"
    "l1d-cache-prefetch-misses\0"
    "l1d-cache-prefetches\0"
    "l1d-cache-read-misses\0"
    "l1d-cache-reads\0"
    "l1d-cache-store-misses\0"
    "l1d-cache-stores\0"
    "l1d-cache-write-misses\0"
    "l1d-cache-writes\0"
    "l1d-load-misses\0"
    "l1d-loads\0"
    "l1d-prefetch-misses\0"
    "l1d-prefetches\0"
    "l1d-read-misses\0"
    "l1d-reads\0"
    "l1d-store-misses\0"
    "l1d-stores\0"
    "l1d-write-misses\0"
    "l1d-writes\0"
    "l1i-cache-load-misses\0"
    "l1i-cache-loads\0"
    "l1i-cache-prefetch-misses\0"
    "l1i-cache-prefetches\0"
    "l1i-cache-read-misses\0"
    "l1i-cache-reads\0"
    "l1i-load-misses\0"
    "l1i-loads\0"
    "l1i-prefetch-misses\0"
    "l1i-prefetches\0"
    "l1i-read-misses\0"
    "l1i-reads\0"
    "llc-cache-load-misses\0"
    "llc-cache-loads\0"
    "llc-cache-prefetch-misses\0"
    "llc-cache-prefetches\0"
    "llc-cache-read-misses\0"
    "llc-cache-reads\0"
    "llc-cache-store-misses\0"
    "llc-cache-stores\0"
    "llc-cache-write-misses\0"
    "llc-cache-writes\0"
    "llc-load-misses\0"
    "llc-loads\0"
    "llc-prefetch-misses\0"
    "llc-prefetches\0"
    "llc-read-misses\0"
    "llc-reads\0"
    "llc-store-misses\0"
    "llc-stores\0"
    "llc-write-misses\0"
    "llc-writes\0"
    "major-faults\0"
    "migrations\0"
    "minor-faults\0"
    "page-faults\0"
    "stalled-cycles-backend\0"
    "stalled-cycles-frontend\0"
    "task-clock\0"
    "\0";

static const Events eventlist[] = {
    {   0, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_ALIGNMENT_FAULTS, QTest::AlignmentFaults },
    {  17, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS, QTest::BranchInstructions },
    {  37, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS, QTest::BranchMisses },
    {  56, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ, QTest::BranchInstructions },
    {  69, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS, QTest::BranchMisses },
    {  88, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES, QTest::BranchMisses },
    { 102, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ, QTest::BranchInstructions },
    { 118, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ_MISS, QTest::BranchMisses },
    { 137, PERF_TYPE_HW_CACHE, CACHE_BRANCH_READ, QTest::BranchInstructions },
    { 150, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS, QTest::BranchInstructions },
    { 159, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES, QTest::BusCycles },
    { 170, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES, QTest::CacheMisses },
    { 183, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES, QTest::CacheReferences },
    { 200, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES, QTest::ContextSwitches },
    { 217, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_CLOCK, QTest::WalltimeMilliseconds },
    { 227, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES, QTest::CPUCycles },
    { 238, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_MIGRATIONS, QTest::CPUMigrations },
    { 253, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES, QTest::ContextSwitches },
    { 256, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES, QTest::CPUCycles },
    { 263, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_EMULATION_FAULTS, QTest::EmulationFaults },
    { 280, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS, QTest::PageFaults },
    { 287, PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND, QTest::StalledCycles },
    { 307, PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, QTest::StalledCycles },
    { 328, PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS, QTest::Instructions },
    { 341, PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS, QTest::CacheReads },
    { 363, PERF_TYPE_HW_CACHE, CACHE_L1D_READ, QTest::CacheReads },
    { 379, PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH_MISS, QTest::CachePrefetches },
    { 405, PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH, QTest::CachePrefetches },
    { 426, PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS, QTest::CacheReads },
    { 448, PERF_TYPE_HW_CACHE, CACHE_L1D_READ, QTest::CacheReads },
    { 464, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS, QTest::CacheWrites },
    { 487, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE, QTest::CacheWrites },
    { 504, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS, QTest::CacheWrites },
    { 527, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE, QTest::CacheWrites },
    { 544, PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS, QTest::CacheReads },
    { 560, PERF_TYPE_HW_CACHE, CACHE_L1D_READ, QTest::CacheReads },
    { 570, PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH_MISS, QTest::CachePrefetches },
    { 590, PERF_TYPE_HW_CACHE, CACHE_L1D_PREFETCH, QTest::CachePrefetches },
    { 605, PERF_TYPE_HW_CACHE, CACHE_L1D_READ_MISS, QTest::CacheReads },
    { 621, PERF_TYPE_HW_CACHE, CACHE_L1D_READ, QTest::CacheReads },
    { 631, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS, QTest::CacheWrites },
    { 648, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE, QTest::CacheWrites },
    { 659, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE_MISS, QTest::CacheWrites },
    { 676, PERF_TYPE_HW_CACHE, CACHE_L1D_WRITE, QTest::CacheWrites },
    { 687, PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS, QTest::CacheReads },
    { 709, PERF_TYPE_HW_CACHE, CACHE_L1I_READ, QTest::CacheReads },
    { 725, PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH_MISS, QTest::CachePrefetches },
    { 751, PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH, QTest::CachePrefetches },
    { 772, PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS, QTest::CacheReads },
    { 794, PERF_TYPE_HW_CACHE, CACHE_L1I_READ, QTest::CacheReads },
    { 810, PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS, QTest::CacheReads },
    { 826, PERF_TYPE_HW_CACHE, CACHE_L1I_READ, QTest::CacheReads },
    { 836, PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH_MISS, QTest::CachePrefetches },
    { 856, PERF_TYPE_HW_CACHE, CACHE_L1I_PREFETCH, QTest::CachePrefetches },
    { 871, PERF_TYPE_HW_CACHE, CACHE_L1I_READ_MISS, QTest::CacheReads },
    { 887, PERF_TYPE_HW_CACHE, CACHE_L1I_READ, QTest::CacheReads },
    { 897, PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS, QTest::CacheReads },
    { 919, PERF_TYPE_HW_CACHE, CACHE_LLC_READ, QTest::CacheReads },
    { 935, PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH_MISS, QTest::CachePrefetches },
    { 961, PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH, QTest::CachePrefetches },
    { 982, PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS, QTest::CacheReads },
    { 1004, PERF_TYPE_HW_CACHE, CACHE_LLC_READ, QTest::CacheReads },
    { 1020, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS, QTest::CacheWrites },
    { 1043, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE, QTest::CacheWrites },
    { 1060, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS, QTest::CacheWrites },
    { 1083, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE, QTest::CacheWrites },
    { 1100, PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS, QTest::CacheReads },
    { 1116, PERF_TYPE_HW_CACHE, CACHE_LLC_READ, QTest::CacheReads },
    { 1126, PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH_MISS, QTest::CachePrefetches },
    { 1146, PERF_TYPE_HW_CACHE, CACHE_LLC_PREFETCH, QTest::CachePrefetches },
    { 1161, PERF_TYPE_HW_CACHE, CACHE_LLC_READ_MISS, QTest::CacheReads },
    { 1177, PERF_TYPE_HW_CACHE, CACHE_LLC_READ, QTest::CacheReads },
    { 1187, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS, QTest::CacheWrites },
    { 1204, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE, QTest::CacheWrites },
    { 1215, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE_MISS, QTest::CacheWrites },
    { 1232, PERF_TYPE_HW_CACHE, CACHE_LLC_WRITE, QTest::CacheWrites },
    { 1243, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MAJ, QTest::MajorPageFaults },
    { 1256, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CPU_MIGRATIONS, QTest::CPUMigrations },
    { 1267, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MIN, QTest::MinorPageFaults },
    { 1280, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS, QTest::PageFaults },
    { 1292, PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND, QTest::StalledCycles },
    { 1315, PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, QTest::StalledCycles },
    { 1339, PERF_TYPE_SOFTWARE, PERF_COUNT_SW_TASK_CLOCK, QTest::WalltimeMilliseconds },
    {   0, PERF_TYPE_MAX, 0, QTest::Events }
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
