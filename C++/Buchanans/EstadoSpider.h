#pragma once
#include "Card.h"
#include "DLinkedList.h"
#include "LinkedStack.h"

class EstadoSpider {
public:
 
    DLinkedList<Card*>* columnasEstado[10];
    LinkedStack<Card*>* mazoEstado;
    int escalerasCompletadas;

   
    EstadoSpider(DLinkedList<Card*>* columnas[10], LinkedStack<Card*>* mazo, int escaleras) {
        escalerasCompletadas = escaleras;

      
        for (int i = 0; i < 10; i++) {
            columnasEstado[i] = new DLinkedList<Card*>();

            columnas[i]->goToStart();
            while (!columnas[i]->atEnd()) {
                columnasEstado[i]->append(columnas[i]->getElement()); 
                columnas[i]->next();
            }
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
    }

   
    ~EstadoSpider() {
      
        for (int i = 0; i < 10; i++) {
            delete columnasEstado[i];
        }

     
        delete mazoEstado;
    }
};