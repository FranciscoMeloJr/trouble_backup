#include "workloadpagefault.h"

#include <sys/mman.h>
#include <unistd.h>

#include <QDebug>

WorkloadPageFault::WorkloadPageFault(int pages) : m_pages(pages)
{
}

//This function runs the workload:
void WorkloadPageFault::run()
{
    size_t page_size = getpagesize();
    size_t buf_size = page_size * m_pages;
    qDebug() << page_size << buf_size;
    void *mm = ::mmap(nullptr, buf_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    qDebug() << mm;
    if (mm == MAP_FAILED)
        return;

    char *buf = static_cast<char *>(mm);
    for (size_t i = 0; i < buf_size; i++) {
        buf[i] = i;
    }
    ::munmap(mm, buf_size);
}

//This function reset the workload:
void WorkloadPageFault::reset()
{
    qDebug() << "Reset";
}
