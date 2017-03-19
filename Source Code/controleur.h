#ifndef CONTROLEUR_H
#define CONTROLEUR_H
#include <QStack>
#include <QVector>
#include <QTextStream>
#include <QObject>
#include <QString>
#include <iostream>
#include <ostream>
#include <istream>
#include <string>
#include <iterator>
#include <QDebug>
#include <QTextStream>
#include "pile.h"
#include "litterale.h"
#include "operateur.h"
#include <algorithm>

/*!
 * Classe permettant de gérer l'application
 */

class Controleur : public QObject {

    Q_OBJECT;

private:
    Pile& pile;
    FactoryLitterale factL;
    QString message;
    //unsigned int nbAffiche;
public :
    void setMessage(QString &m) {message=m;} /*!< Accesseur en écriture à l'attribut message */
    void afficher(); /*!< Méthode permettant d'afficher le programme en console */
    void commande(const QString& s); /*!< Méthode permettant le traitement d'une commande */
    void executer();/*!< Méthode permettant d'executer le programme en console */
    Controleur(Pile& p,FactoryLitterale& fl) : pile(p), factL(fl), message("Bienvenue"){} /*!< Constructeur de la class Controleur */
    void printPile(unsigned int n);/*!< Methode permettant d'afficher la pile du programme en console*/
    void applyOpEVAL();/*!< Méthode permettant d'appliquer l'opérateur EVAL */
    void execProg(QString s); /*!< Méthode permettant d'éxécuter un programme */
    const QString getMessage() const {return message;}/*!< Accesseur en lecture à l'attribut message */


};
#endif // CONTROLEUR_H
