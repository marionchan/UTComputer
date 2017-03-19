#include "pile.h"
#include "litterale.h"


//-------------------------------------DECLARATIONS DES MAPS ET VECTORS -----------------------------------------------
QVector<LittAtome*> LittAtome::variables;
QMap<QString,Litterale*> LittAtome::mapvariables{{"X1", new LittEntiere(1)}};
QMap<QString,QString> LittProgramme::mapprogrammes{{"MONPROG", "[36 X2 STO]"}};
//QVector<Litterale*> FactoryLitterale::tab;

//-------------------------------------LITTERALE-----------------------------------------------

bool Litterale::isLitterale(const QString& s) {
    if (LittNumerique::isLittNumerique(s)||LittComplexe::isLittComplexe(s)||LittAtome::isLittAtome(s)||LittProgramme::isLittProgramme(s)||LittExpression::isLittExpression(s)){
        return true;
    }
    return false;
}
bool Litterale::isLitteraleToPush(const QString& s) {
    if (LittNumerique::isLittNumerique(s)||LittComplexe::isLittComplexe(s)||LittProgramme::isLittProgramme(s)||LittExpression::isLittExpression(s)){
        return true;
    }
    return false;
}
//-------------------------------------LITTERALE NUMERIQUE-----------------------------------------------

bool LittNumerique::isLittNumerique(const QString& s) {
    if (LittEntiere::isLittEntiere(s)||LittRationnelle::isLittRationnelle(s)||LittReelle::isLittReelle(s))
        return true;
    return false;
}

//-------------------------------------LITTERALE ENTIERE-----------------------------------------------

bool LittEntiere::isLittEntiere(const QString& s) {
   QRegExp r("[-]?[0-9]+");
   if (r.exactMatch(s))
       return true;
   QRegExp r2("[-]?[0-9]+\\/[-]?1");
   if (r2.exactMatch(s))
       return true;
   QRegExp r3("[-]?[0-9]+\\.|[-]?[0-9]+\\.0+");
   if (r3.exactMatch(s))
       return true;
   return false;
}

LittEntiere* FactoryLitterale::createLittEntiere(const QString& s){
   QRegExp r("[-]?[0-9]+");
   QRegExp r2("[-]?[0-9]+\\/[-]?1");
   QRegExp r3("[-]?[0-9]+\\.|[-]?[0-9]+\\.0+");
   if (r.exactMatch(s)) {
       int v=s.toInt();
       LittEntiere* l = new LittEntiere(v);
       tab.append(l);
       return l;
   }

   else if (r2.exactMatch(s)) {
       QString cpy =s;
       QStringList res =cpy.split("/");
       QString num,den;
       num=res[0];
       den=res[1];
       QRegExp r("[-][0-9]+");
       int nume;
       nume=num.toInt();
       int deno;
       deno=den.toInt();
       if ((nume<0 && deno<0) || (nume>0 && deno<0)) {
           nume=-nume;
       }
       LittEntiere* l = new LittEntiere(nume);
       tab.append(l);
       return l;
   }

   else if (r3.exactMatch(s)){
       QString cpy=s;
       QStringList res =cpy.split(".");
       QString cpy2=res[0];
       int v=cpy2.toInt();
       LittEntiere* l = new LittEntiere(v);
       tab.append(l);
       return l;

   }
   else {
       throw UTComputerException("erreur");
   }
}

double LittEntiere::getRealValue(){
    double d = (double)value;
    return d;
}

//-------------------------------------LITTERALE REELLE-----------------------------------------------

bool LittReelle::isLittReelle(const QString& s) {
   //QRegExp r("[-]?[0-9]+\\.[0-9]+|\\.[0-9]+|[-]?[0-9]+\\.|[-]?[0-9]+\\.[0-9]+");
   //QRegExp r("[-]?[0-9]+\\.[0-9]+|\\.[0-9]+");
   //okQRegExp r("[-]?[0-9]+\\.0[0-9]+|[-]?[0-9]+\\.[1-9]+0*[1-9]*|\\.[0-9]+");
   QRegExp r("[-]?[0-9]+\\.0+[1-9]+[0-9]*|[-]?[0-9]+\\.[1-9]+0*[0-9]*|\\.[0-9]+");
   if (r.exactMatch(s))
       return true;
   return false;
}

LittReelle* FactoryLitterale::createLittReelle(const QString& s) {
    QString cpy =s;
    double res;
    res=cpy.toDouble();
    LittReelle* l = new LittReelle(res);
    tab.append(l);
    return l ;
}

double LittReelle::getRealValue(){
    return value;
}

//-------------------------------------LITTERALE RATIONNELLE-----------------------------------------------

bool LittRationnelle::isLittRationnelle(const QString& s) {
    QRegExp r("[-]?[0-9]+\\/[-]?[0-9]+");
    if (r.exactMatch(s))
        return true;
    return false;
}


LittRationnelle* FactoryLitterale::createLittRationnelle(const QString& s) {
     QString cpy =s;
     QStringList res =cpy.split("/");
     QString num,den;
     num=res[0];
     den=res[1];
     int nume;
     nume=num.toInt();
     int deno;
     deno=den.toInt();
     LittEntiere* d= new LittEntiere(deno);
     tab.append(d);
     LittEntiere* n = new LittEntiere(nume);
     tab.append(n);
     LittRationnelle* l = new LittRationnelle(*n,*d);
     tab.append(l);
     return l;
}

void LittRationnelle::simplification(){

    int i=pgcd(numerateur.getValue(),denominateur.getValue());
    int num=getNumerateur().getValue()/i;
    int den=getDenominateur().getValue()/i;
    numerateur.setValue(num);
    denominateur.setValue(den);
    if (getNumerateur().getValue()<0 && getDenominateur().getValue()<0)
    {
        numerateur.neg();
        denominateur.neg();
    }
    if (getNumerateur().getValue()>0 && getDenominateur().getValue()<0)
    {
        numerateur.neg();
        denominateur.neg();
    }
}

double LittRationnelle::getRealValue(){
    int num = getNumerateur().getValue();
    int den = getDenominateur().getValue();
    double d = num /(double)den;
    return d;
}

QString LittRationnelle::afficher(){
    if (denominateur.getValue()==1)
        return QString::number(numerateur.getValue());
    return QString::number(numerateur.getValue())+"/"+QString::number(denominateur.getValue());
}

LittRationnelle::LittRationnelle(LittEntiere n, LittEntiere d) : numerateur(n), denominateur(d) {

    if (d.getValue()==0) throw UTComputerException("Impossible de créer une fraction ayant pour denominateur 0");
    simplification();
}


//-------------------------------------LITTERALE COMPLEXE-----------------------------------------------
bool LittComplexe::isLittComplexe(const QString& s) {
    QRegExp r("((?:(?:[+-]?\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)\\$[+-]?(?:\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)))");
    if (r.exactMatch(s))
        return true;
    return false;
}

LittComplexe* FactoryLitterale::createLittComplexe(const QString& s){
        QString cpy=s;
        QStringList res =cpy.split("$");
        QString re=res[0];
        QString im=res[1];
        LittNumerique* r;
        LittNumerique* i;
        if (LittRationnelle::isLittRationnelle(re)) {
            LittRationnelle* l5=FactoryLitterale::createLittRationnelle(re);
            r=l5;
        }
        if (LittRationnelle::isLittRationnelle(im)) {
            LittRationnelle* l6=FactoryLitterale::createLittRationnelle(im);
            i=l6;
        }
        if (LittEntiere::isLittEntiere(re)) {
            LittEntiere* l1=new LittEntiere(re.toInt());
            tab.append(l1);
            r=l1;
        }
        if (LittEntiere::isLittEntiere(im)) {
            LittEntiere* l2=new LittEntiere(im.toInt());
            tab.append(l2);
            i=l2;
        }
        if (LittReelle::isLittReelle(re)) {
            LittReelle* l3=FactoryLitterale::createLittReelle(re);
            r=l3;
        }
        if (LittReelle::isLittReelle(im)) {
           LittReelle* l4=FactoryLitterale::createLittReelle(im);
            i=l4;
        }

        LittComplexe *l= new LittComplexe(r,i);
        tab.append(l);
        return l;
    }



bool LittComplexe::isNull() {if ((partieReelle->getRealValue()==0)&&(partieIm->getRealValue()==0)) return true; return false;}

//-------------------------------------LITTERALE ATOME-----------------------------------------------
bool LittAtome::isLittAtome(const QString& s) {
    QRegExp r("[A-Z][A-Z|0-9]*");
    if (r.exactMatch(s))
        return true;
    return false;
}

LittAtome* FactoryLitterale::createLittAtome(const QString& s){
    LittAtome* l=new LittAtome(s);
    tab.append(l);
    return l;
}

bool LittAtome::isVariable(LittAtome* l){
    /*for(QVector<LittAtome*>::const_iterator it=variables.begin(); it!=variables.end(); ++it)
            if((*it)->getId()==l->getId()) return true;
        return false;*/
    QString id=l->getId();
    return mapvariables.contains(id);
}

bool LittAtome::isProgramme(LittAtome* l){
    QString id=l->getId();
    return LittProgramme::mapprogrammes.contains(id);
}


//-------------------------------------LITTERALE EXPRESSION-----------------------------------------------

bool LittExpression::isLittExpression(const QString& s) {
    QString cpy=s;
    cpy.remove(QChar(' '));
    //QRegExp r("'[A-Z|a-z|\\/|\\$|\\.|\\+|\\-|0-9|\\*|\\(|\\)|\\s]*'"); //peut etre il faudrait vérifier que l'expression est correcte : meme nb de parentheses ouvrante que fermantes
    QRegExp r("'([^']+)'");
    if (r.exactMatch(cpy))
        return true;
    return false;
}

LittExpression* FactoryLitterale::createLittExpression(const QString& s){
    QString cpy=s;
    cpy.remove(QChar(' '));
    LittExpression* l=new LittExpression(cpy);
    tab.append(l);
    return l;
}

//-------------------------------------LITTERALE PROGRAMME-----------------------------------------------

bool LittProgramme::isLittProgramme(const QString& s) {

    QRegExp r("\\[(.*)\\]");
    if (r.exactMatch(s)){
        return true;
    }
    return false;
}

LittProgramme* FactoryLitterale::createLittProgramme(const QString& s){
    LittProgramme* l=new LittProgramme(s);
    tab.append(l);
    return l;
}

//-------------------------------------FACTORY LITTERALE-----------------------------------------------

Litterale* FactoryLitterale::makeLitt(const QString& s) {
    Litterale* l;
    if (LittEntiere::isLittEntiere(s)) {
        l=FactoryLitterale::createLittEntiere(s);
        return l;
     }
    else if (LittRationnelle::isLittRationnelle(s)) {
        l=FactoryLitterale::createLittRationnelle(s);
        return l;
     }
    else if (LittReelle::isLittReelle(s)) {
        l=FactoryLitterale::createLittReelle(s);
        return l;
     }
    else if (LittComplexe::isLittComplexe(s)) {
        l=FactoryLitterale::createLittComplexe(s);
        return l;
     }
    else if (LittAtome::isLittAtome(s)) {
        l=FactoryLitterale::createLittAtome(s);
        return l;
     }
    else if (LittExpression::isLittExpression(s)) {

        l=FactoryLitterale::createLittExpression(s);
        return l;
     }
    else if (LittProgramme::isLittProgramme(s)) {

        l=FactoryLitterale::createLittProgramme(s);
        return l;
     }
    else return nullptr; //il faut throw une exception
}

FactoryLitterale::~FactoryLitterale(){
    for(QVector<Litterale*>::const_iterator it=tab.begin(); it!=tab.end(); ++it) {
            Litterale* l=*it;
            delete l;
        }
}


//-------------------------------------AUTRES FONCTIONS UTILES-----------------------------------------------



int pgcd(int a, int b) {
    if (b==0)
        return a;
    else
        return pgcd(b, a%b);
}
