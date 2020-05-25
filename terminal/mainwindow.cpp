/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include <QTextStream>
#include "settingsdialog.h"
#include "dialog.h"
#include <QDataStream>
#include <iostream>
#include <QTimer>

#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QTextCodec>
#include <QColor>
//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    //m_console(new Console),
    m_settings(new SettingsDialog),
    m_dia (new Dialog),
//! [1]
    m_serial(new QSerialPort(this))
//! [1]
{
//! [0]
    m_ui->setupUi(this);
    //m_console->setEnabled(false);
    //setCentralWidget(m_console);

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->Plain_Receive->setReadOnly(true);
    m_ui->actionQuit->setEnabled(true);
    m_ui->actionConfigure->setEnabled(true);
    m_ui->OpenFile->setEnabled(false);
    m_ui->SendFile->setEnabled(false);
    m_ui->SendByte->setEnabled(false);
    m_ui->pushButton->setEnabled(false);
    m_ui->statusBar->addWidget(m_status);
    initActionsConnections();

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

//! [2]
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
//! [2]
    //connect(m_plain, &Console::getData, this, &MainWindow::writeData);


//! [3]
}
//! [3]

MainWindow::~MainWindow()
{
    delete m_settings;
    delete m_ui;

}
 static QString a ; static QString b;
 static int push = 0;  char output;
 static quint32 RawData[65356]; // max 64KB
 static quint32 RawDataPack [32]; // send 128B
 static int sizeBuffer; static quint8 Buff;
 static QByteArray Array[65356];
 static QByteArray ArrayData[65356];
 static QByteArray ArraySmall[32];
 static QByteArray ReadData ;
//! [4]
void MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
       // m_console->setEnabled(true);
       // m_console->setLocalEchoEnabled(p.localEchoEnabled);
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);
        m_ui->OpenFile->setEnabled(true);
        m_ui->SendFile->setEnabled(true);
        m_ui->SendByte->setEnabled(true);
        m_ui->pushButton->setEnabled(true);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        showStatusMessage(tr("Open error"));
    }

}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    //m_console->setEnabled(false);
    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionConfigure->setEnabled(true);
    m_ui->OpenFile->setEnabled(false);
    m_ui->SendFile->setEnabled(false);
    m_ui->SendByte->setEnabled(false);
    m_ui->pushButton->setEnabled(false);
    showStatusMessage(tr("Disconnected"));
}
//! [5]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Terminal"),
                       tr("Tuan K61 \r\n"
                          "25/3/2020 \r\n"
                          "DHBKHN"));
}

//! [6]
//void MainWindow::writeData(const QByteArray &data)
//{
//   // m_serial->write(data);
//}

//! [6]

//! [7]
void MainWindow::readData()
{
    ReadData = m_serial->readAll();
   // m_console->putData(data);
//    QString datas = QString::fromStdString(data.toStdString());
//    a.append(datas+"\r\n");
//    m_ui->Plain_Receive->appendPlainText(datas);

}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(m_ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::show);
    connect(m_ui->actionClear, &QAction::triggered, m_ui->Plain_Receive, &QPlainTextEdit::clear);
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(m_ui->actionOpen, &QAction::triggered, m_dia, &Dialog::show);
    QPixmap pic("C:/Users/admin/Desktop/New folder/a.png");
    m_ui->LabelStatus->setPixmap(pic);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_SendData_clicked()
{
    QString text = m_ui->Plain_Send->toPlainText();
    QByteArray str = text.toLatin1();
    const char *str2= str.data();
    m_serial->write(str2);
    m_ui->Plain_Send->clear();
    m_ui->Plain_Receive->setPlainText(text);
}

void MainWindow::on_OpenFile_clicked()
{
    QString filter = "All File (*) ;; Text File (txt) ;; Source File (c) ;; Header File (h)";
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file","C://");
    QFile file(file_name);
    sizeBuffer = file.size();
    m_ui->textLink->setPlainText(file_name);
    m_ui->textSize->setPlainText("Size: "+QString::number(sizeBuffer)+" Bytes");
    file.close();
}

void MainWindow::on_SendFile_clicked()
{
    QFile file("C:/Users/admin/Desktop/FileArray.txt");
    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QDataStream out (&file);
//    for(int i=0;i<128;i++)
//    {
           out<<ReadData;
//    }
    out.setVersion(QDataStream::Qt_5_14);
//    QTextStream out (&file);
//    out<<m_ui->Plain_Receive->toPlainText();
    file.flush();
    file.close();
}

void MainWindow::on_actionClear_triggered()
{
      a.clear();
      b.clear();
}

void MainWindow::on_SendByte_clicked()
{
    push++;
        for(int i =0;i<32;i++)
        {
            RawDataPack[i]=RawData[32*(push-1)+i];
            m_ui->Plain_Receive->appendPlainText("Byte : "+QString::number(128*(push-1)+4*i,10)+" :  "+QString::number(RawDataPack[i],16));
        }
        *ArraySmall = Array->mid(128*(push-1),128);
         m_serial->write(*ArraySmall);
    int packet_number = sizeBuffer/(4*32)+1;
    if(push==packet_number)
    {
        push=0;
        QMessageBox::information(this,"Send Status","Done !!!");
    }
    QFile file("C:/Users/admin/Desktop/ReadFile.txt");
    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QDataStream out (&file);
    for(int i=0;i<sizeBuffer/4;i++)
    {
        out << RawData[i];
    }
    out.setVersion(QDataStream::Qt_5_14);
    file.flush();
    file.close();
}


void MainWindow::on_pushButton_clicked()
{
    QString file_name = m_ui->textLink->toPlainText();
    QFile file(file_name);
    QDataStream in(&file);
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    else
       {
    QDataStream a(Array,QIODevice::ReadWrite);
    a.setVersion(QDataStream::Qt_5_14);
    for(int i=0;i<sizeBuffer/4;i++)
    {
         in>>RawData[i];
         a<<RawData[i];
    }
    //m_ui->Plain_Receive->appendPlainText(ArrayData);
    //m_ui->Plain_Send->appendPlainText(QString::number(ArrayData[0],16));
    in.setVersion(QDataStream::Qt_5_14);
    file.close();
    for( push=0; push<sizeBuffer/(4*32)+1;push++)
    {
        for(int i =0;i<32;i++)
        {
            RawDataPack[i]=RawData[32*(push)+i];
            m_ui->Plain_Receive->appendPlainText("Byte : "+QString::number(128*(push)+4*i,10)+" :  "+QString::number(RawDataPack[i],16));

        }
        *ArraySmall = Array->mid(128*(push),128);
         m_serial->write(*ArraySmall);
         int packet_number = sizeBuffer/(4*32)+1;
        if(push==packet_number-1)
           {
           //push=0;
            QMessageBox::information(this,"Send Status","Done !!!");
           }
         QFile file2("C:/Users/admin/Desktop/ReadFile.txt");
        if(!file2.open(QFile::WriteOnly))
        {
         QMessageBox::warning(this,"title","file not open");
        }
        QDataStream out (&file2);
        for(int i=0;i<sizeBuffer/4;i++)
        {
            out << RawData[i];
        }
        out.setVersion(QDataStream::Qt_5_14);
        file.flush();
        file.close();
      }
    }
}
