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
   
};