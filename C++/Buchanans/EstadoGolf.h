#pragma once
#include <iostream>
#include <ostream>
#include "Card.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "ArrayList.h"
#include "Mazo.h"

using std::cout;
using std::endl;
using std::string;
using std::ostream;

class EstadoGolf {
public:
    ArrayList<LinkedList<Mazo*>*>* columnasEstado;
    LinkedStack<Card*>* mazoEstado;
    LinkedStack<Card*>* pozoEstado;
    int puntuacion;

    // Saca una "foto" del juego: copia columnas, mazo y pozo carta por carta.
    // Así lo que pase después en el juego no cambia esta foto.
    EstadoGolf(ArrayList<LinkedList<Mazo*>*>* columnas, LinkedStack<Card*>* mazo, LinkedStack<Card*>* pozo, int puntuacion) {
        this->puntuacion = puntuacion;

        columnasEstado = new ArrayList<LinkedList<Mazo*>*>();
        for (int i = 0; i < columnas->getSize(); i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* listaOriginal = columnas->getElement()->getElement()->cartas;
            LinkedList<Card*>* listaClonada = new LinkedList<Card*>();
            listaOriginal->goToStart();
            while (!listaOriginal->atEnd()) {
                Card* cartaOriginal = listaOriginal->getElement();
                Card* cartaCopia = new Card(*cartaOriginal);
                listaClonada->append(cartaCopia);
                listaOriginal->next();
            }
            LinkedList<Mazo*>* grupo = new LinkedList<Mazo*>();
            grupo->append(new Mazo(listaClonada));
            columnasEstado->append(grupo);
        }

        // Pasamos el mazo a una pila temporal y de vuelta para copiarlo sin cambiar su orden.
        mazoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempMazo;
        while (!mazo->isEmpty()) {
            tempMazo.push(mazo->pop());
        }
        while (!tempMazo.isEmpty()) {
            Card* cartaOriginal = tempMazo.pop();
            mazo->push(cartaOriginal);
            Card* cartaCopia = new Card(*cartaOriginal);
            mazoEstado->push(cartaCopia);
        }

        // Lo mismo que con el mazo, pero para el pozo.
        pozoEstado = new LinkedStack<Card*>();
        LinkedStack<Card*> tempPozo;
        while (!pozo->isEmpty()) {
            tempPozo.push(pozo->pop());
        }
        while (!tempPozo.isEmpty()) {
            Card* cartaOriginal = tempPozo.pop();
            pozo->push(cartaOriginal);
            Card* cartaCopia = new Card(*cartaOriginal);
            pozoEstado->push(cartaCopia);
        }
    }

    // Borra todas las cartas copiadas y las estructuras de la foto.
    // Se hace a mano porque las listas solo guardan punteros.
    ~EstadoGolf() {
        for (int i = 0; i < columnasEstado->getSize(); i++) {
            columnasEstado->goToPos(i);
            LinkedList<Mazo*>* grupo = columnasEstado->getElement();
            Mazo* columna = grupo->getElement();
            columna->cartas->goToStart();
            while (!columna->cartas->atEnd()) {
                delete columna->cartas->getElement();
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

    // Permite imprimir el estado con cout, mostrando solo la puntuación.
    friend ostream& operator<<(ostream& os, const EstadoGolf& estado) {
        os << "Puntuacion: " << estado.puntuacion;
        return os;
    }
};