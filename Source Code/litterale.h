#ifndef LITTERALE_H
#define LITTERALE_H
#include "pile.h"
#include <iostream>
#include <string>
#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QChar>
#include <regex>


int pgcd(int a, int b); /*!< Fonction permettant de calculer le PGCD de deux entiers */

/*!
 * Classe abstraite pour les litterales
 */

class Litterale {
public:
    virtual QString afficher() =0; /*!< Fonction virtuelle pure qui permet d'afficher la litterale sous la forme d'un string */
    static bool isLitterale(const QString& s); /*!< Fonction booléenne qui renvoie true si le string passé en parametre est une littérale, sinon renvoie false */
    static bool isLitteraleToPush(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale qu'il faut push directement dans la pile sans traitement préalable nécessaire, sinon renvoie false */
    virtual ~Litterale(){}; /*!< destructeur de la classe Litterale, il est virtuel car nous sommes dans un cas de polymorphisme */
};

/*!
 * Classe abstraite pour les litterales numériques
 */

class LittNumerique : public Litterale{
public:
    virtual void neg()=0;
    static bool isLittNumerique(const QString& s);/*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale numérique, sinon renvoie false */
    virtual double getRealValue()=0;

};

/*!
 * Classe pour les litterales entières
 */

class LittEntiere : public LittNumerique {
    int value;
public :
    QString afficher(){return QString::number(value);} /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale entière sous la forme d'un string */
    LittEntiere(int v) : value(v) {} /*!< Constructeur de littérale entière qui initialise la valeur de l'attribut value avec la valeur passée en parametre */
    void neg(){value=-value;} /*!< Méthode qui permet de changer la valeur de l'attribut value en sa valeur négative */
    void setValue(int v){value=v;} /*!< Accesseur en écriture qui permet de modifier la valeur de l'attribut value avec la valeur passée en parametre*/
    const int& getValue () const {return value;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut value */
    static bool isLittEntiere(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale entière, sinon renvoie false */
    double getRealValue(); /*!< Methode qui permet d'obtenir la valeur réelle pour les operateurs logiques */

};

/*!
 * Classe pour les litterales rationnelles
 */

class LittRationnelle : public LittNumerique {
    LittEntiere numerateur;
    LittEntiere denominateur;
public :
   void neg() {numerateur.neg();} /*!< Méthode qui permet de changer la valeur des attributs en leurs valeurs négative */
   QString afficher(); /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale rationnelle sous la forme d'un string */
    LittRationnelle(LittEntiere n, LittEntiere d);

    void simplification(); /*!< Methode qui permet de simplifier la litterale rationnelle avant de l'ajouter à la pile */
    const LittEntiere getNumerateur()const { return numerateur;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut numérateur */
    const LittEntiere getDenominateur()const { return denominateur;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut dénominateur */
    double getRealValue(); /*!< Methode qui permet d'obtenir la valeur réelle pour les operateurs logiques */
    void setNumerateur(LittEntiere n) { numerateur=n;} /*!< Accesseur en écriture qui permet de modifier la valeur de l'attribut numérateur avec la valeur passée en parametre*/
    void setDenominateur(LittEntiere d) { denominateur=d;} /*!< Accesseur en écriture qui permet de modifier la valeur de l'attribut dénominateur avec la valeur passée en parametre*/
    static bool isLittRationnelle(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale rationnelle, sinon renvoie false */

};

/*!
 * Classe pour les litterales réelles
 */

class LittReelle : public LittNumerique {
    double value;
public:
    void neg() {value=-value;} /*!< Méthode qui permet de changer la valeur de l'attribut value en sa valeur négative */
    QString afficher(){return QString::number(value);}; /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale réelle sous la forme d'un string */
    LittReelle(double d) : value(d) {} /*!< Constructeur de littérale réelle qui initialise la valeur de l'attribut value avec la valeur passée en parametre */
    static bool isLittReelle(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale réelle, sinon renvoie false */
    const double& getValue () const {return value;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut value */
    double getRealValue(); /*!< Methode qui permet d'obtenir la valeur réelle pour les operateurs logiques */

};

/*!
 * Classe pour les litterales complexes
 */

class LittComplexe : public Litterale {
    LittNumerique* partieReelle;
    LittNumerique* partieIm;
public:
    virtual void neg() {partieReelle->neg(); partieIm->neg();} /*!< Méthode qui permet de changer la valeur des attributs en leurs valeurs négatives */
    QString afficher(){return partieReelle->afficher()+"$"+partieIm->afficher();}; /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale complexe sous la forme d'un string */
    LittComplexe(LittNumerique* r, LittNumerique*i) : partieReelle(r), partieIm(i) {} /*!< Constructeur de littérale complexe qui initialise la valeur des attributs avec les valeurs passées en parametre */
    static bool isLittComplexe(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale complexe, sinon renvoie false */
    LittNumerique* getRe() const {return partieReelle;} /*!< Accesseur en écriture de l'attribut partieReelle*/
    LittNumerique* getIm() const {return partieIm;}/*!< Accesseur en écriture de l'attribut partieIm*/
    bool isNull(); /*!< Methode qui permet de tester si le complexe est nul pour les operateurs logiques */
};

/*!
 * Classe pour les litterales expressions
 */

class LittExpression : public Litterale  {
    QString value;
public:
    QString afficher(){return value;}; /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale complexe sous la forme d'un string */
    LittExpression(const QString& s): value(s) {} /*!< Constructeur de littérale expression qui initialise la valeur de l'attribut value avec la valeur passée en parametre */
    const QString getValue () const {return value;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut value */
    static bool isLittExpression(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale expression, sinon renvoie false */

};

/*!
 * Classe pour les litterales programmes
 */

class LittProgramme : public Litterale {
    QString value;
public:
    static QMap<QString,QString> mapprogrammes;
    static QVector<LittProgramme*> programmes;
    QString afficher(){return value;}; /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale programme sous la forme d'un string */
    LittProgramme(const QString& s): value(s) {} /*!< Constructeur de littérale programme qui initialise la valeur de l'attribut value avec la valeur passée en parametre */
    const QString getValue () const {return value;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut value */
    static bool isLittProgramme(const QString& s);  /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale programme, sinon renvoie false */


};


/*!
 * Classe pour les litterales atomes
 */

class LittAtome : public Litterale {
    QString id;
    Litterale* value;
public:
    static QVector<LittAtome*> variables;
    static QMap<QString,Litterale*> mapvariables;
    QString afficher(){return id;}; /*!< Reimplémentation de la fonction virtuelle qui permet d'afficher la litterale atome sous la forme d'un string */
    LittAtome(const QString& s): id(s), value(nullptr) {} /*!< Constructeur de littérale atome qui initialise la valeur de l'attribut id avec la valeur passée en parametre et l'attribut value au pointeur nul */
    const QString getId () const {return id;} /*!< Accesseur en lecture qui permet d'acceder à la valeur de l'attribut id */
    static bool isLittAtome(const QString& s); /*!< Méthode statique booléenne qui renvoie true si le string passé en parametre est une littérale atome, sinon renvoie false */
    static bool isVariable(LittAtome* l); /*!< Méthode statique booléenne qui renvoie true si la litterale atome passé en parametre correspond à l'identificateur d'une variable, sinon renvoie false */
    static bool isProgramme(LittAtome* l); /*!< Méthode statique booléenne qui renvoie true si la litterale atome passé en parametre correspond à l'identificateur d'un programme, sinon renvoie false */

};

/*!
 * Classe qui permet de créer les  différentes littérales à partir d'un string
 */

class FactoryLitterale {

public:
    ~FactoryLitterale(); /*!< Destructeur de la FactoryLitterale : on désalloue les pointeurs Litterale de la pile */
    QVector<Litterale*> tab;
    QVector<QString> tlastop;
    QVector<Litterale*> tlastargs;
    //QVector<Litterale*> tundoargs;
    Litterale* makeLitt(const QString& s);/*!< Méthode qui renvoie un pointeur Litterale* à partir d'un string passé en parametre */
    Litterale* makeLittAtome(const QString& s); /*!< Méthode qui renvoie un pointeur Litterale* à partir d'un string passé en parametre */
    LittEntiere* createLittEntiere(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale entiere à partir d'un string passé en parametre */
    LittRationnelle* createLittRationnelle(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale rationnelle à partir d'un string passé en parametre */
    LittReelle* createLittReelle(const QString& s);/*!< Méthode qui renvoie un pointeur vers une litterale réelle à partir d'un string passé en parametre */
    LittComplexe* createLittComplexe(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale complexe à partir d'un string passé en parametre */
    LittAtome* createLittAtome(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale atome à partir d'un string passé en parametre */
    LittExpression* createLittExpression(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale expression à partir d'un string passé en parametre */
    LittProgramme* createLittProgramme(const QString& s); /*!< Méthode qui renvoie un pointeur vers une litterale programme à partir d'un string passé en parametre */


//makelitt est appelée et en suite en fonction du type on appelle les autres fonctions
};



#endif // LITTERALE_H
