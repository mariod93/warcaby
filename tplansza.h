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
    int biale; /*! \var ilość białych pionków na planszy */
    int czarne; /*! \var ilość czarnych pionków na planszy */
    int akt; /*! \var aktualny gracz */
    int **pola; /*! \var tablica reprezentujaca stan planszy, co gdzie stoi */
    QColor **kolory_pol; /*! \var tablica okreslajaca kolory pol */
    QColor *tabKolor; /*! \var pomocnicza tablica z 2 kolorami, bialy i brazowy */
    list<TRuch>::iterator it; /*! \var iterator na listy bic i ruchow */
    list<TRuch> ruchy; /*! \var lista ruchow */
    list<TRuch> bicia; /*! \var lista bic */
    bool masz_bicie; /*! \var flaga okreslajaca czy gracz ma bicie */
    int w; /*! \var szerokosc jednego pola */
    bool brak_ruchow; /*! \var flaga okreslajaca czy dany gracz ma jakikolwiek ruch */
    bool czy_zlapany; /*! \var flaga okreslajaca czy zaznaczylem pionek */
    bool zmiana; /*! \var flaga okreslajaca czy zmienic kto aktualnie wykonuje ruch */
    bool koniec_ruchu; /*! \var flaga okreslajaca koniec ruchu */
    TPole pole; /*! \var startowe pole przy wykonywaniu ruchu */
    TPole mouse_klik; /*! \var koncowe pole przy wykonywaniu ruchu */
    TPole bicie; /*! \var zapamietuje pozycje startowa bicia */




public:
    TPlansza(QWidget *parent = 0);
    ~TPlansza();
    int** plansza();
    int& gracz();
    void paintEvent(QPaintEvent *);

    void awans_damka();
    void wyznacz_ruchy(TPole pole);
    void wyznacz_bicia(TPole pole);
    bool przeglad_pola(int gracz);
    TPole wyznacz_poz_klikniecia(QMouseEvent *klik);
    void zamaluj_pola();
    void ruch();
    /*!
     * \brief wykonaj_ruch
     * \param start
     * \param koniec
     * \return
     */
    bool wykonaj_ruch(TPole start, TPole koniec);
    bool wykonaj_bicie(TPole start, TPole koniec);
    void mousePressEvent(QMouseEvent *klik);
    void wyczysc_zaznaczenia();
    int czy_wygrana();
    bool czy_na_liscie(TPole, list<TRuch> lista);


signals:
    void klikniecie();

public slots:
      int gra();
      void nowa_gra();

};




#endif // TPLANSZA_H
