#include <iostream>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <string>
#include "GolfGame.h"
#include "Card.h"
#include "Spider.h"

using std::endl;
using std::getline;
using std::cin;
using namespace std;

void crearBarajaGolf(Card* baraja[52]) { //funcion para crear una baraja estandard de 52 cardas, mantener para la galeria del menu principal pls
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

// Mezcla la baraja del Golf de forma aleatoria
void mezclarBarajaGolf(Card* baraja[52]) {
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < 1000; i++) {
		int num1 = rand() % 52;
		int num2 = rand() % 52;

		Card* temp = baraja[num1];
		baraja[num1] = baraja[num2];
		baraja[num2] = temp;
	}
}

void crearBarajaSpider(Card* baraja[104]) {
	int b = 0;
	for (int t = 0; t < 8; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, 3); // Palo 3 (picas)
			b++;
		}
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

int main() {	//ahora es el menu principal
	try {
		string opcion;
		do { //un do  para que siempre que se termine una funcion regrese al menu principal
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|                BIENVENIDO AL SOLITARIO DE AXEL, MIKEL, ZACK y WIN ZHI                 |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|  ELIGA SU MODO DE JUEGO:                                                              |" << endl;
			cout << "|     1. SOLITARIO GOLF                                                                 |" << endl;
			cout << "|     2. SOLITARIO SPIDER                                                               |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl; //sirvio para verificar si el menu funciona correctamente
			cout << "|  OPCION:                                                                              |" << endl;
			cout << "|     3. Salir                                                                          |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;

			do {
				cout << "OPCION: ";
				getline(cin, opcion);
			} while (opcion != "1" && opcion != "2" && opcion != "3");


			if (opcion == "1") {
				system("cls"); //esto es para borrar todos que ya estaba impreso en la pantalla, para mantener el orden. Siempre ponerlo antes y despues de imprimir algo, para el orden

				GolfGame juegoGolf;

				Card* barajaGolf[52];
				crearBarajaGolf(barajaGolf);
				mezclarBarajaGolf(barajaGolf);
				juegoGolf.iniciarJuego(barajaGolf);

				string opcionGolf = "";
				do {
					juegoGolf.mostrarTablero();
					cout << "\nSELECCIONE UNA OPCION DEL JUEGO GOLF: ";
					getline(cin, opcionGolf);

					if (opcionGolf == "1") {
						string colO;
						cout << "Columna de origen (1-7): ";
						getline(cin, colO);

						try {
							int orig = stoi(colO) -1;
							juegoGolf.moverCarta(orig);
						}
						catch (...) {
							cout << "Entrada invalida. Presione ENTER para continuar...";
							string p; getline(cin, p);
						}
					}
					else if (opcionGolf == "2") {
						juegoGolf.sacarCartaMazo();
					}
					else if (opcionGolf == "3") {
						cout << "Opcion Deshacer en desarrollo. Presione ENTER para continuar...";
						string p; getline(cin, p);
					}
					else if (opcionGolf == "4") {
						cout << "Opcion Rehacer en desarrollo. Presione ENTER para continuar...";
						string p; getline(cin, p);
					}
				} while (opcionGolf != "5" && !juegoGolf.haGanado() && (juegoGolf.hayMovimientosDisponibles() || !juegoGolf.mazoVacio()));

				system("cls");
			}
			else if (opcion == "2") {
				system("cls");
				Spider juegoSpider;

				// Crear y rellenar la baraja de 104 cartas para Spider
				Card* barajaSpider[104];
				crearBarajaSpider(barajaSpider);
				mezclarBarajaSpider(barajaSpider);
				juegoSpider.iniciarJuego(barajaSpider);

				// BUCLE INTERACTIVO PARA JUGAR SPIDER
				string opcionSpider = "";
				do {
					juegoSpider.mostrarTablero();
					cout << "\nSELECCIONE UNA OPCION DEL JUEGO SPIDER: ";
					getline(cin, opcionSpider);

					if (opcionSpider == "1") {
						string colO, colD, cant;
						cout << "Columna origen (1-10): ";
						getline(cin, colO);
						cout << "Columna destino (1-10): ";
						getline(cin, colD);
						cout << "Cantidad de cartas: ";
						getline(cin, cant);

						try {
							int orig = stoi(colO) - 1;
							int dest = stoi(colD) - 1;
							int cantidad = stoi(cant);
							juegoSpider.moverCartas(orig, dest, cantidad);
						}
						catch (...) {
							cout << "Entrada invalida. Presione ENTER para continuar...";
							string p; getline(cin, p);
						}
					}
					else if (opcionSpider == "2") {
						juegoSpider.repartirMazo();
					}
					else if (opcionSpider == "3") {
						cout << "Opcion Deshacer en desarrollo. Presione ENTER para continuar...";
						string p; getline(cin, p);
					}
					else if (opcionSpider == "4") {
						cout << "Opcion Rehacer en desarrollo. Presione ENTER para continuar...";
						string p; getline(cin, p);
					}

				} while (opcionSpider != "5" && !juegoSpider.haGanado());


				system("cls");
			}


			else if (opcion == "3") {
				system("cls");
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				cout << "|                          GRACIAS POR USAR, QUE DIOS LE BENDIGA                        |" << endl;
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				return 0;
			}
		} while (opcion != "3");
	}
	catch (exception& e) {
		cout << "Ha ocurrido un error relacionado con: " << e.what() << "\n";
	}
	return 0;
}