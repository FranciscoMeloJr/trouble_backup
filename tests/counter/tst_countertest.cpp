#include <QString>
#include <QtTest>

#include <counter.h>

class CounterTest : public QObject
{
    Q_OBJECT

public:
    CounterTest();

private Q_SLOTS:
    void testListCounters();
};

CounterTest::CounterTest()
{
}

void CounterTest::testListCounters()
{
    QStringList lst = Counter::getEventsList();
    QVERIFY2(lst.size() > 0, "No events found");
}

QTEST_APPLESS_MAIN(CounterTest)

#include "tst_countertest.moc"
