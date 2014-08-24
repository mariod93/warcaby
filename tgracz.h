#ifndef TGRACZ_H
#define TGRACZ_H
#include <QtWidgets>
class TGracz
{
private:
    int **pionki; //przechowuje informacje o wspolrzednych pionkow gracza
    int id; //identyfikator gracz
            //1-czarne
            //2-biale

public:
    TGracz();
    TGracz(int numer);
    ~TGracz();
    int** Pionki();
    int wez_id();
    void nadaj_id(int numer);

};

#endif // TGRACZ_H
