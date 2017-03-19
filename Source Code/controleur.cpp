#include "controleur.h"

using namespace std;

void Controleur::printPile(unsigned int n) {
    QVector<Litterale*> liste = pile.stackToVector(n);
    for (QVector<Litterale*>::const_iterator it = liste.begin(); it != liste.end(); ++it) {
         //int i= *it;
        Litterale *i=*it;
        if (i==nullptr)
        {
            cout<<"nul";
        }
        else {
            qDebug().noquote() << i->afficher() <<endl;
        }

    }
}

void Controleur::afficher() {
system("cls");
    cout<<"********************************************* \n";
    qDebug().noquote()<<"M : "<<message<<"\n";
    cout<<"---------------------------------------------\n";
    printPile(2);
    cout<<"\n---------------------------------------------\n";
}



void Controleur::executer(){
    QString c;
    do {
        afficher();
        cout<<"?-";
        std::string s;
        std::getline(cin,s);
        c=c.fromStdString(s);
        if (c!="Q") commande(c);
    } while(c!="Q");// critère d'arret du programme
}

void Controleur::commande(const QString& c) {
    try{
    if(LittProgramme::isLittProgramme(c) || LittExpression::isLittExpression(c)){
        Litterale* l=factL.makeLitt(c);
        QString m = "Commande effectuée";
        setMessage(m);
        pile.push(l);
    }
    else {
        QStringList res =c.split(" ");
        for (int i=0; i<res.size();i++) {
            QString s=res[i];
            if (Operateur::isOperateur(s)){
                if(s=="EVAL" && pile.size()>0) applyOpEVAL();
                /*else if (OpBinaire::isOpBinaire(s)&& pile.size()<2) {
                    throw UTComputerException("Impossible d'appliquer l'opérateur (pas assez d'arguments)");
                }
                else if (OpUnaire::isOpUnaire(s)&& pile.size()==0) {
                    throw UTComputerException("Impossible d'appliquer l'opérateur (pas assez d'arguments)");
                }*/
                else if (OpPile::isOpPile(s)){
                    OpPile::applyOpPile(pile,factL,s);
                    QString message="opérateur effectué";
                    setMessage(message);
                }
                else if (OpUnaire::isOpUnaire(s) && pile.size()>0){
                    OpUnaire::applyOpUnaire(pile,factL,s);
                    QString message="opérateur effectué";
                    setMessage(message);
                }
                else if (OpBinaire::isOpBinaire(s) && pile.size()>1){
                    OpBinaire::applyOpBinaire(pile,factL,s);
                    QString message="opérateur effectué";
                    setMessage(message);
                }
                else {
                    throw UTComputerException("Impossible d'appliquer l'opérateur");
                }

            }
            else
            if (Litterale::isLitterale(s)) {
                if (Litterale::isLitteraleToPush(s)) {
                    Litterale* l=factL.makeLitt(s);
                    pile.push(l);
                }
                else if (LittAtome::isLittAtome(s)) {
                            LittAtome *l=factL.createLittAtome(s);
                            if (l->isVariable(l)) {
                                Litterale* l1=LittAtome::mapvariables.value(l->getId());
                                pile.push(l1);
                            }
                    else if (l->isProgramme(l)){
                        execProg(s);
                    }
                    else {
                        QString value=l->getId();
                        //delete l;
                        QString carac = "'";
                        QString exp = carac+value+carac;
                        LittExpression* e = factL.createLittExpression(exp);
                        //LittExpression *e= new LittExpression(exp);
                        //FactoryLitterale::tab.append(e);
                        pile.push(e);
                    }
                }
                else {
                    throw UTComputerException("Erreur dans la ligne de commande");
                }
                QString m = "Commande effectuée";
                setMessage(m);
            }


    else {
        throw UTComputerException("Erreur dans la ligne de commande");
    }
    }
}
}
    catch(UTComputerException e) {
        QString m=e.getException();
        setMessage(m);
    }
}

void Controleur::applyOpEVAL() {
        LittExpression* exp=dynamic_cast<LittExpression*>(pile.top());
        //FactoryLitterale::tab.append(exp);
        LittProgramme* pgr=dynamic_cast<LittProgramme*>(pile.top());
        //FactoryLitterale::tab.append(pgr);
        if (pgr){
            QString cpy=pgr->afficher();
            cpy.remove(0,1);
            cpy.chop(1);
            pile.pop();
            commande(cpy);
            QString message="opérateur effectué";
            setMessage(message);

        }
        else if (exp) {
            QString expr=exp->afficher();
            QString message="opérateur EVAL sur les expressions non défini";
            setMessage(message);

            /*expr.remove(0,1); // on enleve la premiere cote
            QStringList list=expr.split("");
            unsigned int stop=0;
            unsigned int i=0;
            while (i<list.size()&& stop==0) {
                QString c=list[i];
                QRegExp r("[0-9]|\\/|\\$");
                while(r.exactMatch(list[i])) {
                    c+=list[i+1];
                    i++;
                }
                if (Litterale::isLitterale(c))
                {
                    Litterale* a=factL.makeLitt(c);
                    pile.push(a);
                    factL.tab.append(a);
                }

            }*/

        }
        else {
           throw UTComputerException("Impossible d'appliquer l'opérateur EVAL sur une littérale d'un type autre que programme");
        }

}

void Controleur::execProg(QString s) {

    QString instructions=LittProgramme::mapprogrammes[s];
    instructions.remove(0,1);
    instructions.chop(1);
    //pile.pop();
    commande(instructions);
     QString m="exec programme"; setMessage(m);

}

