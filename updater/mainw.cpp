#include "mainw.h"
#include "ui_mainw.h"
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QDebug>
#ifdef EXPORT_TO_BOARD
#define IPATH "/mnt/udisk/"
#define OPATH "/root/"
#else
#define IPATH "./"
#define OPATH "../"
#endif

mainw::mainw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainw)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QFile ver(OPATH"ver.bin");
    ver.open(QIODevice::ReadOnly);
    QDataStream v(&ver);
    v>>verold>>veroldcmp;
    ver.close();
    ui->edit->appendPlainText(tr("Ensure this device has enough power!!!"));
    ui->edit->appendPlainText(tr("Current installed version is:"));
    ui->edit->appendPlainText(verold);
}

mainw::~mainw()
{
    delete ui;
}

void mainw::keyPressEvent(QKeyEvent *ke)
{
    static int sw = 0xFFFFFFFF;
    switch(sw & ke->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        // not response to extra button
        sw = 0;
        ui->confirm->animateClick(100);
        break;
    case Qt::Key_Escape:
        ui->cancel->animateClick(100);
        break;
    default:
        break;
    }
}

bool mainw::delDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo file, fileList){
        if (file.isFile()){
            file.dir().remove(file.fileName());
        }else{
            delDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}

void mainw::cleanExit()
{
    QFile::remove(OPATH"update.tar");
    delDir(OPATH"update");
    system("sync");
    system("reboot");
    exit(0);
}

void mainw::doUpdate()
{
    // step 1: validate file
    ui->edit->appendPlainText(tr("Validating..."));
    if(QFile::exists(IPATH"update.img"))
    {

        QFile::copy(IPATH"update.img", OPATH"update.tar");
        system("tar xvf "OPATH"update.tar");
        // read version
        QFile ver(OPATH"update/ver.bin");
        ver.open(QIODevice::ReadOnly);
        QDataStream verds(&ver);
        verds>>vernew>>vernewcmp;
        ver.close();
        // show version
        ui->edit->appendPlainText(tr("Target version is:"));
        ui->edit->appendPlainText(vernew);
        if(vernewcmp <= veroldcmp)
        {
            ui->edit->appendPlainText(tr("Already the newest version! Exiting..."));
            QTimer::singleShot(2000,this,SLOT(cleanExit()));
            return;
        }
    }
    else
    {
        ui->edit->appendPlainText(tr("File not found, cleanning up & exiting..."));
        QTimer::singleShot(2000,this,SLOT(cleanExit()));
        return;
    }
    // step 2: kill && del && cp && sync
    ui->edit->appendPlainText(tr("Updating..."));
    system("killall FlawDetector");
    system("rm FlawDetector");
    QFile::copy(OPATH"update/FlawDetector",OPATH"FlawDetector");
    QFile::remove(OPATH"default.ini");
    system("sync");
    // step 3: execute additional script
    ui->edit->appendPlainText(tr("Executing extra task..."));
    system("sh "OPATH"update/update.sh");
    // step 4: reboot
    ui->edit->appendPlainText(tr("Update finished, rebooting..."));
    QTimer::singleShot(2000,this,SLOT(cleanExit()));
}

void mainw::on_confirm_clicked()
{
    doUpdate();
}

void mainw::on_cancel_clicked()
{
    this->close();
}
