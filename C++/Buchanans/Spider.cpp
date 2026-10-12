#pragma once

#include "Spider.h"
#include "Card.h"
#include <cstdlib>
#include <ctime>

// Mikel estuvo aquí 
void crearBarajaSpider(Card* baraja[104]) { // crea la baraja para el Spider
    int b = 0;
    for (int t = 0; t < 8; t++) {
        for (int i = 1; i < 14; i++) {
            baraja[b] = new Card(i, 3);
            b++;
        }
    }
}

void deleteBarajaSpider(Card* baraja[104]) { // borra la baraja pal spider
    for (int i = 0; i < 104; i++) {
        delete baraja[i];
    }
}

void mezclarBarajaSpider(Card* baraja[104]) { // mezcla la baraja pal spider
    srand(time(nullptr));
    for (int i = 0; i < 1000; i++) {
        int num1 = rand() % 104;
        int num2 = rand() % 104;

        Card* temp = baraja[num1];
        baraja[num1] = baraja[num2];
        baraja[num2] = temp;
    }
}
