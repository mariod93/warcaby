#include "tplansza.h"
#define POLA 10


TPlansza::TPlansza(QWidget *parent):
    QWidget(parent)
{
    w=60;
    czy_zlapany= false;
    masz_bicie=false;
    pole.x=0;
    pole.y=0;
    biale=czarne=12;
    akt=-1;

    /*
     *  Stan planszy reprezentowany jest tablica 8x8
     * Każda komorka reprezentuje jedno pole
     * o tym czy cos stoi na polu decyduje wartosc wpisana do komorki:
     *   0 - brak pionka,
     *   1 - pionek czarny,
     *   2 - damka czarna,
     *  -1 - pionek bialy,
     *  -2 - damka biala.
     *  10 - pole zabronione
     */
    //alokacja pamieci na tablice stanu planszy
    pola = new int*[POLA];
    for(int i=0; i<POLA; i++){
        pola[i] = new int[POLA];
    }
    //ramka planszy
    for(int i=0; i<POLA; i++){
        pola[0][i]=10;
        pola[i][0]=10;
        pola[POLA-1][i]=10;
        pola[i][POLA-1]=10;
    }
    //zainicjowanie wszystkich pol zerami
    for (int i=1; i<POLA-1; i++){
        for(int j=1; j<POLA-1; j++){
            if((i+j)%2 == 1)
                pola[i][j]=0;
            else
                pola[i][j]=10;
        }
    }
    //zainicjowanie wszystkich pol zerami
    kolory_pol = new QColor*[POLA];
    for(int i=0; i<POLA; i++){
        kolory_pol[i] = new QColor[POLA];
    }

    tabKolor = new QColor[2];
    tabKolor[0]=QColor(255,255,255);
    tabKolor[1]=QColor(60,30,0);
    //zainicjowanie wszystkich pol zerami
    for (int i=1; i<POLA-1; i++){
        for(int j=1; j<POLA-1; j++){
            kolory_pol[j][i]=tabKolor[(i+j)%2];
        }
    }

    for(int i=1; i<4; i++){
        for(int j=1; j<POLA-1; j+=2){
            pola[j+i%2][i]=1;
        }
    }
    for(int i=6; i<9; i++){
        for(int j=1; j<POLA-1; j+=2){
            pola[j+i%2][i]=-1;
        }
    }
}

TPlansza::~TPlansza(){
    for(int i=0; i<POLA; i++){
        delete [] pola[i];
        delete [] kolory_pol[i];
    }
    delete [] pola;
    delete [] kolory_pol;
    delete [] tabKolor;

}



void TPlansza::paintEvent(QPaintEvent *){
    QPainter painter(this);

    //rysowanie planszy
    for(int i=1; i<POLA-1; i++){
            for(int j=1; j<POLA-1; j++){
                painter.fillRect(w*(j-1), w*(i-1), w, w,kolory_pol[j][i]);
            }
        }

    //rysowanie pionkow
    int pole;
    for(int y=1; y<POLA-1; y++){
        for(int x=1; x<POLA-1; x++){
            pole=pola[x][y];
            switch(pole){
                case 1:
                painter.setBrush(Qt::black);
                painter.setPen(Qt::white);
                painter.drawEllipse(w*(x-1)+5,w*(y-1)+5,w-10,w-10);
                break;

                case -1:
                painter.setBrush(Qt::white);
                painter.setPen(Qt::black);
                painter.drawEllipse(w*(x-1)+5,w*(y-1)+5,w-10,w-10);
                break;
            }
        }
    }
}

void TPlansza::mousePressEvent(QMouseEvent *klik){

    mouse_klik=wyznacz_poz_klikniecia(klik);

    if(biale == 0){
        qDebug()<<"wygraly czarne";
    }
    else if(czarne == 0){
        qDebug()<<"wygraly biale";
    }
    else
        ruch();
    qDebug() << akt << "'" << czy_zlapany;


}


void TPlansza::ruch(){
    bool a=true;
    //jesli zaznaczylismy poprawne pole(brazowe)
    if(pola[mouse_klik.x][mouse_klik.y] != 10){
        //oraz gdy zaznaczylismy swoj pionek
        przeglad_pola(akt);

        //gdy czy_zlapany == true to gracz zaznaczyl pionek, ktorym chce pojechac
        //kolejne zaznaczenie poprawnego pola spowoduje wykonanie ruchu

        switch(czy_zlapany){
        case false:


            if(a){
                //jesli zaznaczylismy pionek
                if(pola[mouse_klik.x][mouse_klik.y] == akt){
                    czy_zlapany =true;
                    wyznacz_ruchy(mouse_klik.x, mouse_klik.y);
                    wyznacz_bicia(mouse_klik);
                    pole=mouse_klik;

                    // zapamietuje pozycje pionka, ktory zaznaczylem
                    kolory_pol[mouse_klik.x][mouse_klik.y]=Qt::yellow;
                    zamaluj_pola();
                }
                //jesli pionkiem nie da sie ruszyc, anuluje ruch
                if(ruchy.empty() and bicia.empty()){
                    kolory_pol[mouse_klik.x][mouse_klik.y]=tabKolor[(pole.x+pole.y)%2];
                    czy_zlapany=false;
                }
                update();
            }
            break;

        case true:

            //gdy gracz zmieni zdanie i chce zagrac innym pionkiem
            if(pola[mouse_klik.x][mouse_klik.y] == akt){
                kolory_pol[pole.x][pole.y]=tabKolor[(pole.x+pole.y)%2];
                for(it=ruchy.begin(); it!=ruchy.end(); it++){//kasuje podswietlenia mozliwych ruchow
                    kolory_pol[it->x][it->y]=tabKolor[(it->x+it->y)%2];
                }
                for(it=bicia.begin(); it!=bicia.end(); it++){//kasuje podswietlenia mozliwych ruchow
                    kolory_pol[it->x][it->y]=tabKolor[(it->x+it->y)%2];
                }
                ruchy.clear();
                bicia.clear();
                wyznacz_bicia(mouse_klik);
                wyznacz_ruchy(mouse_klik.x, mouse_klik.y);

                pole=mouse_klik; // zapamietuje pozycje pionka, ktory zaznaczylem
                kolory_pol[mouse_klik.x][mouse_klik.y]=Qt::yellow;
                zamaluj_pola();

                //jesli pionkiem nie da sie ruszyc, anuluje ruch
                if(ruchy.empty() and bicia.empty()){
                    kolory_pol[mouse_klik.x][mouse_klik.y]=tabKolor[(pole.x+pole.y)%2];
                    czy_zlapany=false;
                }
                update();
            }

            //wykonaj ruch lub bicie
            //gdy jest bicie nalezy je wykonac
            else{
                if(!bicia.empty()){
                    wykonaj_bicie(pole, mouse_klik);
                    if(akt == -1){
                        czarne--;

                    }
                    else{
                        biale --;

                    }
                }
                else
                    wykonaj_ruch(pole,mouse_klik);
                if(czy_zlapany == false){


                    if(akt == -1){
                        akt=1;
                    }
                    else{
                        akt=-1;
                    }
                }

            }
            if(czy_zlapany == false){

                for(it=ruchy.begin();it!=ruchy.end(); it++)//kasuje podswietlenia mozliwych ruchow
                    kolory_pol[it->x][it->y]=tabKolor[(it->x+it->y)%2];

                for(it=bicia.begin();it!=bicia.end(); it++)//kasuje podswietlenia mozliwych bic
                    kolory_pol[it->x][it->y]=tabKolor[(it->x+it->y)%2];
                ruchy.clear();
                bicia.clear();
            }
            break;

        }

    }
}

void TPlansza::wyznacz_ruchy(int x, int y){
    TPole ruch;

    int cz;
    if(pola[x][y] == 1){
        cz=1;
    }
    else if(pola[x][y] == -1){
        cz=-1;
    }
        if(pola[x+1][y+cz] == 0){
           ruch.x=x+1;
           ruch.y=y+cz;
           ruchy.push_back(ruch);
        }
        if(pola[x-1][y+cz] == 0){
            ruch.x=x-1;
            ruch.y=y+cz;
            ruchy.push_back(ruch);
        }




}

TPole TPlansza::wyznacz_poz_klikniecia(QMouseEvent *klik){
    TPole pole;
    //wyznaczam pole, nad ktorym kliknalem
    pole.x = klik->pos().x() /w +1;
    pole.y = klik->pos().y() /w +1;
    return pole;
}


void TPlansza::wyznacz_bicia(TPole pole){
    int cz;
    int x=pole.x;
    int y=pole.y;
    TPole ruch;
    if(pola[x][y] == 1){
        cz=1;
    }
    else if(pola[x][y] == -1){
        cz=-1;
    }

    for(int i=x-1; i<=x+1; i++){
        for(int j=y-1; j<=y+1; j++){
            if(pola[i][j] == -cz){
                if(pola[i+(i-x)][j+(j-y)] == 0){
                    ruch.x=i+(i-x);
                    ruch.y=j+(j-y);
                    bicia.push_back(ruch);
                }
            }
        }
    }

}

bool TPlansza::przeglad_pola(int gracz){
    TPole pole;
    for(int i=1; i<POLA-1; i++){
        for(int j=0; j<POLA-1; j++){
            if(pola[i][j] == gracz){
                pole.x=i;
                pole.y=j;
                wyznacz_bicia(pole);
            }
        }
    }
}



void TPlansza::zamaluj_pola(){

    if(bicia.empty()){
        for(it=ruchy.begin();it!=ruchy.end(); it++){
            kolory_pol[it->x][it->y]=Qt::red;
         }
    }
    else{
        for(it=bicia.begin();it!=bicia.end(); it++){
            kolory_pol[it->x][it->y]=Qt::red;
         }
    }
}


void TPlansza::wykonaj_bicie(TPole start, TPole koniec){
    bool a=false;

    kolory_pol[start.x][start.y]=tabKolor[(start.x+start.y)%2];

       for(it=bicia.begin(); it != bicia.end(); it++){
           if(*it == koniec){
               a=true;
               break;
           }
       }
   if(a){
     if(*it == koniec){
         int x = (koniec.x-start.x)/2;
         int y = (koniec.y-start.y)/2;
         pola[koniec.x][koniec.y]=pola[start.x][start.y];
         pola[start.x][start.y]=0;
         //usuwam zbity pionek
         pola[start.x+x][start.y+y]=0;
         czy_zlapany = false;
         update();
         }
     }
}


void TPlansza::wykonaj_ruch(TPole start, TPole koniec){

    kolory_pol[start.x][start.y]=tabKolor[(start.x+start.y)%2];
    for(it=ruchy.begin(); it != ruchy.end(); it++){
        if(*it == koniec){
            pola[koniec.x][koniec.y]=pola[start.x][start.y];
            pola[start.x][start.y]=0;
            czy_zlapany = false;
            update();
        }
    }
}


bool operator == (TPole p1, TPole p2){
    return (p1.x == p2.x) and (p1.y == p2.y);
}

TPole & TPole::operator = (TPole p){
    this->x=p.x;
    this->y=p.y;
}

void TPlansza::gra(){

}
