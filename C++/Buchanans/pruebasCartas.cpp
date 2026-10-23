#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Card.h"
#include "GolfGame.h"

using std::cout;
using std::cin;
using std::endl;

void crearBaraja(Card* baraja[52]) { //funcion para crear una baraja estandard de 52 cardas, mantener para la galeria del menu principal pls
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

void mezclarBaraja(Card* baraja[52]) {
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < 1000; i++) {
		int num1 = rand() % 52;
		int num2 = rand() % 52;

		Card* temp = baraja[num1];
		baraja[num1] = baraja[num2];
		baraja[num2] = temp;
	}
}

int main() {
	Card* baraja[52];
	crearBaraja(baraja);
	mezclarBaraja(baraja);

	GolfGame* juego = new GolfGame();
	juego->iniciarJuego(baraja);

	int entrada = -1;
	while (!juego->haGanado() && entrada != 0) {
		juego->mostrarTablero();
		if (!juego->hayMovimientosDisponibles()) {
			cout << "No hay movimientos disponibles en las columnas." << endl;
			break;
		}
		cout << "Columna (1-7, 0 para salir): ";
		cin >> entrada;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			entrada = -1;
			continue;
		}
		if (entrada != 0) juego->moverCarta(entrada - 1);
	}
	if (juego->haGanado()) cout << "¡Ganaste!" << endl;

	delete juego;   // el destructor libera las 52 cartas, no hagas delete baraja[i]
}