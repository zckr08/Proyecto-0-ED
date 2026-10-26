#pragma once
#include <iostream>
#include <ostream>
#include "Card.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "LinkedStack.h"
#include "Mazo.h"

using std::cout;
using std::endl;
using std::string;
using std::ostream;

class EstadoSpider {
public:
    ArrayList<LinkedList<Mazo*>*>* columnasEstado;
    LinkedStack<Card*>* mazoEstado;
    int escalerasCompletadas;
    int vecesRepartido;

    // Saca una "foto" del juego: copia las columnas y el mazo carta por carta,
    // y guarda cuántas escaleras van y cuántas veces se ha repartido.
    EstadoSpider(ArrayList<LinkedList<Mazo*>*>* columnas, LinkedStack<Card*>* mazo, int escaleras, int repartido) {
        escalerasCompletadas = escaleras;
        vecesRepartido = repartido;

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
            Card* carta = tempMazo.pop();
            mazo->push(carta);
            Card* cartaCopia = new Card(*carta);
            mazoEstado->push(cartaCopia);
        }
    }

    // Borra todas las cartas copiadas y las estructuras de la foto.
    // Se hace a mano porque las listas solo guardan punteros.
    ~EstadoSpider() {
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
    }

    // Permite imprimir el estado con cout, mostrando escaleras y repartos.
    friend ostream& operator<<(ostream& os, const EstadoSpider& estado) {
        os << "Escaleras completadas: " << estado.escalerasCompletadas;
        os << ", Veces repartido: " << estado.vecesRepartido;
        return os;
    }
};