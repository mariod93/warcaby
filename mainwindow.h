#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tplansza.h"
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

/*!
 * \class MainWindow
 * \brief Klasa modelująca głowne okno aplikacji
 * Klasa modeluje główne okno aplikacji, w którym zawarta jest plansza do gry oraz interfejs użytkownika. */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy MainWindow
     * Konstruktor inicjalizuje interfejs uzytkownika
     * \param parent - wskaznik na rodzica
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
     * \brief Dekonstruktor klasy MainWindow */
    ~MainWindow();

private:
    /*! \brief interfejs użytkownika */
    Ui::MainWindow *ui;
    /*! \brief wskaznik na obiekt planszy do gry */
    TPlansza *plansza;

public slots:

    /*!
     * \brief Metoda, która informuje użytkownika o stanie gry.
     * Metoda informuje, który gracz ma wykonać ruch oraz czy gra została zakończona */
    void wyswietl();

    /*!
     * \brief Metoda służąca do uruchomiena partii od nowa
     * Metoda wywołuje metodę klasy TPlansza, która resetuje swoje zmienne do wartości początkowych. */
    void nowa_gra();
};

#endif // MAINWINDOW_H
