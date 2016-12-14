#include <counter.h>
#include <workloadpagefault.h>
#include <workloadcachemisses.h>
#include <workloadinstructions.h>
#include <analysis.h>
#include <createfile.h>

#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QFile>
#include <cassert>
#include <algorithm>
#include <QElapsedTimer>

//Class for the tests:
//command-line interface
class Tests {
public:
    static void do_compute(int repeat) {
        volatile int x = 0;
        while(x < repeat) {
            x++;
        }
    }

    static void read_data(QVector<int> &v, QVector<int> &idx)
    {
        volatile int b = 0;
        (void) b;
        int size = idx.size();
        for (int i = 0; i < (1 << 20); i++) {
            b = v[idx[i % size] % v.size()];
        }
    }
    static int factorial(int n)
    {
      return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
    }

    //http://computing.dcu.ie/~humphrys/Notes/Neural/chaos.html
    static double sinoide_example(double x){

        return sin((1/x)*(1/(1-x)));
    }
};

class Sample {
public:
    bool slow;
    qint64 delta;
    qint64 inst;
    qint64 cpu;
    qint64 miss;
    friend QDebug operator<< (QDebug dbg, const Sample model) {
        dbg << "(" << model.slow << model.delta << "," << model.inst << "," << model.cpu << "," << model.miss << ")";
        return dbg;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList lst = Counter::getAvailableEvents();

    Analysis* an= new Analysis();

    int avail = 0;
    //for each event in the list:
    for (QString name: lst) {
        quint64 val;
        Counter counter(name);
        //run the counter:
        if (counter.open()) {
            avail++;
            counter.enable();
            counter.disable();
        }
    }


    WorkloadCacheMisses* cache= new WorkloadCacheMisses();
    WorkloadInstructions* inst = new WorkloadInstructions();
    WorkloadPageFault* pf = new WorkloadPageFault();

    CreateFile* cf = new CreateFile();

    //write the csv:
    QFile file("troubleWorkload.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << " " << "\n";
        file.close();
    }
    return 0;
}

