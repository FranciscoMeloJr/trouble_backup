#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>
#include <linux/perf_event.h>

class Counter : public QObject
{
    Q_ENUMS(CounterType)
    Q_OBJECT
public:
    enum CounterType {
        Instruction,
    };
    explicit Counter(QObject *parent = 0);
    long value();
    int open();
    int close();

signals:

public slots:

private:
    struct perf_event_attr m_attr;
};

#endif // COUNTER_H
