#pragma once
#include "Spider.h"
#include "Card.h"
#include <cstdlib>
#include <ctime>

// Mikel estuvo aquí: crea la baraja para el Spider (104 cartas, 8 veces del 1 al 13)
void crearBarajaSpider(Card* baraja[104]) {
    int b = 0;
    for (int t = 0; t < 8; t++) {
        for (int i = 1; i < 14; i++) {
            baraja[b] = new Card(i, 3); // Palo 3 (picas)
            b++;
        }
    }
}

// Borra la baraja del Spider para liberar memoria
void deleteBarajaSpider(Card* baraja[104]) {
    for (int i = 0; i < 104; i++) {
        delete baraja[i];
    }
}

// Mezcla la baraja del Spider de forma aleatoria
void mezclarBarajaSpider(Card* baraja[104]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 1000; i++) {
        int num1 = rand() % 104;
        int num2 = rand() % 104;

        Card* temp = baraja[num1];
        baraja[num1] = baraja[num2];
        baraja[num2] = temp;
    }
}