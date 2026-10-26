#pragma once
#include <iostream>
#include <ostream>
#include "Card.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "LinkedStack.h"
#include "Mazo.h"

// guarda una copia del tablero del spider en un momento, para deshacer/rehacer
class EstadoSpider {
public:
    ArrayList<LinkedList<Mazo*>*>* columnasEstado;
    LinkedStack<Card*>* mazoEstado;
    int escalerasCompletadas;
    int vecesRepartido;

    EstadoSpider(ArrayList<LinkedList<Mazo*>*>* columnas, LinkedStack<Card*>* mazo, int escaleras, int repartido) {
        escalerasCompletadas = escaleras;
        vecesRepartido = repartido;

        // copia las columnas, con la misma estructura que Spider (lista con un Mazo)
        columnasEstado = new ArrayList<LinkedList<Mazo*>*>();
        for (int i = 0; i < columnas->getSize(); i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* listaOriginal = columnas->getElement()->getElement()->cartas;
            LinkedList<Card*>* listaClonada = new LinkedList<Card*>();
            listaOriginal->goToStart();
            while (!listaOriginal->atEnd()) {
                Card* cartaOriginal = listaOriginal->getElement();
                Card* cartaCopia = new Card(*cartaOriginal);   // copia de la carta, no el mismo puntero
                listaClonada->append(cartaCopia);
                listaOriginal->next();
            }
            LinkedList<Mazo*>* grupo = new LinkedList<Mazo*>();
            grupo->append(new Mazo(listaClonada));
            columnasEstado->append(grupo);
        }

        // copia el mazo: se pasa a una pila temporal para no invertir el orden
        mazoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempMazo;
        while (!mazo->isEmpty()) {
            tempMazo.push(mazo->pop());
        }
        while (!tempMazo.isEmpty()) {
            Card* carta = tempMazo.pop();
            mazo->push(carta);                  // devuelve la carta al mazo original
            Card* cartaCopia = new Card(*carta);
            mazoEstado->push(cartaCopia);
        }
    }

    // el estado es dueño de sus copias, asi que las libera todas
    ~EstadoSpider() {
        for (int i = 0; i < columnasEstado->getSize(); i++) {
            columnasEstado->goToPos(i);
            LinkedList<Mazo*>* grupo = columnasEstado->getElement();
            Mazo* columna = grupo->getElement();
            columna->cartas->goToStart();
            while (!columna->cartas->atEnd()) {
                delete columna->cartas->getElement();   // primero las cartas
                columna->cartas->next();
            }
            delete columna->cartas;
            delete columna;
            delete grupo;
        }
        delete columnasEstado;
        while (!mazoEstado->isEmpty()) {
            delete mazoEstado->pop();
        }
        delete mazoEstado;
    }
    friend std::ostream& operator<<(std::ostream& os, const EstadoSpider& estado) {
        os << "Escaleras completadas: " << estado.escalerasCompletadas;
        os << ", Veces repartido: " << estado.vecesRepartido;
        return os;
    }
};
