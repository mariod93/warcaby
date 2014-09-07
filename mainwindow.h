#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tplansza.h"
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void gra();


private:


    Ui::MainWindow *ui;
    TPlansza *plansza;


public slots:
     void wyswietl();
     void nowa_gra();

};

#endif // MAINWINDOW_H
