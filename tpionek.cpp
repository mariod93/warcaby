#include "tpionek.h"

TPionek::TPionek():
    kolor(Qt::black)
{
    x=y=0;
}

TPionek::TPionek(int x_pos, int y_pos, QColor k){
    x=x_pos;
    y=y_pos;
    kolor=k;
}

void TPionek::ustaw_pozycje(int x_pos, int y_pos){
    x=x_pos;
    y=y_pos;
}


unsigned int TPionek::pozycja_x(){
    return x;
}

unsigned int TPionek::pozycja_y(){
    return y;
}

QColor TPionek::wez_kolor(){
    return kolor;
}
