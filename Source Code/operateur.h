#ifndef OPERATEUR_H
#define OPERATEUR_H
#include <QString>
#include <QMap>
#include <iostream>
#include <QDebug>
#include "pile.h"
#include "litterale.h"
//penser à STO et FORGET

/*!
 * Classe pour les operateurs
 */

class Operateur {
public:
    static bool isOperateur(const QString& s); /*!< Fonction booléenne qui renvoie true si le string passé en parametre est un operateur, sinon renvoie false */
    virtual ~Operateur() {}
};

/*!
 * Classe abstraite pour les operateurs de pile et d'expressions
 */

class OpPile : public Operateur {
    static QVector<QString> vectOpPile;
public:
    //OpPile(const QString &s) : Operateur(s){};
    static bool isOpPile(const QString& s); /*!< Fonction booléenne qui renvoie true si le string passé en parametre est un operateur de pile, sinon renvoie false */
    static void applyOpPile(Pile &p,FactoryLitterale& fl,const QString& s); /*!< Méthode statique qui applique l'operateur designé par la variable s à la pile p */
    static int nbClear;
    //static QString undoOp;

};

/*!
 * Classe abstraite pour les operateurs unaires
 */

class OpUnaire : public Operateur {
    static QVector<QString> vectOpUnaire;
public:
    //OpUnaire(const QString &s) : Operateur(s){};
    static bool isOpUnaire(const QString& s); /*!< Fonction booléenne qui renvoie true si le string passé en parametre est un operateur unaire, sinon renvoie false */
    static void applyOpUnaire(Pile &p,FactoryLitterale& fl,const QString& s); /*!< Méthode statique qui applique l'operateur designé par la variable s à la dernière littérale de la pile p */

};

/*!
 * Classe abstraite pour les operateurs binaires
 */

class OpBinaire : public Operateur {
    static QVector<QString> vectOpBinaire;
public:
    //OpBinaire(const QString &s) : Operateur(s){};
    static bool isOpBinaire(const QString& s); /*!< Fonction booléenne qui renvoie true si le string passé en parametre est un operateur binaire, sinon renvoie false */
    static void applyOpBinaire(Pile &p,FactoryLitterale& fl,const QString& s); /*!< Méthode statique qui applique l'operateur designé par la variable s aux deux dernières littérales de la pile p */

};

#endif // OPERATEUR_H
