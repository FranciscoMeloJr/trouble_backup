#ifndef CREATEFILE
#define CREATEFILE

#include <QString>
#include <QTemporaryFile>
#include <QDebug>
#include <QProcess>

//This class creates temporary file:
class CreateFile
{
public:
    CreateFile();
    void create();

private:
    QString str;
};

#endif // CREATEFILE

