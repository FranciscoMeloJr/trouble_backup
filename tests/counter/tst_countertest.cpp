#include <QString>
#include <QtTest>

#include <counter.h>

class CounterTest : public QObject
{
    Q_OBJECT

public:
    CounterTest();

private Q_SLOTS:
    void testCounters();
};

CounterTest::CounterTest()
{
}

void CounterTest::testCounters()
{
    QStringList lst = Counter::getAvailableEvents();
    QVERIFY2(lst.size() > 0, "No events found");

    int avail = 0;
    for (QString name: lst) {
        quint64 val;
        Counter counter(name);
        if (counter.open()) {
            avail++;
            counter.enable();
            QVERIFY2(counter.read(val), "read failed");
            counter.disable();
        }
    }
    qDebug() << avail << "/" << lst.size();
}

QTEST_APPLESS_MAIN(CounterTest)

#include "tst_countertest.moc"
