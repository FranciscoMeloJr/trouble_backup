#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>
#include <linux/perf_event.h>

class Counter : public QObject
{
    Q_ENUMS(CounterType)
    Q_OBJECT
public:
    explicit Counter(QString name, QObject *parent = 0);
    bool read(quint64 &value);
    bool open();
    void close();
    void enable();
    void disable();

    static QStringList getAvailableEvents();

signals:

public slots:

private:
    QString m_name;
    int m_fd;
};

#endif // COUNTER_H
