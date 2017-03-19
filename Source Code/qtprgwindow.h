#ifndef PRGWINDOW_H
#define PRGWINDOW_H
#include <qtcomputer.h>
#include <QWidget>

class QtComputer;

/*!
 * Classe pour la fenetre des programmes
 */
class PrgWindow : public QWidget{
        Q_OBJECT;
private:
    QTableWidget* programmes;
    QtComputer* main;
    QPushButton* quitter;
    QVBoxLayout* couche;
public:
    PrgWindow(QtComputer *m);
    void getProgrammes();
};



#endif // PRGWINDOW_H
