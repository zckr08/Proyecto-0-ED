#pragma once
#include <iostream>
#include <ostream>
#include "Card.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "ArrayList.h"
#include "Mazo.h"

// guarda una copia del tablero del golf en un momento, para deshacer/rehacer
class EstadoGolf {
public:
    ArrayList<LinkedList<Mazo*>*>* columnasEstado;
    LinkedStack<Card*>* mazoEstado;
    LinkedStack<Card*>* pozoEstado;
    int puntuacion;

    EstadoGolf(ArrayList<LinkedList<Mazo*>*>* columnas, LinkedStack<Card*>* mazo, LinkedStack<Card*>* pozo, int puntuacion) {
        this->puntuacion = puntuacion;

        // copia las columnas, con la misma estructura que GolfGame (lista con un Mazo)
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
            Card* cartaOriginal = tempMazo.pop();
            mazo->push(cartaOriginal);                  // devuelve la carta al mazo original
            Card* cartaCopia = new Card(*cartaOriginal);
            mazoEstado->push(cartaCopia);
        }

        // copia el pozo, igual que el mazo
        pozoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempPozo;
        while (!pozo->isEmpty()) {
            tempPozo.push(pozo->pop());
        }
        while (!tempPozo.isEmpty()) {
            Card* cartaOriginal = tempPozo.pop();
            pozo->push(cartaOriginal);                  // devuelve la carta al pozo original
            Card* cartaCopia = new Card(*cartaOriginal);
            pozoEstado->push(cartaCopia);
        }
    }

    // el estado es dueño de sus copias, asi que las libera todas
    ~EstadoGolf() {
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
        while (!pozoEstado->isEmpty()) {
            delete pozoEstado->pop();
        }
        delete pozoEstado;
    }
    friend std::ostream& operator<<(std::ostream& os, const EstadoGolf& estado) {
        os << "Puntuacion: " << estado.puntuacion;
        return os;
    }
};