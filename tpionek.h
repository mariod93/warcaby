#ifndef TPIONEK_H
#define TPIONEK_H

#include <QtWidgets>
class TPionek
{
private:
    //wspolrzedne pionka na planszy 8x8
    unsigned x,y;
    QColor kolor;
public:
    TPionek();
    TPionek(int x_pos, int y_pos, QColor k);
    void ustaw_pozycje(int x_pos, int y_pos);
    unsigned int pozycja_x();
    unsigned int pozycja_y();
    QColor wez_kolor();


};

#endif // TPIONEK_H
