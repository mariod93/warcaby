#include "mainwindow.h"
#include "tplansza.h"
#include "ui_mainwindow.h"
#define OKNO_X 900
#define OKNO_Y 650

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
     ui->setupUi(this);
     setFixedSize(OKNO_X, OKNO_Y);


     connect(ui->plansza,SIGNAL(klikniecie()), this, SLOT(wyswietl()));

     connect(ui->nowa_gra,SIGNAL(clicked()),this,SLOT(nowa_gra()));

     ui->z_tag->setVisible(false);
     ui->zwyciezca->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::wyswietl()
{
        if(ui->plansza->gracz() == 1)
            ui->gracz->setText("Czarne");
        else
            ui->gracz->setText("Białe");
        int wygrana = ui->plansza->czy_wygrana();
        //jesli ktos wygral
        if(wygrana != 0){

            if(wygrana == 1){

                ui->zwyciezca->setText("Czarne");}
            else
                ui->gracz->setText("Białe");

            ui->z_tag->setVisible(true);
            ui->zwyciezca->setVisible(true);
            ui->plansza->setVisible(false);
            update();

        }

}


void MainWindow::nowa_gra(){

    ui->plansza->nowa_gra();
    ui->plansza->setVisible(true);
    ui->z_tag->setVisible(false);
    ui->zwyciezca->setVisible(false);

    update();


}



