#ifndef QTCOMPUTER_H
#define QTCOMPUTER_H
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include "controleur.h"
#include "litterale.h"
#include "pile.h"
#include"qtvarwindow.h"
#include "qtprgwindow.h"
#include "qtoptions.h"
#include "operateur.h"
#include <QApplication>
#include <QLineEdit>
#include <QObject>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QInputDialog>
#include <QSpinBox>
#include <QWidget>
class PrgWindow;
class OptionsWindow;
class VarWindow;

/*!
 * Classe pour la fenetre principale
 */

class QtComputer : public QWidget{

    Q_OBJECT;

public:
    QtComputer();
    Pile* pile;
    friend class OptionsWindow;
private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;
    QPushButton *button8;
    QPushButton *button9;
    QPushButton *button0;
    QPushButton *buttonadd;
    QPushButton *buttonminus;
    QPushButton *buttondiv;
    QPushButton *buttonmult;
    QPushButton *m_bouton;
    QPushButton *enter;
    QPushButton *dup;
    QPushButton *backspace;
    QPushButton *dollar;
    QPushButton *clear;
    QPushButton *drop;
    QPushButton *sto;
    QPushButton *lastop;
    QPushButton *lastargs;
    QPushButton *undo;
    QPushButton *swap;

    QPushButton *forget;
    QTableWidget* vuePile;
    QWidget* keyboard;
    QWidget* stack;
    QVBoxLayout* coucheK;
    QVBoxLayout* coucheS;
    QLineEdit* message;
    QLineEdit* commande;
    QVBoxLayout* couche;
    QHBoxLayout* coucheH;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QHBoxLayout* coucheH5;
    Controleur* controleur;
    FactoryLitterale* fl;
    QPushButton* options;
    QPushButton* var;
    QPushButton* prg;
    OptionsWindow* w;
    VarWindow* w2;
    PrgWindow* w3;

public slots:
    void getNextCommande();
    void refresh();
    void b0();
    void b1();
    void b2();
    void b3();
    void b4();
    void b5();
    void b6();
    void b7();
    void b8();
    void b9();
    void badd();
    void bdiv();
    void bmult();
    void bminus();
    void dupslot();
    void dollarslot();
    void openOptionsWindow();
    void backspaceslot();
    void clearslot();
    void dropslot();
    void stoslot();
    void forgetslot();
    void openVarWindow();
    void openPrgWindow();
    void swapslot();
    void lastopslot();
    void lastargsslot();
    void undoslot();

};



#endif // QTCOMPUTER_H
