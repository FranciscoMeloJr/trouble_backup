#include <algorithm>

#include "createfile.h"

CreateFile::CreateFile()
{
    str = QString("filename");
    QTemporaryFile file;
    if (file.open()) {
       QProcess process;
       process.start("touch", QStringList() << "/home/a.txt");

    }
    else{
           qDebug("Shape of my heart");
    }
}

void CreateFile::create()
{
    qDebug("x");
}
