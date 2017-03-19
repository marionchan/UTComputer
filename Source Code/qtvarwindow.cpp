#include "qtcomputer.h"
#include "qtvarwindow.h"
#include "litterale.h"


VarWindow::VarWindow(QtComputer *m){
    main=m;
    quitter=new QPushButton("Quitter les variables");
    couche=new QVBoxLayout(this);
    connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
    //connect(LittAtome::addVariable(),SIGNAL(modificationmapp()),this, SLOT(modif()));
    variables = new QTableWidget(LittAtome::mapvariables.size(),2,this);
    getVariables();
   couche->addWidget(variables);
   couche->addWidget(quitter);
   setFixedSize(500,500);
   setLayout(couche);
   variables->setHorizontalHeaderItem(0, new QTableWidgetItem("Identificateur"));
   variables->setHorizontalHeaderItem(1, new QTableWidgetItem("Variable"));
    variables->horizontalHeader()->setStretchLastSection(true);
}

void VarWindow::getVariables(){
    //variables->setRowCount(LittAtome::mapvariables.size());

    //for(unsigned int i=0; i<LittAtome::mapvariables.size();i++)
        //variables->setItem(0,0,new QTableWidgetItem("ok"));
        //variables->item(0,0)->setText("ok");
       int i=0;
       for(QMap<QString,Litterale*>::iterator it=LittAtome::mapvariables.begin(); it!=LittAtome::mapvariables.end(); ++it) {
               //QtWidgetItem* item1 = new QUTTableWidgetItem(QString::fromStdString((*it1)->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
               //QUTTableWidgetItem* item2 = new QUTTableWidgetItem(QString::fromStdString((*it1)->getValeur()->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
               //vars->setItem(i,0,item1);
               //vars->setItem(i,1,item2);
               //i++;
           variables->setItem(i,0,new QTableWidgetItem(""));
            variables->item(i,0)->setText(it.key());
            variables->setItem(i,1,new QTableWidgetItem(""));
           variables->item(i,1)->setText(it.value()->afficher());
           i++;

       }
}
