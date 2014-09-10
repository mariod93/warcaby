#ifndef TPLANSZA_H
#define TPLANSZA_H

//#include "tgracz.h"
#include <list>
#include <QtWidgets>

using namespace std;

/*!
 * \struct TPole
 * \brief Struktura modelująca pojęcie pola na planszy. */
typedef struct TPole{
    /*! \brief współrzędna 'x'*/
    int x;
    /*! \brief współrzędna 'x'*/
    int y;
    /*! \brief  przeciążony operator przypisania dla klasy TPole
     * \param p - obiekt klasy Tpole, który zostanie przypisany
     * \return referencja do przypisanego obiektu */
    TPole & operator = (TPole p);
}TPole;

/*!
 * \brief Operator porównania dla klasy TPole.
 * \param p1,p2 - obiekty do porównania
 * \return true - jeśli współrzędne obu obiektów są takie same \n
 *         false - jeśli współrzędne nie są takie same */
bool operator == (TPole p1, TPole p2);

/*!
 * \struct TRuch
 * \brief Struktura modelująca pojęcie ruchu.
 * Ruch określany jest przez pozycje początkową i końcową. */
typedef struct TRuch{
    /*! \brief pozycja początkowa */
    TPole start;
    /*! \brief pozycja końcowa */
    TPole koniec;
}TRuch;


/*! \class TPlansza
 * \brief  Klasa modelująca pojęcie planszy do gry w warcaby.
 *
 *  Stan planszy do gry (położenie pionków) reprezentowany jest jako tablica 10x10, gdzie skrajne indeksy (0 oraz 9) \n
 * to ramka planszy. Ramka, podobnie jak białe pola, jest niedostępna dla pionków, ułatwia ona wyzaczanie \n
 * ruchów.
 *  Dodatkowo istnieje tablica, która przechowuje informacje o kolorze każdego pola.
 *
 * Każda komórka tablicy reprezentuje jedno pole
     * O tym czy coś stoi na polu decyduje wartość wpisana do komórki: \n
     *   0 - brak pionka, \n
     *   1 - pionek czarny, \n
     *   2 - damka czarna, \n
     *  -1 - pionek biały, \n
     *  -2 - damka biała. \n
     *  10 - pole zabronione \n
 *
 * Gracze reprezentowani są dwoma liczbami: \n
 *      gracz czarny : 1, \n
 *      gracz bialy: -1. \n
 */
class TPlansza: public QWidget
{
    Q_OBJECT
private:
    //ZMIENNE
    /*! \var biale
     * \brief ilość białych pionków na planszy */
    int biale;
    /*! \brief ilość czarnych pionków na planszy */
    int czarne;
    /*! \brief aktualny gracz */
    int akt;
    /*! \brief tablica reprezentujaca stan planszy, co gdzie stoi */
    int **pola;
    /*! \brief tablica okreslajaca kolory pol */
    QColor **kolory_pol;
    /*! \brief pomocnicza tablica z 2 kolorami, bialy i brazowy */
    QColor *tabKolor;
    /*! \brief iterator na listy bic i ruchow */
    list<TRuch>::iterator it;
    /*! \brief lista ruchow */
    list<TRuch> ruchy;
    /*! \brief lista bic */
    list<TRuch> bicia;
    /*! \brief flaga okreslajaca czy gracz ma bicie */
    bool masz_bicie;
    /*! \brief szerokosc jednego pola */
    int w;
    /*! \brief flaga okreslajaca czy dany gracz ma jakikolwiek ruch */
    bool brak_ruchow;
    /*! \brief flaga okreslajaca czy zaznaczylem pionek */
    bool czy_zlapany;
    /*! \brief flaga okreslajaca czy zmienic kto aktualnie wykonuje ruch */
    bool zmiana;
    /*! \brief flaga okreslajaca koniec ruchu */
    bool koniec_ruchu;
    /*! \brief startowe pole przy wykonywaniu ruchu */
    TPole pole;
    /*! \brief koncowe pole przy wykonywaniu ruchu */
    TPole mouse_klik;
    /*! \brief zapamietuje pozycje startowa bicia */

    TPole bicie;
    //METODY PRYWATNE
    /*!
     * \brief Metoda sprawdza, czy awansować pionek do damki.
     *
     */
    void awans_damka();
    /*!
     * \brief Metoda wyznacza wszystkie dozwolone ruchy danego pionka.
     * Ruchy zapisywane są na liście 'ruchy'. Lista ta służy do weryfikacji czy
     * ruch, który gracz chce wykonać jest poprawny.
     * \param pole - pole, dla którego wyznaczane są ruchy.
     */
    void wyznacz_ruchy(TPole pole);

    /*!
     * \brief Metoda wyznacza wszystkie dozwolone bicia danego pionka.
     * Bicia zapisywane są na liście 'bicia'. Lista ta służy do weryfikacji czy
     * bicie, które gracz chce wykonać jest poprawne.
     * \param pole - pole, dla którego wyznaczane są bicia.
     */
    void wyznacz_bicia(TPole pole);

    /*!
     * \brief Metoda sprawdzająca czy dany gracz ma jakieś bicie
     * Metoda po każdym ruchu sprawdza czy aktualny gracz ma jakieś bicie.
     * \param gracz - aktualny gracz
     * \return true - gracz ma jakieś bicie \n
     *         false - gracz nie ma żadnego bicia
     */
    bool przeglad_pola(int gracz);

    /*!
     * \brief Metoda wyznacza współrzedne pola nad którym gracz nacisnął przycisk myszy.
     * \param klik - zdarzenie kliknięcia
     * \return pole, nad którym został naciśnięty przycisk myszy
     */
    TPole wyznacz_poz_klikniecia(QMouseEvent *klik);

    /*!
     * \brief Metoda pomocnicza, służąca do zmiany kolorów pól,
     * na które można przenieść aktualnie zaznaczony pionek.
     */
    void zamaluj_pola();

    /*!
     * \brief Metoda odpowiada za wykonanie ruchu.
     * Ruch składa się z 2 faz:
     *  1. Zaznaczenie pionka,
     *  2. Zaznaczenie pola, na które pionek ma się przemieścić.
     *
     * Działanie metody jest zależne od faktu czy gracz zaznaczył jakiś pionek:
     *  -jeśli żaden pionek nie jest zaznaczony:
     *      *zaznacza kliknięty pionek, ustawia go aktywnym.
     *      *zmienia kolor pola zaznaczonego pionka na żółty.
     *  -jeśli jakiś pionek jest zaznaczony:
     *      *gdy gracz ma bicie, sprawdza czy pole docelowe to faktycznie ruch bicia.
     *
     *
     */
    void ruch();

    /*!
     * \brief Metoda modeluje wykonanie ruchu.
     * Ruch to przemieszczenie pionka bez bicia.
     * Gdy ruch zaznaczony przez gracza jest zgody z dozwolonymi ruchami - wykonuje ruch,
     * jeśli nie - nie zmienia stanu gry.
     * \param start - pole początkowe
     * \param koniec - pole docelowe
     * \return true - ruch został poprawnie wykonany \n
     *         false - ruch nie został wykonany
     */
    bool wykonaj_ruch(TPole start, TPole koniec);

    /*!
     * \brief Metoda modeluje wykonanie bicia.
     * Gdy bicie zaznaczone przez gracza jest zgode z istniejącymi biciami - wykonuje bicie,
     * jeśli nie - nie zmienia stanu gry.
     * \param start - pole początkowe
     * \param koniec - pole docelowe
     * \return true - bicie zostało poprawnie wykonane \n
     *         false - bicie nie zostało wykonane
     */
    bool wykonaj_bicie(TPole start, TPole koniec);

    /*!
     * \brief Metoda do obsługi przycisków myszki
     * Metoda wywołuje funkcje wyznaczającą pozycje kliknięcia.
     * Po każdym wystąpieniu kliknięcia, uruchamiana jest metoda gra(),
     * która steruje działaniem gry.
     * \param klik - zdarzenie myszki
     */
    void mousePressEvent(QMouseEvent *klik);

    /*!
     * \brief Metoda pomocnicza, która ustawia kolory wszystkich pól na domyślne (białe lub brązowe).
     *
     */
    void wyczysc_zaznaczenia();

    /*!
     * \brief Metoda sprawdza czy dane pole znajduje się na liście ruchów lub bić.
     * \param lista - lista, w której szukamy pola
     * \return true - pole jest na liście, \n
     *         false - pola nie ma.
     */
    bool czy_na_liscie(TPole, list<TRuch> lista);

public:
    /*!
     * \brief Konstruktor klasy TPlansza.
     *  Konstruktor inicjalizuje wszystkie zmienne.
     * \param parent - wskaźnik na rodzica.
     */
    TPlansza(QWidget *parent = 0);

    /*!
      \brief Dekonstruktor klasy TPlansza.
      */
    ~TPlansza();

    /*!
     * \brief Metoda dająca dostęp do tablicy stanu gry.
     * \return wskaźnik na tablice stanu gry.
     */
    int** plansza();

    /*!
     * \brief Metoda dająca dostęp do informacji o aktualnym graczu.
     * \return 1 - czarne, \n
     *        -1 - białe.
     */
    int& gracz();

    /*!
     * \brief Metoda obsługująca rysowanie planszy oraz pionków.
     * Metoda rysuje plansze, następnie, na podstawie tablicy stanu gry, umieszcza na niej pionki.
     * Zbite pionki rysowane są po bokach planszy.
     */
    void paintEvent(QPaintEvent *);

    /*!
     * \brief Metoda sprawdza warunek końca gry.
     * Metoda aktualizuje ilość pionków każdego z graczy oraz sprawdza warunek końca gry.
     * Koniec gry następuje gdy któryś z graczy straci wszystkie pionki lub nie będzie miał możliwości ruchu.
     * \return 0 - gra toczy się dalej, \n
     *         1 - zwyciężyły czarne, \n
     *        -1 - zwyciężyły białe.
     */
    int czy_wygrana();

signals:

    /*!
     * \brief Sygnał, który wywołuje slot "gra()".
     * Sygnał jest generowany po każdym kliknięciu myszki.
     */
    void klikniecie();

public slots:

    /*!
       * \brief Metoda monitorująca stan rozgrywki.
       * Metoda: \n
       *    -sprawdza warunek wygranej \n
       *    -dokonuje przeglądu pola(czy jest jakieś bicie) \n
       *    -wywołuje metodę wykonującą ruch.
       * \return 0 - gra toczy się dalej, \n
       *         1 - wygrały czarne, \n
       *        -1 - wygrały białe.
       */
    int gra();

    /*!
     * \brief Metoda uruchamiająca nową grę.
     * Metoda resetuje wszystkie zmienne do wartości początkowych.
     * Ustawia stan planszy na ten z początku gry.
     */
    void nowa_gra();
};

#endif // TPLANSZA_H
