
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Card.h"


void crearBaraja(Card* baraja[52]) { //funcion para crear una baraja
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

void deleteBaraja(Card* baraja[52]) { //borrar la baraja porque esta en memoria dinamica
	for (int i = 0; i < 52; i++)
		delete baraja[i];
}

void mezclarBaraja(Card* baraja[52]) {
	srand(time(nullptr));
	for (int i = 0; i < 1000; i++) {
		int num1;
		int num2;
		Card* temp;
		do {
			num1 = rand() % 52;
			num2 = rand() % 52;
		} while (num2 == num1);

		temp = baraja[num1];
		baraja[num1] = baraja[num2];
		baraja[num2] = temp;
	}
}

int main() {
	Card* baraja[52];

	crearBaraja(baraja);

	mezclarBaraja(baraja);
	for (int i = 0; i < 52; i++) {
		baraja[i]->unCover();
		baraja[i]->draw();
	}
	deleteBaraja(baraja);

}
