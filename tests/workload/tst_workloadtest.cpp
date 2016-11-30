#include <QString>
#include <QtTest>

#include <counter.h>
#include <workloadpagefault.h>

class WorkloadTest : public QObject
{
    Q_OBJECT

public:
    WorkloadTest();

private Q_SLOTS:
    void testCase1();
};

WorkloadTest::WorkloadTest()
{
}

void WorkloadTest::testCase1()
{
    int exp = 100;
    WorkloadPageFault pf(exp);
    quint64 v1, v2;


    Counter counter("page-faults");
    QVERIFY2(counter.open(), "open failed");
    QVERIFY2(counter.read(v1), "read failed");
    pf.run();
    QVERIFY2(counter.read(v2), "read failed");

    quint64 diff = v2 - v1;
    qDebug() << v1 << v2 << diff;
    QVERIFY2(diff >= exp, "less page faults then expected");
}

QTEST_APPLESS_MAIN(WorkloadTest)

#include "tst_workloadtest.moc"
