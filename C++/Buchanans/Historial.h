#pragma once
#include <stdexcept>
#include "DLinkedList.h"

using std::runtime_error;

template <typename E>
class Historial {
private:
    DLinkedList<E>* listaEstados;
    int indiceActual; // Controla en qué punto del historial estamos parados

public:
    Historial(const Historial<E>& otro) = delete;
    Historial<E>& operator=(const Historial<E>& otro) = delete;
    Historial() {
        listaEstados = new DLinkedList<E>();
        indiceActual = -1;
    }

    ~Historial() {
        while (listaEstados->getSize() > 0) {
            listaEstados->goToPos(listaEstados->getSize() - 1);
            delete listaEstados->remove();                 // libera el estado, no solo el nodo
        }
        delete listaEstados;
    }

    void guardarEstado(E nuevoEstado) {
        // borra y libera solo lo que está delante del indice
        while (listaEstados->getSize() > indiceActual + 1) {
            listaEstados->goToPos(listaEstados->getSize() - 1);
            delete listaEstados->remove();
        }
        listaEstados->append(nuevoEstado);
        indiceActual = listaEstados->getSize() - 1;
    }

   
    E deshacer() {
        if (indiceActual > 0) {
            indiceActual--;
            listaEstados->goToPos(indiceActual);
            return listaEstados->getElement();
        }

        throw runtime_error("No hay estados anteriores");
    }

    E rehacer() {
        if (indiceActual < listaEstados->getSize() - 1) {
            indiceActual++;
            listaEstados->goToPos(indiceActual);
            return listaEstados->getElement();
        }

        throw runtime_error("No hay estados siguientes");
    }

    E seleccionarEstado() {
        while (listaEstados->getSize() > indiceActual + 1) {
            listaEstados->goToPos(listaEstados->getSize() - 1);
            delete listaEstados->remove();
        }

        listaEstados->goToPos(indiceActual);
        return listaEstados->getElement();
    }

    int getIndiceActual() { return indiceActual; }
    int getTamano() { return listaEstados->getSize(); }
};