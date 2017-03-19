#include <QApplication>
#include <QPushButton>
#include "qtcomputer.h"
#include "controleur.h"
#include"pile.h"
#include "litterale.h"
#include "operateur.h"

using namespace std;
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

   QtComputer fenetre;
    fenetre.show();

    return app.exec();
}
