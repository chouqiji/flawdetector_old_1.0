#include <QFile>
#include <QDataStream>

int main(int argc, char *argv[])
{
    qDebug(__DATE__ __TIME__);
    if(argc == 4)
    {
        QFile ver(argv[1]);
        ver.open(QIODevice::WriteOnly);
        QDataStream v(&ver);
        v<<QString(argv[2])<<QString(argv[3]).toInt();
        ver.close();
        ver.open(QIODevice::ReadOnly);
        QString version;
        int versionNum;
        v>>version>>versionNum;
        ver.close();
        qDebug(QString("ver:%1,vernum:%2,generated").arg(version).arg(versionNum).toLatin1());
        return 0;
    }
    else
    {
        qDebug("usage: vergen [outfile] [version] [version number for compare]");
    }
}
