#include <algorithm>

#include "createfile.h"

CreateFile::CreateFile()
{
    str = QString("filename");
    QTemporaryFile file;
    if (file.open()) {
       QProcess * p = new QProcess();
       p->start("touch", QStringList() << "../others/a.txt");
       delete p;
    }
    else{
           qDebug("file not created");
    }
}

void CreateFile::create()
{
    qDebug("createFile");
}
