#pragma once
#include "Card.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "ArrayList.h"

class EstadoGolf {
public:
    ArrayList<LinkedList<Card*>*>* columnasEstado;
    LinkedStack<Card*>* mazoEstado;
    LinkedStack<Card*>* pozoEstado;

   
    EstadoGolf(ArrayList<LinkedList<Card*>*>* columnas, LinkedStack<Card*>* mazo, LinkedStack<Card*>* pozo) {

       
        columnasEstado = new ArrayList<LinkedList<Card*>*>();
        for (int i = 0; i < columnas->getSize(); i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* listaOriginal = columnas->getElement();

            LinkedList<Card*>* listaClonada = new LinkedList<Card*>();
            listaOriginal->goToStart();
            while (!listaOriginal->atEnd()) {
                listaClonada->append(listaOriginal->getElement());
                listaOriginal->next();
            }
            columnasEstado->append(listaClonada);
        }

      
        mazoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempMazo;
        while (!mazo->isEmpty()) {
            tempMazo.push(mazo->pop());
        }
        while (!tempMazo.isEmpty()) {
            Card* carta = tempMazo.pop();
            mazo->push(carta);   // Devuelve la carta al original para dejarlo intacto
            mazoEstado->push(carta);    // Guarda la carta en la copia del estado
        }

       
        pozoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempPozo;
        while (!pozo->isEmpty()) {
            tempPozo.push(pozo->pop());
        }
        while (!tempPozo.isEmpty()) {
            Card* carta = tempPozo.pop();
            pozo->push(carta);   // Devuelve la carta al original para dejarlo intacto
            pozoEstado->push(carta);    // Guarda la carta en la copia del estado
        }
    }

  
    ~EstadoGolf() {
        for (int i = 0; i < columnasEstado->getSize(); i++) {
            columnasEstado->goToPos(i);
            delete columnasEstado->getElement();
        }
        delete columnasEstado;

        delete mazoEstado;
        delete pozoEstado;
    }
};