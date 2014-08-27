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

typedef struct TRuch{
    TPole start;
    TPole koniec;
}TRuch;

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
    list<TRuch>::iterator it;
    list<TRuch> ruchy;
    list<TRuch> bicia;
    bool masz_bicie;
    int w;
    bool czy_zlapany;
    bool zmiana;
    TPole pole;
    TPole mouse_klik;
    //TGracz gracz1, gracz2;



public:
    TPlansza(QWidget *parent = 0);
    ~TPlansza();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *klik);
    void awans_damka();
    void wyznacz_ruchy(TPole pole);
    void wyznacz_bicia(TPole pole);
    bool przeglad_pola(int gracz);
    TPole wyznacz_poz_klikniecia(QMouseEvent *klik);
    void zamaluj_pola();
    void ruch();
    bool wykonaj_ruch(TPole start, TPole koniec);
    bool wykonaj_bicie(TPole start, TPole koniec);
    int gra();
    void wyczysc_zaznaczenia();

};




#endif // TPLANSZA_H
