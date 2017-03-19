#ifndef VARWINDOW_H
#define VARWINDOW_H
#include <qtcomputer.h>
#include <QWidget>

class QtComputer;

/*!
 * Classe pour la fenetre des variables
 */
class VarWindow : public QWidget{
        Q_OBJECT;
private:
    QTableWidget* variables;
    QtComputer* main;
    QPushButton* quitter;
    QVBoxLayout* couche;
public:
    VarWindow(QtComputer *m);
    void getVariables();
};

#endif // VARWINDOW_H
