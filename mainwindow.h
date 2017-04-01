#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <iostream>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString inputPath = "";
    QString outputPath = "";
signals:
    void startStabilize(QString inputPath,QString outputPath,int outputFps, bool Type_Pass_Stabilizer, bool quietMode );

private slots:
    void on_pushButton_clicked();

    void on_pushButton_input_clicked();


    void restart();

private:
    Ui::MainWindow *ui;
};




#endif // MAINWINDOW_H
