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
    //for each event in the list:
    for (QString name: lst) {
        quint64 val;
        Counter counter(name);
        //run the counter:
        if (counter.open()) {
            avail++;
            counter.enable();
            QVERIFY2(counter.read(val), "read failed");
            counter.disable();
        }
    }
    //availability:
    qDebug() << "Availability:" << avail << "/" << lst.size();
}

QTEST_APPLESS_MAIN(CounterTest)

#include "tst_countertest.moc"
