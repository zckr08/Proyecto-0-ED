#pragma once
#include "Card.h"
#include "DLinkedList.h"
#include "LinkedStack.h"

class EstadoSpider {
public:
 
    DLinkedList<Card*>* columnasEstado[10];
    LinkedStack<Card*>* mazoEstado;
    int escalerasCompletadas;

   
    EstadoSpider(DLinkedList<Card*>* columnasActuales[10], LinkedStack<Card*>* mazoActual, int escaleras) {
        escalerasCompletadas = escaleras;

      
        for (int i = 0; i < 10; i++) {
            columnasEstado[i] = new DLinkedList<Card*>();

            columnasActuales[i]->goToStart();
            while (!columnasActuales[i]->atEnd()) {
                columnasEstado[i]->append(columnasActuales[i]->getElement()); 
                columnasActuales[i]->next();
            }
        }

       
        mazoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempMazo;

        while (!mazoActual->isEmpty()) {
            tempMazo.push(mazoActual->pop());
        }
        while (!tempMazo.isEmpty()) {
            Card* carta = tempMazo.pop();
            mazoActual->push(carta);   // Devuelve la carta al original para dejarlo intacto
            mazoEstado->push(carta);    // Guarda la carta en la copia del estado
        }
    }

   
    ~EstadoSpider() {
      
        for (int i = 0; i < 10; i++) {
            delete columnasEstado[i];
        }

     
        delete mazoEstado;
    }
};