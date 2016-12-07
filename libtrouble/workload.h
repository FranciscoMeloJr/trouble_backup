#ifndef WORKLOAD_H
#define WORKLOAD_H


class Workload
{
public:
    Workload();
    void setSlowdown(float ratio);
    virtual void run() = 0;
    virtual void reset() = 0;
private:
    float m_slowdown;
};

#endif // WORKLOAD_H
