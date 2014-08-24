#ifndef TPLANSZA_H
#define TPLANSZA_H

//#include "tgracz.h"
#include <list>
#include <QtWidgets>

using namespace std;

typedef struct TPole{
    int x;
    int y;
    TPole & operator = (TPole p);
}TPole;

bool operator == (TPole p1, TPole p2);

class TPlansza: public QWidget
{
    Q_OBJECT
private:
    int biale,czarne;
    int akt;
    int **pola;
    QColor **kolory_pol;
    QColor *tabKolor;
    list<TPole>::iterator it;
    list<TPole> ruchy;
    TPole bicie;
    list<TPole> bicia;
    bool masz_bicie;
    int w;
    bool czy_zlapany;
    TPole pole;
    TPole mouse_klik;
    //TGracz gracz1, gracz2;



public:
    TPlansza(QWidget *parent = 0);
    ~TPlansza();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *klik);
    void wyznacz_ruchy(int x, int y);
    void wyznacz_bicia(TPole pole);
    bool przeglad_pola(int gracz);
    TPole wyznacz_poz_klikniecia(QMouseEvent *klik);
    void zamaluj_pola();
    void ruch();
    void wykonaj_ruch(TPole start, TPole koniec);
    void wykonaj_bicie(TPole start, TPole koniec);
    void gra();

};




#endif // TPLANSZA_H
