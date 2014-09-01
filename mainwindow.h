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
    QTcpSocket *socket;


private slots:
    void on_connect_clicked();
public slots:
    void pobierzDane();
     void wyswietl();
     void nowa_gra();
     void wyslijDane();
};

#endif // MAINWINDOW_H
