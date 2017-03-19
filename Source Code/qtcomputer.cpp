#include "qtcomputer.h"

QtComputer::QtComputer() : QWidget()
{
    enter = new QPushButton("ENTER");
    button1 = new QPushButton("1");
    button2 = new QPushButton("2");
    button3 = new QPushButton("3");
    button4 = new QPushButton("4");
    button5 = new QPushButton("5");
    button6 = new QPushButton("6");
    button7 = new QPushButton("7");
    button8 = new QPushButton("8");
    button9 = new QPushButton("9");
    button0 = new QPushButton("0");
    drop = new QPushButton("DROP");
    lastop = new QPushButton("LASTOP");
    lastargs = new QPushButton("LASTARGS");
    swap = new QPushButton("SWAP");
    undo = new QPushButton("UNDO");
    sto=new QPushButton("STO");
    forget=new QPushButton("FORGET");
    clear = new QPushButton("CLEAR");
    backspace=new QPushButton ("BACKSPACE");
    dup = new QPushButton("DUP");
    dollar=new QPushButton("$");
    buttonadd = new QPushButton("+");
    buttonminus = new QPushButton("-");
    buttondiv = new QPushButton("/");
    buttonmult = new QPushButton("*");
    keyboard= new QWidget;
    stack=new QWidget;
    couche = new QVBoxLayout(this);
    coucheH = new QHBoxLayout(this);
    coucheH1 = new QHBoxLayout(this);
    coucheH2 = new QHBoxLayout(this);
    coucheH3 = new QHBoxLayout(this);
    coucheH4 = new QHBoxLayout(this);
    coucheH5 = new QHBoxLayout(this);
    coucheS=new QVBoxLayout(this);
    coucheK = new QVBoxLayout(this);
    message = new QLineEdit(this);
    commande = new QLineEdit(this);
    options = new QPushButton("Options");
    var= new QPushButton("Variables");
    prg= new QPushButton("Programmes");
    pile = new Pile;
    fl = new FactoryLitterale;
    controleur = new Controleur (*pile,*fl);
    m_bouton = new QPushButton("Quitter UTComputer", this);
    vuePile = new QTableWidget(pile->getNbAffiche(),1,this);




    this->setWindowTitle("UTComputer");
   setStyleSheet("QPushButton {background: pink; color:black}");
   message->setStyleSheet("background:lightcoral;color:white");
   // setStyleSheet("background: light yellow; color:white");
    vuePile->setEditTriggers(QAbstractItemView::NoEditTriggers); // Permet de dire qu'on ne peut pas écrire sur les lignes du tableau
    vuePile->horizontalHeader()->setVisible(false);    //Permet d'enlever l'entête des colonnes
    message->setReadOnly(true);
    message->setText(controleur->getMessage());
    vuePile->horizontalHeader()->setStretchLastSection(true);
    //vuePile->resizeRowsToContents();

    QStringList numberList;
    for(unsigned int i=0;i<pile->getNbAffiche()+1;i++){
        QString str = QString::number(i+1); // transforme le nombre en string
        str+= ":";
        numberList << str;
        vuePile->setItem(i-1,0,new QTableWidgetItem(""));
    }
    // affectation de la liste de labels ou header verticaux
    vuePile->setVerticalHeaderLabels(numberList);

    connect(enter, SIGNAL(clicked()), this, SLOT(getNextCommande()));
    connect(m_bouton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(commande, SIGNAL(returnPressed()),this,SLOT(getNextCommande()));
    connect(pile, SIGNAL(modificationPile()),this,SLOT(refresh()));
    connect(button1, SIGNAL(clicked()), this, SLOT(b1()));
    connect(button2, SIGNAL(clicked()), this, SLOT(b2()));
    connect(button3, SIGNAL(clicked()), this, SLOT(b3()));
    connect(button4, SIGNAL(clicked()), this, SLOT(b4()));
    connect(button5, SIGNAL(clicked()), this, SLOT(b5()));
    connect(button6, SIGNAL(clicked()), this, SLOT(b6()));
    connect(button7, SIGNAL(clicked()), this, SLOT(b7()));
    connect(button8, SIGNAL(clicked()), this, SLOT(b8()));
    connect(button9, SIGNAL(clicked()), this, SLOT(b9()));
    connect(button0, SIGNAL(clicked()), this, SLOT(b0()));
    connect(buttonadd, SIGNAL(clicked()), this, SLOT(badd()));
    connect(buttonminus, SIGNAL(clicked()), this, SLOT(bminus()));
    connect(buttondiv, SIGNAL(clicked()), this, SLOT(bdiv()));
    connect(buttonmult, SIGNAL(clicked()), this, SLOT(bmult()));
    connect(dup,SIGNAL(clicked(bool)),this,SLOT(dupslot()));
    connect(dollar,SIGNAL(clicked()),this,SLOT(dollarslot()));
    connect(options,SIGNAL(clicked()), this,SLOT(openOptionsWindow()));
    connect(backspace,SIGNAL(clicked()),this,SLOT(backspaceslot()));
    connect(clear, SIGNAL(clicked()), this, SLOT(clearslot()));
    connect(drop,SIGNAL(clicked()), this, SLOT(dropslot()));
    connect(sto, SIGNAL(clicked()), this, SLOT(stoslot()));
    connect(forget,SIGNAL(clicked()), this, SLOT(forgetslot()));
    connect(var, SIGNAL(clicked(bool)),this, SLOT(openVarWindow()));
    connect(prg, SIGNAL(clicked(bool)),this, SLOT(openPrgWindow()));
    connect(undo,SIGNAL(clicked(bool)),this,SLOT(undoslot()));
    connect(swap, SIGNAL(clicked(bool)),this,SLOT(swapslot()));
    connect(lastop,SIGNAL(clicked(bool)), this, SLOT(lastopslot()));
    connect(lastargs,SIGNAL(clicked(bool)),this, SLOT(lastargsslot()));

    coucheH->addWidget(button1);
    coucheH->addWidget(button2);
    coucheH->addWidget(button3);
    coucheH->addWidget(buttonadd);
    coucheH->addWidget(dup);
    coucheH->addWidget(swap);
    coucheH1->addWidget(commande);
    coucheH1->addWidget(backspace);
    coucheH1->addWidget(enter);
    coucheH2->addWidget(button4);
    coucheH2->addWidget(button5);
    coucheH2->addWidget(button6);
    coucheH2->addWidget(buttonminus);
    coucheH2->addWidget(drop);
    coucheH2->addWidget(lastop);
    coucheH3->addWidget(button7);
    coucheH3->addWidget(button8);
    coucheH3->addWidget(button9);
    coucheH3->addWidget(buttondiv);
    coucheH3->addWidget(sto);
    coucheH3->addWidget(lastargs);
    coucheH4->addWidget(dollar);
    coucheH4->addWidget(button0);
    coucheH4->addWidget(clear);
    coucheH4->addWidget(buttonmult);
    coucheH4->addWidget(forget);
    coucheH4->addWidget(undo);
    coucheH5->addWidget(var);
    coucheH5->addWidget(prg);

    coucheS->addWidget(message);
    coucheS->addWidget(vuePile);
    coucheS->addLayout(coucheH1);

    coucheK->addLayout(coucheH);
   coucheK->addLayout(coucheH2);
    coucheK->addLayout(coucheH3);
    coucheK->addLayout(coucheH4);
    coucheK->addLayout(coucheH5);


    keyboard->setLayout(coucheK);
    stack->setLayout(coucheS);
    couche->addWidget(stack);
    couche->addWidget(keyboard);
    couche->addWidget(options);
    couche->addWidget(m_bouton);
    setFixedSize(600,700);
    setLayout(couche);
    commande->setFocus();

}

void QtComputer::getNextCommande(){
    QString c = commande->text();
    if (c!="") controleur->commande(c);
    message->setText(controleur->getMessage());
    commande->clear();
    commande->setFocus();
}

void QtComputer::refresh(){

    for(unsigned int i=0; i<pile->getNbAffiche();i++)
        vuePile->item(i,0)->setText("");
    //mettre a jour
    unsigned int nb =0;
    QVector<Litterale*> liste = pile->stackToVector(pile->getNbAffiche());
    for (QVector<Litterale*>::const_iterator it = liste.begin(); it != liste.end() && nb<pile->getNbAffiche(); ++it,++nb)
        vuePile->item(nb,0)->setText((*it)->afficher());
}


void QtComputer::b1() {
    commande->insert("1");
    commande->setFocus();
}
void QtComputer::b2() {
    commande->insert("2");
    commande->setFocus();
}
void QtComputer::b3() {
    commande->insert("3");
    commande->setFocus();
}
void QtComputer::b4() {
    commande->insert("4");
    commande->setFocus();
}
void QtComputer::b5() {
    commande->insert("5");
    commande->setFocus();
}
void QtComputer::b6() {
    commande->insert("6");
    commande->setFocus();
}
void QtComputer::b7() {
    commande->insert("7");
    commande->setFocus();
}
void QtComputer::b8() {
    commande->insert("8");
    commande->setFocus();
}
void QtComputer::b9() {
    commande->insert("9");
    commande->setFocus();
}
void QtComputer::b0() {
    commande->insert("0");
    commande->setFocus();
}

void QtComputer::badd() {
    controleur->commande("+");
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::bminus() {
    controleur->commande("-");
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::bdiv() {
    controleur->commande("/");
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::bmult() {
    controleur->commande("*");
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::dupslot(){
    QString s="DUP";
    controleur->commande(s);
    message->setText(controleur->getMessage());
}
void QtComputer::swapslot(){
    QString s="SWAP";
    controleur->commande(s);
    message->setText(controleur->getMessage());
}

void QtComputer::lastopslot(){
    QString s="LASTOP";
    controleur->commande(s);
    message->setText(controleur->getMessage());
}
void QtComputer::lastargsslot(){
    QString s="LASTARGS";
    controleur->commande(s);
    message->setText(controleur->getMessage());
}
void QtComputer::undoslot(){
    QString s="UNDO";
    controleur->commande(s);
    message->setText(controleur->getMessage());
}

void QtComputer::dollarslot(){
    controleur->commande("$");
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::openOptionsWindow(){
    w= new OptionsWindow(this);
    w->show();

}
void QtComputer::openVarWindow(){
    w2= new VarWindow(this);
    w2->show();

}

void QtComputer::openPrgWindow(){
    w3= new PrgWindow(this);
    w3->show();

}

void QtComputer::backspaceslot(){
    QString tmp=commande->text();
    tmp.chop(1);
    commande->setText(tmp);
    commande->setFocus();
}

void QtComputer::clearslot(){
    QString s="CLEAR";
    controleur->commande(s);
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::dropslot(){
    QString s="DROP";
    controleur->commande(s);
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::stoslot(){
    QString s="STO";
    controleur->commande(s);
    message->setText(controleur->getMessage());
    commande->setFocus();
}

void QtComputer::forgetslot(){
    QString s="FORGET";
    controleur->commande(s);
    message->setText(controleur->getMessage());
    commande->setFocus();
}



