#include "sendfile.h"
#include "ui_sendfile.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
SendFile::SendFile(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::SendFile)

SendFile::~SendFile()
{
    delete ui;
}
