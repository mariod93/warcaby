#include "mainwindow.h"
#include "ui_mainwindow.h"
#define OKNO_X 900
#define OKNO_Y 650

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    plansza = new TPlansza(this);
    plansza->setGeometry(100,100,60*8,60*8);
    setFixedSize(OKNO_X, OKNO_Y);
}

MainWindow::~MainWindow()
{
    delete ui;
}
