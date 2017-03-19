#ifndef QTOPTIONS_H
#define QTOPTIONS_H
#include "qtcomputer.h"
#include<QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>

class QtComputer;

/*!
 * Classe pour la fenetre d'options
 */

class OptionsWindow : public QWidget {
    Q_OBJECT;
private:
    QPushButton* test;
    QLabel* texte;
    QLabel* texte2;
    QRadioButton* oui;
    QRadioButton* non;
    QVBoxLayout *couche1;
    QPushButton* quitter;
    QtComputer* main;
    QSpinBox* num;
    QHBoxLayout *coucheH1;
public:
    OptionsWindow(QtComputer* m);
public slots:
    void changePile(int nb);
    void showKeyboard();
    void disableKeyboard();

};


#endif // QTOPTIONS_H
