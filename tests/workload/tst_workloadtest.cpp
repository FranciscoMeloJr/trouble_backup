#include <QString>
#include <QtTest>

#include <counter.h>
#include <workloadpagefault.h>
#include <workloadcachemisses.h>
#include <workloadinstructions.h>
#include <createfile.h>

class WorkloadTest : public QObject
{
    Q_OBJECT

public:
    WorkloadTest();

private Q_SLOTS:
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();

};

WorkloadTest::WorkloadTest()
{
}

//First test case - page faults
void WorkloadTest::testCase1()
{
    uint exp = 100; // waiting 100 page faults
    WorkloadPageFault pf(exp); //creating the exp page faults
    quint64 v1, v2;

    Counter counter("page-faults");
    QVERIFY2(counter.open(), "open failed");
    QVERIFY2(counter.read(v1), "read failed");
    pf.run();
    QVERIFY2(counter.read(v2), "read failed");
    QVERIFY2(1+1 == 2, "sum failed");
    pf.reset();

    quint64 diff = v2 - v1;
    qDebug() << v1 << v2 << diff;
    QVERIFY2(diff >= exp, "less page faults then expected");

    return;
}

//Third test case - cache misses
void WorkloadTest::testCase2()
{
    uint exp = 100; // waiting 100 page faults
    WorkloadPageFault pf(exp); //creating the exp page faults
    quint64 v1, v2;

    QList<QString> list_speed;
    list_speed << "slow" << "fast";

    QList<QString> list_counter;
    list_counter << "cache-misses" << "page-faults";

    QString speed;
    foreach( speed, list_speed )
        qDebug() << speed;

    Counter counter("cache-misses");
    QVERIFY2(counter.open(), "open failed");
    QVERIFY2(counter.read(v1), "read failed");

    pf.run();
    QVERIFY2(counter.read(v2), "read failed");
    QVERIFY2(1+1 == 2, "sum failed");
    pf.reset();

    quint64 diff = v2 - v1;
    qDebug() << v1 << v2 << diff;
    QVERIFY2(diff >= exp, "less page faults then expected");

    return;
}

//Third test case - cache misses
void WorkloadTest::testCase3(){

    quint64 v1, v2;
    qDebug() << " testCase3 ";

    Counter counter("cache-misses");
    WorkloadCacheMisses* cache = new WorkloadCacheMisses();

    QVERIFY2(counter.open(), "open failed");
    QVERIFY2(counter.read(v1), "read failed");
    cache->run();
    QVERIFY2(counter.read(v2), "read failed");

    //Creating file:
    CreateFile* cf = new CreateFile();
    cf->create();

    return;

}
//Fourth test case - instructions
void WorkloadTest::testCase4(){

    quint64 v1, v2;
    qDebug() << " testCase4 ";

    Counter counter("cache-misses");
    WorkloadInstructions* inst = new WorkloadInstructions();

    QVERIFY2(counter.open(), "open failed");
    QVERIFY2(counter.read(v1), "read failed");
    inst->run();
    QVERIFY2(counter.read(v2), "read failed");

    return;

}
QTEST_APPLESS_MAIN(WorkloadTest)

#include "tst_workloadtest.moc"
