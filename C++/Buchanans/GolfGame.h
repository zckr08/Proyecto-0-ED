#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Card.h"
#include "LinkedStack.h"

class GolfGame {
private:
    LinkedStack<Card*> columnas[7];
    LinkedStack<Card*> mazo;
    LinkedStack<Card*> pozo;

public:

    GolfGame() {}

    ~GolfGame() {
        for (int i = 0; i < 7; i++) {
            while (!columnas[i].isEmpty()) {
                delete columnas[i].pop();
            }
        }
        while (!mazo.isEmpty()) {
            delete mazo.pop();
        }
        while (!pozo.isEmpty()) {
            delete pozo.pop();
        }
    }
    void iniciarJuego(Card* baraja[52]) {
        int i = 0;

        // reparte las cartas en las 7 columnas
        for (int col = 0; col < 7; col++) {
            for (int fila = 0; fila < 5; fila++) {
                Card* cartaActual = baraja[i++];
                cartaActual->unCover();
                cartaActual->flip();
                columnas[col].push(cartaActual);
            }
        }

        // inicia la primera carta pal pozo 
        Card* cartaInicial = baraja[i++];
        cartaInicial->unCover();
        cartaInicial->flip();
        pozo.push(cartaInicial); // la pushea en un stack 

        // mazo de las cartas que sobran
        while (i < 52) {
            Card* cartaMazo = baraja[i++];
            cartaMazo->unCover();
            mazo.push(cartaMazo); // las inserta en otro stack 
        }
    }
    bool esMovimientoValido(int entradaC) {
        if (entradaC < 0 || entradaC >= 7) return false;
        if (columnas[entradaC].isEmpty() || pozo.isEmpty()) {
            return false;
        }

        Card* cartaColumna = columnas[entradaC].topValue();
        Card* cartaActual = pozo.topValue();

        int v1 = cartaColumna->numero;
        int v2 = cartaActual->numero;

        // la regla de +1 -1 del golf
        return (v1 == v2 + 1) || (v1 == v2 - 1);
    }
    void moverCarta(int entradaC) {
        if (esMovimientoValido(entradaC)) {
            Card* cartaMovida = columnas[entradaC].pop();
            pozo.push(cartaMovida);
            std::cout << "Un saico la jugada! :)\n";
        }
        else {
            std::cout << "Movimiento no válido. Quizá no tan saico :(.\n";
        }
    }

    // funcion que manda al pozo 
    void sacarCartaMazo() {
        if (mazo.isEmpty()) {
            std::cout << " Ya no hay mas movimientos disponibles. :(\n";
            return;
        }

        Card* cartaMazo = mazo.pop();
        cartaMazo->flip();
        cartaMazo->unCover();
        pozo.push(cartaMazo);
        std::cout << "Maravillosa jugada! \n";
    }

    // veamos si ganó.
    bool haGanado() {
        for (int i = 0; i < 7; i++) {
            if (!columnas[i].isEmpty()) {
                return false;
            }
        }
        return true;
    }

    // si no ganó (Todavia)
    bool hayMovimientosDisponibles() {
        if (pozo.isEmpty()) return false;

        int PozoRest = pozo.topValue()->numero;

        for (int i = 0; i < 7; i++) {
            if (!columnas[i].isEmpty()) {
                int valorColumna = columnas[i].topValue()->numero;
                if (valorColumna == PozoRest + 1 || valorColumna == PozoRest - 1) {
                    return true;
                }
            }
        }
        return false;
    }

    // para ver como va el jueguito
    void mostrarTablero() {
        std::cout << "\n POZO  \n";
        if (!pozo.isEmpty()) {
            std::cout << "Cima del Pozo -> ";
            pozo.topValue()->draw();
        }
        else {
            std::cout << "Pozo Vacío\n";
        }

        std::cout << "\n COLUMNAS \n";
        for (int i = 0; i < 7; i++) {
            std::cout << "Columna " << i + 1 << " (Cartas restantes: " << columnas[i].getSize() << "):\n";
            if (!columnas[i].isEmpty()) {
                std::cout << "  Cima -> ";
                columnas[i].topValue()->draw();
            }
            else {
                std::cout << "  Columna Vacía\n";
            }
        }
        std::cout << "Lo que te falta mi hermano: " << mazo.getSize() << "\n";
    }
};
