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

int main3() {
	Card* baraja[104];
	crearBarajaSpider(baraja);
	mezclarBarajaSpider(baraja);

	Spider* juego = new Spider();
	juego->iniciarJuego(baraja);

	int opcion = 0;
	while (!juego->haGanado() && opcion != 5) {
		juego->mostrarTablero();
		cout << "Opcion: ";
		cin >> opcion;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			opcion = 0;
			continue;
		}
		if (opcion == 1) {
			int origen, destino, cantidad;
			cout << "Columna origen (1-10): ";
			cin >> origen;
			cout << "Columna destino (1-10): ";
			cin >> destino;
			cout << "Cantidad de cartas: ";
			cin >> cantidad;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
			if (juego->esMovimientoValido(origen - 1, destino - 1, cantidad)) {
				juego->moverCartas(origen - 1, destino - 1, cantidad);
			}
			else {
				cout << "Movimiento no valido." << endl;
				system("pause");
			}
		}
		else if (opcion == 2) {
			juego->repartirMazo();
		}
	}
	if (juego->haGanado()) cout << "¡Ganaste!" << endl;

	delete juego;
	return 0;
}