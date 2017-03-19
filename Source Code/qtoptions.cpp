#include "qtoptions.h"
#include "qtcomputer.h"

OptionsWindow::OptionsWindow(QtComputer* m){
    main=m;
    test=new QPushButton("Changer");
    texte=new QLabel("Changer le nombre de littérales à afficher");
    texte2=new QLabel("afficher le clavier");
    quitter=new QPushButton("Quitter les parametres");
    oui = new QRadioButton ("oui");
    non = new QRadioButton("non");
    if (main->keyboard->isVisible()) {
        oui->setChecked(true);
    }
    else {
        non->setChecked(true);
    }
    couche1= new QVBoxLayout(this);
    coucheH1=new QHBoxLayout(this);
    num= new QSpinBox;
    num->setMinimum(1);
    num->setValue(main->vuePile->rowCount());
    connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
    connect(num,SIGNAL(valueChanged(int)),this, SLOT(changePile(int)));
    connect(oui,SIGNAL(clicked()),this,SLOT(showKeyboard()));
    connect(non,SIGNAL(clicked()),this,SLOT(disableKeyboard()));
    coucheH1->addWidget(oui);
    coucheH1->addWidget(non);
    couche1->addWidget(texte);
    couche1->addWidget(num);
    couche1->addWidget(texte2);
    couche1->addLayout(coucheH1);
    couche1->addWidget(quitter);
    setLayout(couche1);
}

void OptionsWindow::changePile(int nb){

    for(int i=0; i<main->vuePile->rowCount(); i++) {
        QTableWidgetItem* item = main->vuePile->item(i,0);
        delete item;
    }
    main->vuePile->setRowCount(nb);
    main->pile->setNbAffiche(nb);
    for(int i=0; i<main->vuePile->rowCount(); i++)
        main->vuePile->setItem(i,0,new QTableWidgetItem(""));
    main->refresh();

}

void OptionsWindow::showKeyboard() {
    main->keyboard->setVisible(true);
}

void OptionsWindow::disableKeyboard(){
    main->keyboard->hide();
}

