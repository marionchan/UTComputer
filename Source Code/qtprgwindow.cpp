#include "qtprgwindow.h"
#include "qtcomputer.h"
#include "litterale.h"


PrgWindow::PrgWindow(QtComputer *m){
    main=m;
    quitter=new QPushButton("Quitter les programmes");
    couche=new QVBoxLayout(this);
    connect(quitter, SIGNAL(clicked()), this, SLOT(close()));
    programmes = new QTableWidget(LittProgramme::mapprogrammes.size(),2,this);
    getProgrammes();
   couche->addWidget(programmes);
   couche->addWidget(quitter);
   setFixedSize(500,500);
   setLayout(couche);
   programmes->setHorizontalHeaderItem(0, new QTableWidgetItem("Identificateur"));
   programmes->setHorizontalHeaderItem(1, new QTableWidgetItem("Programme"));
   programmes->horizontalHeader()->setStretchLastSection(true);
}

void PrgWindow::getProgrammes(){

       int i=0;
       for(QMap<QString,QString>::iterator it=LittProgramme::mapprogrammes.begin(); it!=LittProgramme::mapprogrammes.end(); ++it) {
               //QtWidgetItem* item1 = new QUTTableWidgetItem(QString::fromStdString((*it1)->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
               //QUTTableWidgetItem* item2 = new QUTTableWidgetItem(QString::fromStdString((*it1)->getValeur()->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
               //vars->setItem(i,0,item1);
               //vars->setItem(i,1,item2);
               //i++;
           programmes->setItem(i,0,new QTableWidgetItem(""));
            programmes->item(i,0)->setText(it.key());
            programmes->setItem(i,1,new QTableWidgetItem(""));
           programmes->item(i,1)->setText(it.value());
           i++;

       }
}
