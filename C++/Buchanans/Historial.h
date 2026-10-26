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
            delete listaEstados->remove();
        }
        delete listaEstados;
    }

    void guardarEstado(E nuevoEstado) {
        // Borra los estados adelantados (si se hizo deshacer y luego un nuevo movimiento)
        while (listaEstados->getSize() > indiceActual + 1) {
            listaEstados->goToPos(listaEstados->getSize() - 1);
            delete listaEstados->remove();
        }
        listaEstados->append(nuevoEstado);
        indiceActual = listaEstados->getSize() - 1;
    }

    E deshacer() {
        // CORRECCIÓN: Permite retroceder siempre que el índice sea mayor a 0, 
        // llegando exactamente hasta el estado inicial (índice 0).
        if (indiceActual > 0) {
            indiceActual--;
            listaEstados->goToPos(indiceActual);
            return listaEstados->getElement();
        }

        throw runtime_error("No hay estados anteriores");
    }

    E rehacer() {
        // Permite avanzar si hay estados por delante en el historial
        if (indiceActual < listaEstados->getSize() - 1) {
            indiceActual++;
            listaEstados->goToPos(indiceActual);
            return listaEstados->getElement();
        }

        throw runtime_error("No hay estados siguientes");
    }

    int getIndiceActual() { return indiceActual; }
    int getTamano() { return listaEstados->getSize(); }
};