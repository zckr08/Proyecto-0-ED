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

void crearBaraja1(Card* baraja[52]) { //funcion para crear una baraja estandard de 52 cardas, mantener para la galeria del menu principal pls
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

int main3() {	//ahora es el menu principal
	try {
		string opcion;
		do { //un do  para que siempre que se termine una funcion regrese al menu principal
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|                BIENVENIDO AL SOLITARIO DE AXEL, MIKEL, ZACK y WIN ZHI                 |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|  ELIGA SU MODO DE JUEGO:                                                              |" << endl;
			cout << "|     1. SOLITARIO GOLF                                                                 |" << endl;
			cout << "|     2. SOLITARIO SPIDER                                                               |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|  EXTRAS:                                                                              |" << endl;
			cout << "|     3. GALERIA DE CARTAS                                                              |" << endl; //esto como un extra, ademas que me
			cout << "+---------------------------------------------------------------------------------------+" << endl; //sirvio para verificar si el menu funciona correctamente
			cout << "|  OPCION:                                                                              |" << endl;
			cout << "|     4. Salir                                                                          |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;

			do {
				cout << "OPCION: ";
				getline(cin, opcion);
			} while (opcion != "1" && opcion != "2" && opcion != "3" && opcion != "4");


			if (opcion == "1") {
				system("cls"); //esto es para borrar todos que ya estaba impreso en la pantalla, para mantener el orden. Siempre ponerlo antes y despues de imprimir algo, para el orden

				// Aqui iria la funcion que lleva al solitario golf

				system("cls");
			}
			else if (opcion == "2") {
				system("cls");
				Spider juegoSpider;

				// Crear y rellenar la baraja de 104 cartas para Spider
				Card* barajaSpider[104];
				int index = 0;
				for (int m = 0; m < 2; m++) { // 2 mazos de 52 cartas para Spider
					for (int t = 0; t < 4; t++) {
						for (int i = 1; i < 14; i++) {
							barajaSpider[index] = new Card(i, t);
							index++;
						}
					}
				}

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

				// Limpiar memoria de la baraja Spider
				for (int i = 0; i < 104; i++) {
					delete barajaSpider[i];
				}

				system("cls");
			}
			else if (opcion == "3") { //esto es solo para probar la funcionalidad del menu, solo imprime las cartas para hacer una especie de galeria
				system("cls");
				Card* baraja[52];
				crearBaraja1(baraja);
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				cout << "|                                   GALERIA DE CARTAS                                   |" << endl;
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				for (int i = 0; i < 52; i++) {
					baraja[i]->unCover();
					baraja[i]->flip();
					baraja[i]->draw();
				}
				baraja[0]->flip();
				baraja[0]->draw();
				deleteBaraja(baraja);
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				cout << "|  OPCION:                                                                              |" << endl;
				cout << "|     1. VOLVER                                                                         |" << endl;
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				do {
					cout << "OPCION: ";
					getline(cin, opcion);
				} while (opcion != "1");
				system("cls");
			}


			else if (opcion == "4") {
				system("cls");
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				cout << "|                          GRACIAS POR USAR, QUE DIOS LE BENDIGA                        |" << endl;
				cout << "+---------------------------------------------------------------------------------------+" << endl;
				return 0;
			}
		} while (opcion != "4");
	}
	catch (exception& e) {
		cout << "Ha ocurrido un error relacionado con: " << e.what() << "\n";
	}
	return 0;
}