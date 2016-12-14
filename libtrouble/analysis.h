#ifndef ANALYSIS
#define ANALYSIS

#include <QObject>
#include <linux/perf_event.h>
#include <QVector>
#include <QDebug>

class Analysis : public QObject
{

public:
    explicit Analysis();
    QVector<int> v1;
    QVector<int> v2;
};

#endif // ANALYSIS

