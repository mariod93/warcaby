#include "tplansza.h"
#define POLA 10
//*************************************************************************
TPlansza::TPlansza(QWidget *parent):
    QWidget(parent)
{
    w=60;
    czy_zlapany= false;
    masz_bicie=false;
    zmiana= true;
    koniec_ruchu=false;
    brak_ruchow=false;
    pole.x=0;
    pole.y=0;
    biale=czarne=12;
    akt=-1;

    /*
     *  Stan planszy reprezentowany jest tablica 10x10
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
    //alokacja pamieci na tablice kolorow
    kolory_pol = new QColor*[POLA];
    for(int i=0; i<POLA; i++){
        kolory_pol[i] = new QColor[POLA];
    }

    tabKolor = new QColor[2];
    tabKolor[0]=QColor(255,255,255);
    tabKolor[1]=QColor(60,30,0);

    //zainicjowanie pol kolorow
    for (int i=1; i<POLA-1; i++){
        for(int j=1; j<POLA-1; j++){
            //raz bialy, raz brazowy
            kolory_pol[j][i]=tabKolor[(i+j)%2];
        }
    }
    //dodanie czarnych pionkow
    for(int i=1; i<4; i++){
        for(int j=1; j<POLA-1; j+=2){
            pola[j+i%2][i]=1;
        }
    }
    //dodanie bialych pionkow
    for(int i=6; i<9; i++){
        for(int j=1; j<POLA-1; j+=2){
            pola[j+i%2][i]=-1;
        }
    }


    connect(this, SIGNAL(klikniecie()),this,SLOT(gra()));
}

//*************************************************************************
TPlansza::~TPlansza(){
    for(int i=0; i<POLA; i++){
        delete [] pola[i];
        delete [] kolory_pol[i];
    }
    delete [] pola;
    delete [] kolory_pol;
    delete [] tabKolor;
}

//*************************************************************************
//metoda dająca dostep do tablicy stanu planszy
int** TPlansza::plansza(){
    return pola;
}

//*************************************************************************
void TPlansza::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int a=w;//przesuwam poczatek planszy w prawo
    //rysowanie planszy
    for(int i=1; i<POLA-1; i++){
        for(int j=1; j<POLA-1; j++){
            painter.fillRect(a+w*(j-1), w*(i-1), w, w,kolory_pol[j][i]);
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
                painter.drawEllipse(a+w*(x-1)+5,w*(y-1)+5,w-10,w-10);
                break;

            case -1:
                painter.setBrush(Qt::white);
                painter.setPen(Qt::black);
                painter.drawEllipse(a+w*(x-1)+5,w*(y-1)+5,w-10,w-10);
                break;

            case 2:
                painter.setBrush(Qt::black);
                painter.setPen(Qt::white);
                painter.drawEllipse(a+w*(x-1)+5,w*(y-1)+5,w-30,w-30);
                break;

            case -2:
                painter.setBrush(Qt::white);
                painter.setPen(Qt::black);
                painter.drawEllipse(a+w*(x-1)+5,w*(y-1)+5,w-30,w-30);
                break;
            }
        }
    }

    //rysowanie zbitych pionkow
    for(int i=0; i<12-biale; i++){

        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        painter.drawEllipse(20,(w-25)*i+5,w-30,w-30);
    }
    for(int i=0; i<12-czarne; i++){
        painter.setBrush(Qt::black);
        painter.setPen(Qt::white);
        painter.drawEllipse(a+8*w+5,(w-25)*i+5,w-30,w-30);
    }
}

//*************************************************************************
//sprawdza czy ruch jest na liscie bic lub ruchow
bool TPlansza::czy_na_liscie(TPole pole, list<TRuch> lista){
    bool flaga=false;
    list<TRuch>::iterator iter;
    for(iter=lista.begin(); iter!=lista.end(); iter++){
        if(iter->start == pole){
            flaga=true;
            bicie=iter->start;
            break;
        }
    }
    return flaga;
}

//*************************************************************************
void TPlansza::mousePressEvent(QMouseEvent *klik){
    mouse_klik=wyznacz_poz_klikniecia(klik);
    klikniecie();
}

//*************************************************************************
int TPlansza::gra(){
    //warunek konca gry
    if(czy_wygrana() != 0){
        return czy_wygrana();}
    else{
        if(zmiana == true){
            zmiana=false;
            masz_bicie=przeglad_pola(akt);

            //jesli  klikniete pole jest poprawne
            if(pola[mouse_klik.x][mouse_klik.y] != 10){
                //jesli zaznaczylem pionek ktory ma bicie
                if( masz_bicie and czy_na_liscie(mouse_klik,bicia) ){
                    bicia.clear();
                    ruch(); // to wykonuje ruch
                }
                else{
                    ruch();
                }
            }
        }
        else
            ruch();
        awans_damka();
    }
    return 0;
}

//*************************************************************************
void TPlansza::ruch(){

    //gdy czy_zlapany == true to gracz zaznaczyl pionek, ktorym chce pojechac
    //kolejne zaznaczenie poprawnego pola spowoduje wykonanie ruchu

    switch(czy_zlapany){
    case false:

        //jesli zaznaczylismy swoj pionek
        if(pola[mouse_klik.x][mouse_klik.y] == akt or pola[mouse_klik.x][mouse_klik.y] == akt*2){
            czy_zlapany =true;
            ruchy.clear();
            bicia.clear();
            wyznacz_ruchy(mouse_klik);
            wyznacz_bicia(mouse_klik);

            // zapamietuje pozycje pionka, ktory zaznaczylem
            //pole to jest pozycja poczatkowa pionka ktorym chce wykonac ruch
            pole=mouse_klik;
            kolory_pol[mouse_klik.x][mouse_klik.y]=Qt::yellow;
            zamaluj_pola();

            //jesli pionkiem nie da sie ruszyc, anuluje ruch
            if(ruchy.empty() and bicia.empty()){
                kolory_pol[mouse_klik.x][mouse_klik.y]=tabKolor[(pole.x+pole.y)%2];
                czy_zlapany=false;
            }
            update();
        }
        break;

        //gdy gracz ma zaznaczony pionek
    case true:

        //gdy gracz zmieni zdanie i chce zagrac innym pionkiem
        if(pola[mouse_klik.x][mouse_klik.y] == akt or pola[mouse_klik.x][mouse_klik.y] == akt*2){

            kolory_pol[pole.x][pole.y]=tabKolor[(pole.x+pole.y)%2];
            wyczysc_zaznaczenia();
            ruchy.clear();
            bicia.clear();
            wyznacz_bicia(mouse_klik);
            wyznacz_ruchy(mouse_klik);

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
            if(masz_bicie and czy_zlapany == true){
                if(wykonaj_bicie(pole, mouse_klik)){
                    czy_zlapany = false;
                    zmiana=true;

                    wyczysc_zaznaczenia();
                    bicia.clear();
                    wyznacz_bicia(mouse_klik);
                    masz_bicie=!bicia.empty();
                    if(masz_bicie)
                        zmiana=false;
                }
                wyczysc_zaznaczenia();
            }
            else if (czy_zlapany ==true){
                if(wykonaj_ruch(pole,mouse_klik)){
                    zmiana=true;
                    czy_zlapany = false;
                    wyczysc_zaznaczenia();
                }
            }
            if(zmiana == true){

                if(akt == -1){
                    akt=1;
                }
                else{
                    akt=-1;
                }
            }
        }
        break;
    }
}

//*************************************************************************
int TPlansza::czy_wygrana(){
    int b=0,c=0;
    //zliczam pionki graczy
    for(int i=1; i<9; i++){
        for(int j=1; j<9; j++){
            if(pola[i][j] == 1 or pola[i][j] == 2)
                c++;
            if(pola[i][j] == -1 or pola[i][j] == -2)
                b++;
        }
    }
    biale=b;
    czarne=c;
    update();
    if(biale == 0 or (akt==-1 and brak_ruchow==true))
        return 1;
    else if(czarne == 0 or(akt==1 and brak_ruchow==true))
        return -1;
    else
        return 0;
}

//*************************************************************************
void TPlansza::wyznacz_ruchy(TPole pole){
    TPole pionek;
    TRuch ruch;
    //wspolrzedne pola startowego
    int x=pole.x;
    int y=pole.y;
    // zmienna okreslajaca czy sprawdzamy wiersz powyzej aktualnego, czy ponizej
    int cz;
    //ruchy pionka
    if(pola[x][y] == 1 or pola[x][y] == -1){
        if(pola[x][y] == 1){
            cz=1;
        }
        else if(pola[x][y] == -1){
            cz=-1;
        }
        //czy nie stoi pionek
        if(pola[x+1][y+cz] == 0){
            pionek.x=x+1;
            pionek.y=y+cz;
            ruch.start=pole;
            ruch.koniec=pionek;
            ruchy.push_back(ruch);
        }
        if(pola[x-1][y+cz] == 0){
            pionek.x=x-1;
            pionek.y=y+cz;
            ruch.start=pole;
            ruch.koniec=pionek;
            ruchy.push_back(ruch);
        }
    }
    //ruchy damki, 4 kierunki
    else if(pola[x][y] == 2 or pola[x][y] == -2){
        if(pola[x-1][y+1] == 0){
            int i=x-1;
            int j=y+1;
            //sprawdzam kolejne pola na przekatnej
            while(pola[i][j] == 0){
                pionek.x=i;
                pionek.y=j;
                ruch.start=pole;
                ruch.koniec=pionek;
                ruchy.push_back(ruch);
                i--;
                j++;
            }
        }
        if(pola[x-1][y-1] == 0){
            int i=x-1;
            int j=y-1;
            while(pola[i][j] == 0){
                pionek.x=i;
                pionek.y=j;
                ruch.start=pole;
                ruch.koniec=pionek;
                ruchy.push_back(ruch);
                i--;
                j--;
            }
        }
        if(pola[x+1][y+1] == 0){
            int i=x+1;
            int j=y+1;
            while(pola[i][j] == 0){
                pionek.x=i;
                pionek.y=j;
                ruch.start=pole;
                ruch.koniec=pionek;
                ruchy.push_back(ruch);
                i++;
                j++;
            }
        }
        if(pola[x+1][y-1] == 0){
            int i=x+1;
            int j=y-1;
            while(pola[i][j] == 0){
                pionek.x=i;
                pionek.y=j;
                ruch.start=pole;
                ruch.koniec=pionek;
                ruchy.push_back(ruch);
                i++;
                j--;
            }
        }
    }
}

//*************************************************************************
TPole TPlansza::wyznacz_poz_klikniecia(QMouseEvent *klik){
    TPole pole;
    //wyznaczam pole, nad ktorym kliknalem

    // pole.x = ( (klik->pos().x()-70) /w +1 );
    // pole.y = klik->pos().y() /w +1;
    pole.x = klik->pos().x()/w;
    pole.y = klik->pos().y()/w +1;
    return pole;
}

//*************************************************************************
void TPlansza::wyznacz_bicia(TPole pole){
    int przeciwnik;
    int x=pole.x;
    int y=pole.y;
    TPole pionek;
    TRuch ruch;
    if(pola[x][y] == 1 or pola[x][y] == 2){
        przeciwnik=-1;
    }
    else if(pola[x][y] == -1 or pola[x][y] == -2){
        przeciwnik=1;
    }

    //bicia pionka
    if(pola[x][y] == 1 or pola[x][y] == -1){
        for(int i=x-1; i<=x+1; i++){
            for(int j=y-1; j<=y+1; j++){
                //jesli jest przeciwnik
                if(pola[i][j] == przeciwnik or pola[i][j] == przeciwnik*2){
                    //to sprawdzam pole za nim
                    if(pola[i+(i-x)][j+(j-y)] == 0){
                        pionek.x=i+(i-x);
                        pionek.y=j+(j-y);
                        ruch.start=pole;
                        ruch.koniec=pionek;
                        bicia.push_back(ruch);
                    }
                }
            }
        }
    }

    //bicia damki
    if(pola[x][y] == 2 or pola[x][y] == -2){
        int i=x-1;
        int j=y+1;
        while(pola[i][j] == 0){
            i--;
            j++;
        }
        if(pola[i][j] == przeciwnik or pola[i][j] == przeciwnik*2){
            if(pola[i-1][j+1] == 0){
                i=i-1;
                j=j+1;
                while(pola[i][j] == 0){
                    pionek.x=i;
                    pionek.y=j;
                    ruch.start=pole;
                    ruch.koniec=pionek;
                    bicia.push_back(ruch);
                    i--;
                    j++;
                }
            }
        }

        i=x-1;
        j=y-1;
        while(pola[i][j] == 0){

            i--;
            j--;
        }
        if(pola[i][j] == przeciwnik or pola[i][j] == przeciwnik*2){


            if(pola[i-1][j-1] == 0){
                i=i-1;
                j=j-1;
                while(pola[i][j] == 0){
                    pionek.x=i;
                    pionek.y=j;
                    ruch.start=pole;
                    ruch.koniec=pionek;
                    bicia.push_back(ruch);
                    i--;
                    j--;
                }
            }
        }

        i=x+1;
        j=y+1;
        while(pola[i][j] == 0){

            i++;
            j++;
        }
        if(pola[i][j] == przeciwnik or pola[i][j] == przeciwnik*2){


            if(pola[i+1][j+1] == 0){
                i=i+1;
                j=j+1;
                while(pola[i][j] == 0){
                    pionek.x=i;
                    pionek.y=j;
                    ruch.start=pole;
                    ruch.koniec=pionek;
                    bicia.push_back(ruch);
                    i++;
                    j++;
                }
            }
        }

        i=x+1;
        j=y-1;
        while(pola[i][j] == 0){

            i++;
            j--;
        }
        if(pola[i][j] == przeciwnik or pola[i][j] == przeciwnik*2){


            if(pola[i+1][j-1] == 0){
                i=i+1;
                j=j-1;
                while(pola[i][j] == 0){
                    pionek.x=i;
                    pionek.y=j;
                    ruch.start=pole;
                    ruch.koniec=pionek;
                    bicia.push_back(ruch);
                    i++;
                    j--;
                }
            }
        }
    }
}

//*************************************************************************
//po kazdym ruchu sprawdzamy czy jest jakies bicie lub ruch
bool TPlansza::przeglad_pola(int gracz){
    TPole pole;
    for(int i=1; i<POLA-1; i++){
        for(int j=1; j<POLA-1; j++){
            if(pola[i][j] == gracz or pola[i][j]==gracz*2){
                pole.x=i;
                pole.y=j;
                wyznacz_bicia(pole);
                wyznacz_ruchy(pole);

            }
        }
    }
    if(ruchy.empty() and bicia.empty())
        brak_ruchow=true;
    ruchy.clear();
    return !bicia.empty(); //zwraca true jesli jest jakies bicie
}

//*************************************************************************
void TPlansza::zamaluj_pola(){
    if(bicia.empty()){
        for(it=ruchy.begin();it!=ruchy.end(); it++){
            kolory_pol[it->koniec.x][it->koniec.y]=Qt::red;
        }
    }
    else{
        for(it=bicia.begin();it!=bicia.end(); it++){
            kolory_pol[it->koniec.x][it->koniec.y]=Qt::red;
        }
    }
}

//*************************************************************************
bool TPlansza::wykonaj_bicie(TPole start, TPole koniec){
    bool a=false;// czy zaznaczylem poprawne pola
    kolory_pol[start.x][start.y]=tabKolor[(start.x+start.y)%2];
    for(it=bicia.begin(); it != bicia.end(); it++){
        if(it->koniec == koniec and it->start == start){
            a=true;
            break;//w tym momencie 'it' wskazuje na ruch ktory zaznaczylem
        }
    }
    if(a){
        if(it->koniec == koniec){
            int x,y;//okreslaja kierunek
            //ustalam w ktora strone bede sie poruszal
            if((koniec.x-start.x) >0)
                x=1;
            else
                x=-1;
            if((koniec.y-start.y) >0)
                y=1;
            else
                y=-1;
            //przestawiam pionek, ktory bil
            pola[koniec.x][koniec.y]=pola[start.x][start.y];
            pola[start.x][start.y]=0;
            //usuwam zbity pionek
            for(int i=1; i<abs(koniec.x-start.x); i++)
                //'i' okresla o ile sie przesuwamy w danym kierunku
                pola[start.x+x*i][start.y+y*i]=0;

            update();
        }
    }
    return a;
}

//*************************************************************************
//zwraca true jesli ruch zostal poprawnie wykonany
bool TPlansza::wykonaj_ruch(TPole start, TPole koniec){
    bool a=false;
    kolory_pol[start.x][start.y]=tabKolor[(start.x+start.y)%2];
    for(it=ruchy.begin(); it != ruchy.end(); it++){
        if(it->koniec == koniec and it->start == start){
            pola[koniec.x][koniec.y]=pola[start.x][start.y];
            pola[start.x][start.y]=0;
            czy_zlapany = false;
            a=true;
            update();
        }
    }
    return a;
}

//*************************************************************************
bool operator == (TPole p1, TPole p2){
    return (p1.x == p2.x) and (p1.y == p2.y);
}

//*************************************************************************
TPole & TPole::operator = (TPole p){
    this->x=p.x;
    this->y=p.y;
    return *this;
}

//*************************************************************************
void TPlansza::wyczysc_zaznaczenia(){
    for(it=ruchy.begin();it!=ruchy.end(); it++)//kasuje podswietlenia mozliwych ruchow
        kolory_pol[it->koniec.x][it->koniec.y]=tabKolor[(it->koniec.x+it->koniec.y)%2];

    for(it=bicia.begin();it!=bicia.end(); it++)//kasuje podswietlenia mozliwych bic
        kolory_pol[it->koniec.x][it->koniec.y]=tabKolor[(it->koniec.x+it->koniec.y)%2];
    ruchy.clear();
    bicia.clear();
}

//*************************************************************************
void TPlansza::awans_damka(){
    for(int i=1; i<9; i++){
        if(pola[i][1] == -1) // biala damka
            pola[i][1]=-2;
        if(pola[i][8] == 1)
            pola[i][8]=2;
    }
    update();
}

//*************************************************************************
int &TPlansza::gracz(){
    return akt;
}

//*************************************************************************
void TPlansza::nowa_gra(){
    akt=-1;
    czy_zlapany= false;
    masz_bicie=false;
    zmiana= true;
    koniec_ruchu=false;
    brak_ruchow=false;
    pole.x=0;
    pole.y=0;
    biale=czarne=12;
    akt=-1;

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
    update();
}
