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
       
        delete listaEstados;
    }

    // Guarda un nuevo estado y borra el futuro si estábamos en un punto anterior, una locura
    void guardarEstado(E nuevoEstado) {
        // Borra solo lo que está delante del indice
        while (listaEstados->getSize() > indiceActual + 1) {
            listaEstados->goToEnd();
            listaEstados->remove();
        }

        listaEstados->append(nuevoEstado);
        indiceActual = listaEstados->getSize() - 1;
    }

    E deshacer() {
        if (indiceActual <= 0) {throw runtime_error("No hay estados anteriores");}
        indiceActual--;
        listaEstados->goToPos(indiceActual);
        return listaEstados->getElement();
    }

   
    E rehacer() {
        if (indiceActual >= listaEstados->getSize() - 1) {throw runtime_error("No hay estados siguientes");}
        indiceActual++;
        listaEstados->goToPos(indiceActual);
        return listaEstados->getElement();
    }

    int getIndiceActual() { return indiceActual; }
    int getTamano() { return listaEstados->getSize(); }
};