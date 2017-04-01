#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <thread.h>
#include <iostream>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Thread *Class = new Thread();
    QThread *thread = new QThread();
    Class->moveToThread(thread);
//connect

    connect(this,SIGNAL(startStabilize(QString,QString,int,bool,bool)),
            Class,SLOT(Stabilize(QString,QString,int,bool,bool)));

    connect(Class,SIGNAL(restart()),this,SLOT(restart()));


    thread->start();
    ui->comboBox_FPS->addItem("25");
    ui->comboBox_FPS->addItem("30");
    ui->comboBox_FPS->addItem("60");
    ui->comboBox_FPS->addItem("120");
    ui->lineEdit_input->setReadOnly(true);



}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_clicked()
{


    bool pass_stab,quietMode;
    int fps = ui->comboBox_FPS->currentText().toInt();
    if ((ui->lineEdit_input->text()=="")&&(inputPath==""))
    {

        QMessageBox msgBox;
          msgBox.setText("Не выбрано видео!");
          msgBox.exec();
         return;

    }




    QString temp = inputPath.right(inputPath.length() -inputPath.lastIndexOf('/')-1);
    temp = temp.left(temp.indexOf("."))+"_stabilized."+temp.right(temp.length()- temp.lastIndexOf(".")-1);


    outputPath =  temp;



    if (ui->Ch_Play_video->isChecked())
        quietMode = false;
    else
        quietMode = true;

    if (ui->Ch_TwoPassStable->isChecked())
        pass_stab = true;
    else
        pass_stab=false;



    ui->pushButton->setEnabled(false);
    startStabilize(inputPath,outputPath,fps,pass_stab,quietMode);

    QMessageBox msgBox2;
      msgBox2.setText("Пожалуйста - подождите конца программы, появится вспылвающее окно");
      msgBox2.exec();
}

void MainWindow::on_pushButton_input_clicked()
{
    inputPath = QFileDialog::getOpenFileName(this,
                                                 QString::fromUtf8("Открыть файл"),
                                                 QDir::currentPath(),
                                                 "Video Files (*.avi *.mp4)");
    ui->lineEdit_input->setText(inputPath);

}



void MainWindow::restart()
{

     QMessageBox msgBox;
     msgBox.setText("Готово!  ");
     msgBox.exec();
     ui->pushButton->setEnabled(true);
}
