#ifndef MAINWINDOWFILE_H
#define MAINWINDOWFILE_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SendFile; }
QT_END_NAMESPACE

class SendFile : public QMainWindow
{
    Q_OBJECT

public:
    SendFile(QWidget *parent = nullptr);
    ~SendFile();

private:
    Ui::SendFile *ui;
};
#endif // MAINWINDOWFILE_H
