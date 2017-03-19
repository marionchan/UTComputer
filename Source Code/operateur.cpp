#include "operateur.h"
#include "pile.h"
#include "litterale.h"
#include <math.h>

QVector<QString> OpPile::vectOpPile{"DUP","DROP","SWAP","LASTOP","LASTARGS","UNDO","REDO","CLEAR","EVAL","STO","FORGET"};
QVector<QString> OpUnaire::vectOpUnaire{"NEG","NUM","DEN","NOT","RE","IM"};
QVector<QString> OpBinaire::vectOpBinaire{"+","-","/","*","DIV","MOD","$","=","!=","=<","<=","=>",">=","<",">","AND","OR","IFT"};
int OpPile::nbClear=0;
//QString OpPile::undoOp=NULL;

bool Operateur::isOperateur(const QString& s) {
    if ( OpUnaire::isOpUnaire(s) || OpBinaire::isOpBinaire(s) || OpPile::isOpPile(s) )
        return true;
    return false;
}

bool OpPile::isOpPile(const QString& s) {
    return vectOpPile.contains(s);
}
bool OpUnaire::isOpUnaire(const QString& s) {
    return vectOpUnaire.contains(s);
}
bool OpBinaire::isOpBinaire(const QString& s) {
    return vectOpBinaire.contains(s);
}


// A FAIRE : UNDO , REDO , LASTOP , LASTARGS
// A VERIFIER

//-------------------------------------OPERATEURS PILE-----------------------------------------------

void OpPile::applyOpPile(Pile& p,FactoryLitterale& fl,const QString& s){

    if (s=="DROP" && (p.size()>0))
    {
        Litterale* l = p.top();
        fl.tlastargs.append(l);
        p.pop();
        fl.tlastop.append(s);
    }

    else if (s=="SWAP" && (p.size()>1))
    {
        Litterale* l1 = p.top();
        p.pop();
        Litterale* l2= p.top();
        p.pop();
        p.push(l1);
        p.push(l2);
        fl.tlastargs.append(l2);
        fl.tlastargs.append(l1);
        fl.tlastop.append(s);
    }

    else if (s=="DUP" && (p.size()>0))
    {
        Litterale* l1 = p.top();
        QString litt = l1->afficher();
        Litterale* l3 = fl.makeLitt(litt);
        p.push(l3);
        fl.tlastargs.append(l1);
        fl.tlastop.append(s);
    }

    else if (s=="CLEAR" && (p.size()>0))
    {
        OpPile::nbClear=0;
        while (!p.empty())
        {
            fl.tlastargs.append(p.top());
            OpPile::nbClear++;
            p.pop();
        }
        fl.tlastop.append(s);
    }

    else if (s=="STO" && (p.size()>1))
    {
        LittExpression* l1=dynamic_cast<LittExpression*>(p.top());
        if (l1) {
            QString sl=l1->afficher();
            sl.remove(QRegExp("'"));
            if (LittAtome::isLittAtome(sl)){
                p.pop();
                LittProgramme* pgr=dynamic_cast<LittProgramme*>(p.top());
                if (pgr) {
                    QString instructions=pgr->afficher();
                    LittProgramme::mapprogrammes.insert(sl,instructions);
                    p.pop();
                    fl.tlastargs.append(pgr);
                    fl.tlastargs.append(l1);
                    fl.tlastop.append(s);

                }
                else {
                    Litterale* l2=p.top();
                    p.pop();
                    LittAtome::mapvariables.insert(sl,l2);
                    fl.tlastargs.append(l2);
                    fl.tlastargs.append(l1);
                    fl.tlastop.append(s);
                }

              }
              else {
                    throw UTComputerException("L'expression à stocker n'est pas un atome");
              }
         }
         else {
             throw UTComputerException("La variable à stocker n'est pas une expression");
         }
    }

    else if (s=="FORGET" && (p.size()>0))
    {
        LittExpression* l1=dynamic_cast<LittExpression*>(p.top());
        if (l1)
        {
            QString sl=l1->afficher();
            sl.remove(QRegExp("'"));
            if (LittAtome::isLittAtome(sl)) {
                if (LittAtome::mapvariables.contains(sl)) {
                    LittAtome::mapvariables.remove(sl);
                    p.pop();
                    fl.tlastargs.append(l1);
                    fl.tlastop.append(s);
                }
                else if (LittProgramme::mapprogrammes.contains(sl)) {
                    LittProgramme::mapprogrammes.remove(sl);
                    p.pop();
                    fl.tlastargs.append(l1);
                    fl.tlastop.append(s);
                }
                else {
                    throw UTComputerException("Impossible d'oublier un identificateur associé à aucune variable ou programme");
                }

            }
            else {
                throw UTComputerException("Impossible d'oublier un identificateur qui n'est pas un atome");
            }

        }
        else {
            throw UTComputerException("Impossible d'oublier un identificateur qui n'est pas une expression");
        }

    }

    else if (s=="LASTOP")
    {
        if(!fl.tlastop.isEmpty())
        {
            QString& op = fl.tlastop.last();
            if (OpPile::isOpPile(op)){
                OpPile::applyOpPile(p,fl,op);
                /*QString message="opérateur effectué";
                setMessage(message);*/
            }
            else if (OpUnaire::isOpUnaire(op) && p.size()>0){
                OpUnaire::applyOpUnaire(p,fl,op);
                /*QString message="opérateur effectué";
                setMessage(message);*/
            }
            else if (OpBinaire::isOpBinaire(op) && p.size()>1){
                OpBinaire::applyOpBinaire(p,fl,op);
                /*QString message="opérateur effectué";
                setMessage(message);*/
            }
            //else if(op=="EVAL" && p.size()>0) Controleur::applyOpEVAL();
            else {
                throw UTComputerException("Impossible d'appliquer l'opérateur");
            }
        }
        else
        {
            throw UTComputerException("il n'y a pas d'operateur a appeler");
        }
    }

    else if (s=="LASTARGS")
    {
        if((!fl.tlastargs.isEmpty())||(!fl.tlastop.isEmpty()))
        {
            QString& op = fl.tlastop.last();
            if (OpUnaire::isOpUnaire(op)||op=="DROP"||op=="DUP"||op=="FORGET")
            {
                p.push(fl.tlastargs.last());
            }
            else if (OpBinaire::isOpBinaire(op)||op=="SWAP"||op=="STO")
            {
                p.push(fl.tlastargs.at(fl.tlastargs.size()-2));
                p.push(fl.tlastargs.last());
            }
            else if (op=="CLEAR")
            {
                for(int i = fl.tlastargs.size()-1; i > fl.tlastargs.size()-1-OpPile::nbClear; i--)
                    p.push((fl.tlastargs.at(i)));
            }
            else
            {
                throw UTComputerException("Le dernier operateur applique n'avait pas d'arguments");
            }
        }
        else
        {
            throw UTComputerException("Aucune operation avec arguments n'a ete effectuee");
        }
    }

    else if (s=="UNDO")
    {
        if((!fl.tlastargs.isEmpty())&&(!fl.tlastop.isEmpty()))
        {
            QString& op = fl.tlastop.last();
            if (OpUnaire::isOpUnaire(op))
            {
                p.pop();
                p.push(fl.tlastargs.last());
                //fl.tundoargs.append(fl.tlastargs.last());
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            else if(op=="DROP")
            {
                p.push(fl.tlastargs.last());
                //fl.tundoargs.append(fl.tlastargs.last());
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            else if(op=="DUP")
            {
                p.pop();
                //fl.tundoargs.append(fl.tlastargs.last());
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            //op=="FORGET"
            else if (OpBinaire::isOpBinaire(op))
            {
                //fl.tundoargs;
                p.pop();
                p.push(fl.tlastargs.at(fl.tlastargs.size()-2));
                p.push(fl.tlastargs.last());
                //fl.tundoargs.append(fl.tlastargs.at(fl.tlastargs.size()-2));
                //fl.tundoargs.append(fl.tlastargs.last());
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            else if(op=="SWAP")
            {
                p.pop();
                p.pop();
                p.push(fl.tlastargs.at(fl.tlastargs.size()-2));
                p.push(fl.tlastargs.last());
                //fl.tundoargs.append(fl.tlastargs.at(fl.tlastargs.size()-2));
                //fl.tundoargs.append(fl.tlastargs.last());
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            else if (op=="STO")
            {
                p.push(fl.tlastargs.last());
                OpPile::applyOpPile(p,fl,"FORGET");
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
            else if (op=="CLEAR")
            {
                for(int i = fl.tlastargs.size()-1; i > fl.tlastargs.size()-1-OpPile::nbClear; i--){
                    p.push((fl.tlastargs.at(i)));
                    //fl.tundoargs.append(fl.tlastargs.at(i));
                }
                //OpPile::undoOp = op;
                fl.tlastop.append(s);
            }
        }
        else
        {
            throw UTComputerException("Aucune operation avec arguments n'a ete effectuee");
        }
    }

    else if (s=="REDO")
    {
        /*if((!fl.tundoargs.isEmpty())&&(fl.tlastop.last()=="UNDO"))
        {
            if(OpUnaire::isOpUnaire(OpPile::undoOp))
            {
                //p.pop;
                p.push(fl.tundoargs.last());
                OpUnaire::applyOpUnaire(p,fl,OpPile::undoOp);
            }
            else if(OpBinaire::isOpBinaire(OpPile::undoOp))
            {
                p.push(fl.tundoargs.at(fl.tlastargs.size()-2));
                p.push(fl.tundoargs.last());
                OpBinaire::applyOpBinaire(p,fl,OpPile::undoOp);
            }
            else if(OpPile::isOpPile(OpPile::undoOp))
            {
                OpPile::applyOpPile(p,fl,OpPile::undoOp);
            }
        }
        else
        {
           throw UTComputerException("Aucune operation UNDO n'a ete effectuee");
        }*/
        throw UTComputerException("Opérateur REDO non défini");
    }

    else {
        throw UTComputerException("Pas suffisamment d'arguments");
    }
}


//-------------------------------------OPERATEURS UNAIRES-----------------------------------------------

void OpUnaire::applyOpUnaire(Pile& p,FactoryLitterale& fl,const QString& s){

    if (s=="NEG" && p.size()>0) {
        Litterale* l=p.top();
        //fl.tab.append(l);
        QString sl=l->afficher();
        if (LittNumerique::isLittNumerique(sl) || LittComplexe::isLittComplexe(sl)){
            LittNumerique* l1=dynamic_cast<LittNumerique*>(l);
            //fl.tab.   (l1);
            if (l1){
                l1->neg();
                p.pop();
                p.push(l1);
                fl.tlastargs.append(l);
                fl.tlastop.append(s);
            }
            else {
                LittComplexe* l2=dynamic_cast<LittComplexe*>(l);
                //fl.tab.append(l2);
                if (l2) {
                    l2->neg();
                    p.pop();
                    p.push(l2);
                    fl.tlastargs.append(l);
                    fl.tlastop.append(s);
                }
            }
        }
        else {
            throw UTComputerException("Impossible d'appliquer l'opérateur NEG sur une littérale ni numérique ni complexe");
        }
    }

    else if (s=="NUM" && (p.size()>0)) {
        LittRationnelle* l1 = dynamic_cast<LittRationnelle*>(p.top());
        //fl.tab.append(l1);
        if(l1){
        int i=l1->getNumerateur().getValue();
        LittEntiere* l = new LittEntiere(i);
        fl.tab.append(l);
        p.pop();
        p.push(l);
        fl.tlastargs.append(l1);
        fl.tlastop.append(s);
        }
        else {
            LittEntiere* l2=dynamic_cast<LittEntiere*>(p.top());
            //fl.tab.append(l2);
            if (l2) {
                //p.push(p.top());
                //on fait rien
                fl.tlastargs.append(l2);
                fl.tlastop.append(s);
                throw UTComputerException("Operateur RE appliqué : aucun effet sur les littérales entières");
            }

        else {
            throw UTComputerException("Impossible d'appliquer l'opérateur NUM sur une littérale ni rationnelle ni entière");
        }
        }
    }

    else if (s=="DEN" && (p.size()>0)) {
        LittRationnelle* l1 = dynamic_cast<LittRationnelle*>(p.top());
        //fl.tab.append(l1);
        if(l1){
        int i=l1->getDenominateur().getValue();
        LittEntiere* l = new LittEntiere(i);
        fl.tab.append(l);
        p.pop();
        p.push(l);
        fl.tlastargs.append(l1);
        fl.tlastop.append(s);
        }
        else {
            LittEntiere* l2=dynamic_cast<LittEntiere*>(p.top());
            //fl.tab.append(l2);
            if (l2) {
                LittEntiere* l3=new LittEntiere(1);
                fl.tab.append(l3);
                p.pop();
                p.push(l3);
                fl.tlastargs.append(l2);
                fl.tlastop.append(s);
            }

        else {
            throw UTComputerException("Impossible d'appliquer l'opérateur DEN sur une littérale ni rationnelle ni entière");
        }
        }
    }

    else if (s=="RE" && (p.size()>0)) {
        LittComplexe* l1 = dynamic_cast<LittComplexe*>(p.top());
        if(l1){
            LittEntiere* l2=dynamic_cast<LittEntiere*>(l1->getRe());
            LittReelle* l4=dynamic_cast<LittReelle*>(l1->getRe());
            LittRationnelle* l5=dynamic_cast<LittRationnelle*>(l1->getRe());
           if (l2){
               LittEntiere* l3=new LittEntiere(l2->getValue());
               fl.tab.append(l3);
               p.pop();
               p.push(l3);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else if(l4){
               double d=l4->getValue();
               LittReelle* l3= new LittReelle(d);
               fl.tab.append(l3);
               p.pop();
               p.push(l3);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else if(l5) {
               int i=l5->getNumerateur().getValue();
               LittEntiere* num=new LittEntiere(i);
               fl.tab.append(num);
               int j=l5->getDenominateur().getValue();
               LittEntiere* den=new LittEntiere(j);
               fl.tab.append(den);
               LittRationnelle* l6= new LittRationnelle(*num,*den);
               fl.tab.append(l6);
               p.pop();
               p.push(l6);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else {
               throw UTComputerException("Impossible d'appliquer l'opérateur RE");
           }
        }
        else {
            LittEntiere* l2=dynamic_cast<LittEntiere*>(p.top());
            LittReelle* l4=dynamic_cast<LittReelle*>(p.top());
            LittRationnelle* l5=dynamic_cast<LittRationnelle*>(p.top());
            if (l2) {
                fl.tlastargs.append(l2);
                fl.tlastop.append(s);
                throw UTComputerException("Operateur RE appliqué : aucun effet sur les littérales numériques");
            }
            else if (l4) {
                fl.tlastargs.append(l4);
                fl.tlastop.append(s);
                throw UTComputerException("Operateur RE appliqué : aucun effet sur les littérales numériques");
            }
            else if (l5) {
                fl.tlastargs.append(l5);
                fl.tlastop.append(s);
                throw UTComputerException("Operateur RE appliqué : aucun effet sur les littérales numériques");
            }
            else {
                throw UTComputerException("Impossible d'appliquer l'opérateur RE sur une littérale ni numérique ni complexe");
            }
         }
    }

    else if (s=="IM" && (p.size()>0)) {
        LittComplexe* l1 = dynamic_cast<LittComplexe*>(p.top());
        //fl.tab.append(l1);
        if(l1){
            LittEntiere* l2=dynamic_cast<LittEntiere*>(l1->getIm());
            LittReelle* l4=dynamic_cast<LittReelle*>(l1->getIm());
            LittRationnelle* l5=dynamic_cast<LittRationnelle*>(l1->getIm());
           if (l2){
               LittEntiere* l3=new LittEntiere(l2->getValue());
               fl.tab.append(l3);
               p.pop();
               p.push(l3);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else if(l4){
               double d=l4->getValue();
               LittReelle* l3= new LittReelle(d);
               fl.tab.append(l3);
               p.pop();
               p.push(l3);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else if(l5) {
               int i=l5->getNumerateur().getValue();
               LittEntiere* num=new LittEntiere(i);
               fl.tab.append(num);
               int j=l5->getDenominateur().getValue();
               LittEntiere* den=new LittEntiere(j);
               fl.tab.append(den);
               LittRationnelle* l6= new LittRationnelle(*num,*den);
               fl.tab.append(l6);
               p.pop();
               p.push(l6);
               fl.tlastargs.append(l1);
               fl.tlastop.append(s);

           }
           else {
               throw UTComputerException("Impossible d'appliquer l'opérateur IM");
           }

        }
        else {
            LittEntiere* l2=dynamic_cast<LittEntiere*>(p.top());
            LittReelle* l4=dynamic_cast<LittReelle*>(p.top());
            LittRationnelle* l5=dynamic_cast<LittRationnelle*>(p.top());
           if (l2){
               LittEntiere* l7=new LittEntiere(0);
               p.pop();
               p.push(l7);
               fl.tab.append(l7);
               fl.tlastargs.append(l2);
               fl.tlastop.append(s);
           }
           else if (l4){
               LittEntiere* l7=new LittEntiere(0);
               p.pop();
               p.push(l7);
               fl.tab.append(l7);
               fl.tlastargs.append(l4);
               fl.tlastop.append(s);
           }
           else if (l5){
               LittEntiere* l7=new LittEntiere(0);
               p.pop();
               p.push(l7);
               fl.tab.append(l7);
               fl.tlastargs.append(l5);
               fl.tlastop.append(s);
           }
           else {
               throw UTComputerException("Impossible d'appliquer l'opérateur IM sur une littérale ni numérique ni complexe");
           }
        }
    }

    else if (s=="NOT" && p.size()>0)
    {
        Litterale* lb=p.top(); p.pop();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))
        {
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            if ((lnb&&lnb->getRealValue()==0)||(lcb&&lcb->isNull()))
            {
                LittEntiere* l = new LittEntiere(1);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastop.append(s);
            }
            else
            {
                LittEntiere* l = new LittEntiere(0);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            throw UTComputerException("Impossible d'appliquer l'opérateur NOT sur une littérale ni numérique ni complexe");
        }
    }

    else {
        throw UTComputerException("Pas suffisamment d'aguments");
    }
}

//-------------------------------------OPERATEURS BINAIRES-----------------------------------------------

void OpBinaire::applyOpBinaire(Pile& p,FactoryLitterale& fl,const QString& s){


    //elimination des faux complexes/rationnels/reels
    Litterale* l1 = p.top(); p.pop();
    //fl.tab.append(l1);
    Litterale* l2 = p.top(); p.pop();
    //fl.tab.append(l2);

    //-------------ELIMINATION FAUX RATIONNELS-------------
    LittRationnelle* lra1 = dynamic_cast<LittRationnelle*>(l1);
    //fl.tab.append(lra1);
    if(lra1)
    {
        if (lra1->getDenominateur().getValue()==1)
        l1 = new LittEntiere(lra1->getNumerateur().getValue());
        fl.tab.append(l1);
    }
    LittRationnelle* lra2 = dynamic_cast<LittRationnelle*>(l2);
    //fl.tab.append(lra2);
    if(lra2)
    {
        if (lra2->getDenominateur().getValue()==1)
        l2 = new LittEntiere(lra2->getNumerateur().getValue());
        fl.tab.append(l2);
    }

    //-------------ELIMINATION FAUX COMPLEXES-------------
    LittComplexe* lc1 = dynamic_cast<LittComplexe*>(l1);
    //fl.tab.append(lc1);
    if(lc1)
    {
        if (lc1->getIm()->getRealValue()==0)
        {
            LittEntiere* le1 = dynamic_cast<LittEntiere*>(lc1->getRe());
            //fl.tab.append(le1);
            if (le1)
            {
                l1 = new LittEntiere(le1->getValue());
                fl.tab.append(l1);
            }
            LittRationnelle* lra1 = dynamic_cast<LittRationnelle*>(lc1->getRe());
            //fl.tab.append(lra1);
            if (lra1)
            {
                l1 = new LittRationnelle(lra1->getNumerateur().getValue(),lra1->getDenominateur().getValue());
                fl.tab.append(l1);
            }
            LittReelle* lre1 = dynamic_cast<LittReelle*>(lc1->getRe());
            //fl.tab.append(lre1);
            if (lre1)
            {
                l1 = new LittReelle(lre1->getValue());
                fl.tab.append(l1);
            }
        }
    }
    LittComplexe* lc2 = dynamic_cast<LittComplexe*>(l2);
    //fl.tab.append(lc2);
    if(lc2)
    {
        if (lc2->getIm()->getRealValue()==0)
        {
            LittEntiere* le2 = dynamic_cast<LittEntiere*>(lc2->getRe());
            //fl.tab.append(le2);
            if (le2)
            {
                l2 = new LittEntiere(le2->getValue());
                fl.tab.append(l2);
            }
            LittRationnelle* lra2 = dynamic_cast<LittRationnelle*>(lc2->getRe());
            //fl.tab.append(lra2);
            if (lra2)
            {
                l2 = new LittRationnelle(lra2->getNumerateur().getValue(),lra2->getDenominateur().getValue());
                fl.tab.append(l2);
            }
            LittReelle* lre2 = dynamic_cast<LittReelle*>(lc2->getRe());
            //fl.tab.append(lre2);
            if (lre2)
            {
                l2 = new LittReelle(lre2->getValue());
                fl.tab.append(l2);
            }
        }
    }

    //-------------ELIMINATION FAUX REELS-------------
    LittReelle* lre1 = dynamic_cast<LittReelle*>(l1);
    //fl.tab.append(lre1);
    if(lre1)
    {
        if(fmod(lre1->getValue(),1)==0)
        {
            l1 = new LittEntiere((int)lre1->getValue());
            fl.tab.append(l1);
        }
    }
    LittReelle* lre2 = dynamic_cast<LittReelle*>(l2);
    //fl.tab.append(lre2);
    if(lre2)
    {
        if(fmod(lre2->getValue(),1)==0)
        {
            l2 = new LittEntiere((int)lre2->getValue());
            fl.tab.append(l2);
        }
    }

    p.push(l2); p.push(l1);

//--------APPLICATION DES OPERATEURS

    if(s=="+" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittEntiere::isLittEntiere(sb) && LittEntiere::isLittEntiere(sa))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            if (leb&&lea)
            {
                int i = leb->getValue()+lea->getValue();
                LittEntiere* l = new LittEntiere(i);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittEntiere::isLittEntiere(sb)||LittRationnelle::isLittRationnelle(sb))&&LittRationnelle::isLittRationnelle(sa))
               ||((LittEntiere::isLittEntiere(sa)||LittRationnelle::isLittRationnelle(sa))&&LittRationnelle::isLittRationnelle(sb)))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            LittRationnelle* lraa = dynamic_cast<LittRationnelle*>(la);
            //fl.tab.append(lraa);
            LittRationnelle* lrab = dynamic_cast<LittRationnelle*>(lb);
            //fl.tab.append(lrab);
            if (lrab&&lea)
            {
                int numa = lrab->getDenominateur().getValue()*lea->getValue();
                LittRationnelle* l = new LittRationnelle(numa+lrab->getNumerateur().getValue(),lrab->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (leb&&lraa)
            {
                int numb = lraa->getDenominateur().getValue()*leb->getValue();
                LittRationnelle* l = new LittRationnelle(numb+lraa->getNumerateur().getValue(),lraa->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (lrab&&lraa)
            {
                int numa = lraa->getNumerateur().getValue();
                int dena = lraa->getDenominateur().getValue();
                int numb = lrab->getNumerateur().getValue();
                int denb = lrab->getDenominateur().getValue();
                int den = dena*denb;
                LittRationnelle* l = new LittRationnelle((numa*denb)+(numb*dena),den);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if ((LittNumerique::isLittNumerique(sb)&&LittReelle::isLittReelle(sa))
               ||(LittNumerique::isLittNumerique(sa)&&LittReelle::isLittReelle(sb)))
        {
                LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
                //fl.tab.append(lna);
                LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
                //fl.tab.append(lnb);
                if (lnb&&lna)
                {
                    LittReelle* l = new LittReelle(lnb->getRealValue()+lna->getRealValue());
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
        }
        else if (((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&LittComplexe::isLittComplexe(sa))
               ||((LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa))&&LittComplexe::isLittComplexe(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            if(lnb&&lca)
            {
                LittReelle* lr = new LittReelle(lnb->getRealValue()+lca->getRe()->getRealValue());
                fl.tab.append(lr);
                LittComplexe* l = new LittComplexe(lr,lca->getIm());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            if(lcb&&lna)
            {
                LittReelle* lr = new LittReelle(lna->getRealValue()+lcb->getRe()->getRealValue());
                fl.tab.append(lr);
                LittComplexe* l = new LittComplexe(lr,lcb->getIm());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            if(lcb&&lca)
            {
                LittReelle* lr = new LittReelle(lcb->getRe()->getRealValue()+lca->getRe()->getRealValue());
                fl.tab.append(lr);
                LittReelle* li = new LittReelle(lcb->getIm()->getRealValue()+lca->getIm()->getRealValue());
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("cas inexistant : + sur litterales numeriques/complexes seulement");
        }
    }

    else if(s=="-" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittEntiere::isLittEntiere(sb) && LittEntiere::isLittEntiere(sa))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            if (lea&&leb)
            {
                int i = leb->getValue()-lea->getValue();
                LittEntiere* l = new LittEntiere(i);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittEntiere::isLittEntiere(sb)||LittRationnelle::isLittRationnelle(sb))&&LittRationnelle::isLittRationnelle(sa))
               ||((LittEntiere::isLittEntiere(sa)||LittRationnelle::isLittRationnelle(sa))&&LittRationnelle::isLittRationnelle(sb)))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            LittRationnelle* lraa = dynamic_cast<LittRationnelle*>(la);
            //fl.tab.append(lraa);
            LittRationnelle* lrab = dynamic_cast<LittRationnelle*>(lb);
            //fl.tab.append(lrab);
            if (lrab&&lea)
            {
                int numa = lrab->getDenominateur().getValue()*lea->getValue();
                LittRationnelle* l = new LittRationnelle(lrab->getNumerateur().getValue()-numa,lrab->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (leb&&lraa)
            {
                int numb = lraa->getDenominateur().getValue()*leb->getValue();
                LittRationnelle* l = new LittRationnelle(numb-lraa->getNumerateur().getValue(),lraa->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (lrab&&lraa)
            {
                int numa = lraa->getNumerateur().getValue();
                int dena = lraa->getDenominateur().getValue();
                int numb = lrab->getNumerateur().getValue();
                int denb = lrab->getDenominateur().getValue();
                int den = dena*denb;
                LittRationnelle* l = new LittRationnelle((numb*dena)-(numa*denb),den);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if ((LittNumerique::isLittNumerique(sb)&&LittReelle::isLittReelle(sa))
               ||(LittNumerique::isLittNumerique(sa)&&LittReelle::isLittReelle(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            if (lnb&&lna)
            {
                LittReelle* l = new LittReelle(lnb->getRealValue()-lna->getRealValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&LittComplexe::isLittComplexe(sa))
               ||((LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa))&&LittComplexe::isLittComplexe(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            if(lnb&&lca)
            {
                LittReelle* lr = new LittReelle(lnb->getRealValue()-lca->getRe()->getRealValue());
                fl.tab.append(lr);
                LittReelle* li = new LittReelle(-(lca->getIm()->getRealValue()));
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);

            }
            if(lcb&&lna)
            {
                LittReelle* lr = new LittReelle(lcb->getRe()->getRealValue()-lna->getRealValue());
                fl.tab.append(lr);
                LittReelle* li = new LittReelle(lcb->getIm()->getRealValue());
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            if(lcb&&lca)
            {
                LittReelle* lr = new LittReelle(lcb->getRe()->getRealValue()-lca->getRe()->getRealValue());
                fl.tab.append(lr);
                LittReelle* li = new LittReelle(lcb->getIm()->getRealValue()-lca->getIm()->getRealValue());
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("cas inexistant : - sur litterales numeriques/complexes seulement");
        }
    }

    else if(s=="*" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittEntiere::isLittEntiere(sb) && LittEntiere::isLittEntiere(sa))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            if (lea&&leb)
            {
                int i = (leb->getValue())*(lea->getValue());
                LittEntiere* l = new LittEntiere(i);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittEntiere::isLittEntiere(sb)||LittRationnelle::isLittRationnelle(sb))&&LittRationnelle::isLittRationnelle(sa))
               ||((LittEntiere::isLittEntiere(sa)||LittRationnelle::isLittRationnelle(sa))&&LittRationnelle::isLittRationnelle(sb)))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            LittRationnelle* lraa = dynamic_cast<LittRationnelle*>(la);
            //fl.tab.append(lraa);
            LittRationnelle* lrab = dynamic_cast<LittRationnelle*>(lb);
            //fl.tab.append(lrab);
            if (lrab&&lea)
            {
                LittRationnelle* l = new LittRationnelle(lrab->getNumerateur().getValue()*lea->getValue(),lrab->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (leb&&lraa)
            {
                LittRationnelle* l = new LittRationnelle(leb->getValue()*lraa->getNumerateur().getValue(),lraa->getDenominateur().getValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else if (lrab&&lraa)
            {
                int num = lrab->getNumerateur().getValue()*lraa->getNumerateur().getValue();
                int den = lrab->getDenominateur().getValue()*lraa->getDenominateur().getValue();
                LittRationnelle* l = new LittRationnelle(num,den);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if ((LittNumerique::isLittNumerique(sb)&&LittReelle::isLittReelle(sa))
               ||(LittNumerique::isLittNumerique(sa)&&LittReelle::isLittReelle(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            if (lnb&&lna)
            {
                LittReelle* l = new LittReelle(lnb->getRealValue()*lna->getRealValue());
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&LittComplexe::isLittComplexe(sa))
               ||((LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa))&&LittComplexe::isLittComplexe(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            if(lnb&&lca)
            {
                LittReelle* lr = new LittReelle((lnb->getRealValue())*(lca->getRe()->getRealValue()));
                fl.tab.append(lr);
                LittReelle* li = new LittReelle((lnb->getRealValue())*(lca->getIm()->getRealValue()));
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            if(lcb&&lna)
            {
                LittReelle* lr = new LittReelle((lcb->getRe()->getRealValue())*(lna->getRealValue()));
                fl.tab.append(lr);
                LittReelle* li = new LittReelle((lcb->getIm()->getRealValue())*(lna->getRealValue()));
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            if(lcb&&lca)
            {
                LittReelle* lr = new LittReelle(((lcb->getRe()->getRealValue())*(lca->getRe()->getRealValue()))-((lcb->getIm()->getRealValue())*(lca->getIm()->getRealValue())));
                fl.tab.append(lr);
                LittReelle* li = new LittReelle(((lcb->getRe()->getRealValue())*(lca->getIm()->getRealValue()))+((lcb->getIm()->getRealValue())*(lca->getRe()->getRealValue())));
                fl.tab.append(li);
                LittComplexe* l = new LittComplexe(lr,li);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("cas inexistant : * sur litterales numeriques/complexes seulement");
        }
    }

    else if(s=="/" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();

        if ((LittEntiere::isLittEntiere(sb)||LittRationnelle::isLittRationnelle(sb))&&(LittEntiere::isLittEntiere(sa)||LittRationnelle::isLittRationnelle(sa)))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            LittRationnelle* lraa = dynamic_cast<LittRationnelle*>(la);
            //fl.tab.append(lraa);
            LittRationnelle* lrab = dynamic_cast<LittRationnelle*>(lb);
            //fl.tab.append(lrab);
            if ((lea&&lea->getValue()==0)||(lraa&&lraa->getRealValue()==0))
            {
                p.push(lb);
                p.push(la);
                throw UTComputerException("Impossible de diviser par 0");
            }
            else
            {
                if (leb&&lea)
                {
                    LittRationnelle* l = new LittRationnelle(leb->getValue(),lea->getValue());
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                if (lrab&&lea)
                {
                    LittRationnelle* l = new LittRationnelle(lrab->getNumerateur().getValue(),(lrab->getDenominateur().getValue())*(lea->getValue()));
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else if (leb&&lraa)
                {
                    LittRationnelle* l = new LittRationnelle((leb->getValue())*(lraa->getDenominateur().getValue()),lraa->getNumerateur().getValue());
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else if (lrab&&lraa)
                {
                    int num = lrab->getNumerateur().getValue()*lraa->getDenominateur().getValue();
                    int den = lrab->getDenominateur().getValue()*lraa->getNumerateur().getValue();
                    LittRationnelle* l = new LittRationnelle(num,den);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else if ((LittNumerique::isLittNumerique(sb)&&LittReelle::isLittReelle(sa))
               ||(LittNumerique::isLittNumerique(sa)&&LittReelle::isLittReelle(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            if (lna&&lna->getRealValue()==0)
            {
                p.push(lb);
                p.push(la);
                throw UTComputerException("Impossible de diviser par 0");
            }
            else if (lnb&&lna)
            {
                LittReelle* l = new LittReelle((lnb->getRealValue())/(lna->getRealValue()));
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else if (((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&LittComplexe::isLittComplexe(sa))
               ||((LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa))&&LittComplexe::isLittComplexe(sb)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            if(lnb&&lca)
            {
                if(lca->isNull())
                {
                    p.push(lb);
                    p.push(la);
                    throw UTComputerException("Impossible de diviser par 0");
                }
                else
                {
                    LittReelle* lr = new LittReelle((lnb->getRealValue()*lca->getRe()->getRealValue())/((lca->getRe()->getRealValue()*lca->getRe()->getRealValue())+(lca->getIm()->getRealValue()*lca->getIm()->getRealValue())));
                    fl.tab.append(lr);
                    LittReelle* li = new LittReelle((-1*(lnb->getRealValue()*lca->getIm()->getRealValue()))/((lca->getRe()->getRealValue()*lca->getRe()->getRealValue())+(lca->getIm()->getRealValue()*lca->getIm()->getRealValue())));
                    fl.tab.append(li);
                    LittComplexe* l = new LittComplexe(lr,li);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }

            }
            if(lcb&&lna)
            {
                if(lna->getRealValue()!=0)
                {
                    LittReelle* lr = new LittReelle((lcb->getRe()->getRealValue())/(lna->getRealValue()));
                    fl.tab.append(lr);
                    LittReelle* li = new LittReelle((lcb->getIm()->getRealValue())/(lna->getRealValue()));
                    fl.tab.append(li);
                    LittComplexe* l = new LittComplexe(lr,li);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    p.push(lb);
                    p.push(la);
                    throw UTComputerException("Impossible de diviser par 0");
                }
            }
            if(lcb&&lca)
            {
                if(lca->isNull())
                {
                    p.push(lb);
                    p.push(la);
                    throw UTComputerException("Impossible de diviser par 0");
                }
                else
                {
                    LittReelle* lr = new LittReelle(((lcb->getRe()->getRealValue()*lca->getRe()->getRealValue())+(lcb->getIm()->getRealValue()*lca->getIm()->getRealValue()))/((lca->getRe()->getRealValue()*lca->getRe()->getRealValue())+(lca->getIm()->getRealValue()*lca->getIm()->getRealValue())));
                    fl.tab.append(lr);
                    LittReelle* li = new LittReelle(((lcb->getIm()->getRealValue()*lca->getRe()->getRealValue())-(lcb->getRe()->getRealValue()*lca->getIm()->getRealValue()))/((lca->getRe()->getRealValue()*lca->getRe()->getRealValue())+(lca->getIm()->getRealValue()*lca->getIm()->getRealValue())));
                    fl.tab.append(li);
                    LittComplexe* l = new LittComplexe(lr,li);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("cas inexistant : / sur litterales numeriques/complexes seulement");
        }
    }

    else if(s=="DIV" && (p.size()>1)){ //ATTENTION FAUX RATIONNELS/COMPLEXES
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittEntiere::isLittEntiere(sb) && LittEntiere::isLittEntiere(sa))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            if (lea&&lea->getValue()==0)
            {
                p.push(lb);
                p.push(la);
                throw UTComputerException("Impossible de diviser par 0");
            }
            else if (leb&&lea)
            {
                LittEntiere* l = new LittEntiere((leb->getValue())/(lea->getValue()));
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("DIV ne peut être appliqué qu'à 2 litterales entieres");
        }
    }

    else if(s=="MOD" && (p.size()>1)){ //ATTENTION FAUX RATIONNELS/COMPLEXES
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittEntiere::isLittEntiere(sb) && LittEntiere::isLittEntiere(sa))
        {
            LittEntiere* lea = dynamic_cast<LittEntiere*>(la);
            //fl.tab.append(lea);
            LittEntiere* leb = dynamic_cast<LittEntiere*>(lb);
            //fl.tab.append(leb);
            if (lea&&lea->getValue()==0)
            {
                p.push(lb);
                p.push(la);
                throw UTComputerException("Impossible de diviser par 0");
            }
            else if (leb&&lea)
            {
                LittEntiere* l = new LittEntiere((leb->getValue())%(lea->getValue()));
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("MOD ne peut être appliqué qu'à 2 litterales entieres");
        }
    }

    else if(s=="$" && (p.size()>1)){ //ATTENTION FAUX COMPLEXES
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                LittComplexe* l = new LittComplexe(l2,l1);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("$ ne peut être appliqué qu'à 2 litterales numeriques");
        }
    }

    //-------------------------------------OPERATEURS LOGIQUES-----------------------------------------------

    else if(s=="=" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
       // fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()==l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur = ne peut être appliqué qu'à 2 litterales de type numerique");
        }
    }

    else if(s=="!=" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()!=l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur != ne peut être appliqué qu'à 2 litterales de type numerique");
        }
    }

    else if((s=="=<" && (p.size()>1))||(s=="<=" && (p.size()>1))){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()<=l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur <= ne peut être appliquée qu'à 2 litterales de type numerique");
        }
    }

    else if((s==">=" && (p.size()>1))||(s=="=>" && (p.size()>1))){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()>=l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur >= ne peut être appliqué qu'à 2 litterales de type numerique");
        }
    }

    else if(s=="<" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()<l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur < ne peut être appliqué qu'à 2 litterales de type numerique");
        }
    }

    else if(s==">" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
       // fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if (LittNumerique::isLittNumerique(sb)&&LittNumerique::isLittNumerique(sa))
        {
            LittNumerique* l1 = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(l1);
            LittNumerique* l2 = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(l2);
            if (l1&&l2)
            {
                if(l2->getRealValue()>l1->getRealValue())
                {
                    LittEntiere* l = new LittEntiere(1);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
                else
                {
                    LittEntiere* l = new LittEntiere(0);
                    fl.tab.append(l);
                    p.push(l);
                    fl.tlastargs.append(lb);
                    fl.tlastargs.append(la);
                    fl.tlastop.append(s);
                }
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur > ne peut être appliqué qu'à 2 litterales de type numerique");
        }
    }

    else if(s=="AND" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if ((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&(LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            //fl.tab.append(lna);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            //fl.tab.append(lcb);
            if (((lnb&&lnb->getRealValue()==0)||(lcb&&lcb->isNull()))||((lna&&lna->getRealValue()==0)||(lca&&lca->isNull())))
            {
                LittEntiere* l = new LittEntiere(0);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else
            {
                LittEntiere* l = new LittEntiere(1);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur AND ne peut être appliqué qu'à 2 litterales de type numerique/complexe");
        }
    }

    else if(s=="OR" && (p.size()>1)){
        Litterale* la = p.top(); p.pop();
        //fl.tab.append(la);
        Litterale* lb = p.top(); p.pop();
        //fl.tab.append(lb);
        QString sa=la->afficher();
        QString sb=lb->afficher();
        if ((LittNumerique::isLittNumerique(sb)||LittComplexe::isLittComplexe(sb))&&(LittNumerique::isLittNumerique(sa)||LittComplexe::isLittComplexe(sa)))
        {
            LittNumerique* lna = dynamic_cast<LittNumerique*>(la);
            //fl.tab.append(lna);
            LittNumerique* lnb = dynamic_cast<LittNumerique*>(lb);
            //fl.tab.append(lnb);
            LittComplexe* lca = dynamic_cast<LittComplexe*>(la);
            //fl.tab.append(lna);
            LittComplexe* lcb = dynamic_cast<LittComplexe*>(lb);
            //fl.tab.append(lcb);
            if (((lnb&&lnb->getRealValue()==0)||(lcb&&lcb->isNull()))&&((lna&&lna->getRealValue()==0)||(lca&&lca->isNull())))
            {
                LittEntiere* l = new LittEntiere(0);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
            else
            {
                LittEntiere* l = new LittEntiere(1);
                fl.tab.append(l);
                p.push(l);
                fl.tlastargs.append(lb);
                fl.tlastargs.append(la);
                fl.tlastop.append(s);
            }
        }
        else
        {
            p.push(lb);
            p.push(la);
            throw UTComputerException("L'opérateur OR ne peut être appliqué qu'à 2 litterales de type numerique/complexe");
        }
    }

    else if(s=="IFT" && (p.size()>1)){

    }

    else {
        throw UTComputerException("pas assez d'arguments");
    }
}



