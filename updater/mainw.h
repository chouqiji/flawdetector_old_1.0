#ifndef MAINW_H
#define MAINW_H

#include <QWidget>

namespace Ui {
class mainw;
}

class mainw : public QWidget
{
    Q_OBJECT

public:
    explicit mainw(QWidget *parent = 0);
    bool delDir(const QString &path);
    ~mainw();

protected:
    virtual void keyPressEvent(QKeyEvent *ke);
public slots:
    void cleanExit();
private slots:
    void on_confirm_clicked();

    void on_cancel_clicked();

private:
    void doUpdate();
    Ui::mainw *ui;
    QString verold;
    int veroldcmp;
    QString vernew;
    int vernewcmp;
};

#endif // MAINW_H
