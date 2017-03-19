#include "pile.h"
#include "litterale.h"

QVector<Litterale*> Pile::stackToVector(unsigned int n) const {
    //on copie la stack de la pile
    QStack<Litterale*> cpy=p;
    QVector<Litterale*> res;
    unsigned int i=0;
    while (i<n && cpy.size()>0) {
        Litterale* l=cpy.top(); cpy.pop();
        res.push_front(l);
        i++;
    }
    return res;
}

void Pile::push(Litterale* l)
{
    p.push(l);
    modificationPile();
}
