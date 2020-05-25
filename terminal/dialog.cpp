#include "dialog.h"
#include "ui_dialog.h"
#include "settingsdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QSerialPortInfo>
#include <QSerialPort>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this))


{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {


    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());


    }
}
void Dialog::on_pushButton_clicked()
{
    Dialog::openSerialPort();
    if(m_serial->open(QIODevice::ReadWrite))
    {
    QString filter = "All File (*) ;; Text File (txt) ;; Source File (c) ;; Header File (h)";
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file","C://");
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in (&file);
    QString text =in.readAll();
    ui->plainTextEdit->setPlainText(text);

    file.close();
    }
    else
    {
        QMessageBox::warning(this,"title","open Serial Port");
    }
}

void Dialog::on_pushButton_2_clicked()
{
    Dialog::openSerialPort();
    QFile file("C:/Users/admin/Documents/QFile/EditFile.txt");
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream out (&file);
    QString text = ui->plainTextEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void Dialog::on_pushButton_3_clicked()
{
    Dialog::openSerialPort();
    if(m_serial->open(QIODevice::ReadWrite))
        {
        QString filter = "All File (*) ;; Text File (txt) ;; Source File (c) ;; Header File (h)";
        QString file_name = QFileDialog::getOpenFileName(this,"Open a file","C://");
        QFile file(file_name);
        if(!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this,"title","file not open");
        }
        QTextStream in (&file);
        QString text =in.readAll();
        ui->plainTextEdit->setPlainText(text);

        file.close();
        }
        else
        {
            QMessageBox::warning(this,"title","open Serial Port");
        }
}
