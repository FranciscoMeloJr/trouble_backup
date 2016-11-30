#include <algorithm>

#include "workload.h"

Workload::Workload()
{

}

void Workload::setSlowdown(float ratio)
{
    m_slowdown = std::min(std::max(ratio, (float)0), (float)1);
}
