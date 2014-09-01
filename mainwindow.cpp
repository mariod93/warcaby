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
    //plansza = new TPlansza(this);
    //plansza->setGeometry(100,100,60*8,60*8);
    setFixedSize(OKNO_X, OKNO_Y);

    socket = new QTcpSocket(this);
    connect(ui->plansza,SIGNAL(klikniecie()), this, SLOT(wyswietl()));
    connect(ui->wyslij,SIGNAL(clicked()), this, SLOT(wyslijDane()));
    ui->z_tag->setVisible(false);
    ui->zwyciezca->setVisible(false);

    connect(ui->nowa_gra,SIGNAL(clicked()),this,SLOT(nowa_gra()));
    connect(ui->connect,SIGNAL(clicked()),this,SLOT(on_connect_clicked()));
    //connect(ui->pobierz,SIGNAL(clicked()),this,SLOT(pobierzDane()));

    connect(socket,SIGNAL(readyRead()),this,SLOT(pobierzDane()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    socket->abort();
    QHostAddress adress(ui->lineEditIP->text());
    socket->connectToHost(adress, ui->lineEditPort->text().toInt());

}

void MainWindow::wyswietl()
{
        if(ui->plansza->gracz() == 1)
            ui->gracz->setText("Czarne");
        else
            ui->gracz->setText("Białe");
        int wygrana = ui->plansza->czy_wygrana();
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
    qDebug() << "nowa gra";
    ui->plansza->nowa_gra();
    ui->plansza->setVisible(true);
    ui->z_tag->setVisible(false);
    ui->zwyciezca->setVisible(false);

    update();

\
}


void MainWindow::wyslijDane(){

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    /*out <<ui->plansza->gracz();

    int **wsk = ui->plansza->plansza();
    for(int i=0; i<10;i++){
        for(int j=0; j<10;j++){
            out  <<wsk[i][j] ;

        }
    }*/
    char dane[20]="dane";
    socket->write(dane);


    /*for(int i=1; i<9;i++){
        for(int j=1; j<9;j++){
            out << " " <<wsk[i][j] ;
            qDebug()<<wsk[i][j];
        }
    }*/
}

void MainWindow::pobierzDane(){
    QTextStream in(socket);
    int **wsk = ui->plansza->plansza();

    in >> ui->plansza->gracz();
    for(int i=0; i<10;i++){
        for(int j=0; j<10;j++){
            in >> wsk[i][j];
            qDebug()<<ui->plansza->plansza()[i][j];
        }
    }

}
