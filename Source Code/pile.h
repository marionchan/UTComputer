#ifndef PILE_H
#define PILE_H
#include "litterale.h"
#include <QStack>
#include <QVector>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QObject>
#include <algorithm>



/*!
 * Classe qui réprésente la pile de littérales du programme
 */
class Pile : public QObject {

    Q_OBJECT;

private:

        QStack<Litterale*> p;
        unsigned int nbAffiche;
public :
        Pile() : nbAffiche(4) {}/*!< Constructeur de la pile : le nombre de littérales à afficher est initialisé à 4 arbitrairement */
        QVector<Litterale*> stackToVector(unsigned int n) const; /*!< Methode permettant de transformer la pile en stack afin d'y appliquer un itérator */
        void pop() {/*if(!p.empty()) */p.pop(); modificationPile();} /*!< Methode permettant de supprimer le dernier élément de la pile */
        Litterale* top() { return p.top();} /*!< Méthode qui renvoie le dernière élément de la pile */
        bool empty() {return p.isEmpty();}; /*!< Méthode booléenne qui renvoie vrai si la pile est vide */
        unsigned int size() {return p.size();} /*!< Méthode renvoyant la taile de la pile */
        void push(Litterale* l); /*!< Méthode permettant d'ajouter la littérale passée en argument à la pile */
        unsigned int getNbAffiche() const {return nbAffiche;}/*!< Accesseur en lecture à l'attribut NbAffiche */
        void setNbAffiche(unsigned int i) {nbAffiche=i;} /*!< Accesseur en écriture à l'attribut NbAffiche */
signals:
    void modificationPile();

};

/*!
 * Classe permettant de gérer les exceptions du programme
 */
class UTComputerException {

    QString info;
public:
    UTComputerException(const QString& i="") throw():info(i){}; /*!< Constructeur de la classe, permet d'initialiser l'attribut avec la valeur passée en argument */
    const QString& getException() const throw() {return info;} /*!< Méthode permettant de récupérer l'exception envoyée */
};


#endif // PILE_H
