#include <iostream>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <exception>
#include <string>
#include <stdexcept>
#include <windows.h>
#include "GolfGame.h"
#include "EstadoGolf.h"
#include "Card.h"
#include "Spider.h"
#include "EstadoSpider.h"
#include "Historial.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::exception;
using std::runtime_error;

// Crea las 52 cartas de Golf: 13 números por cada uno de los 4 palos.
void crearBarajaGolf(Card* baraja[52]) {
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

// Revuelve las cartas cambiando de lugar dos al azar, 1000 veces.
void mezclarBarajaGolf(Card* baraja[52]) {
	for (int i = 0; i < 1000; i++) {
		int num1 = rand() % 52;
		int num2 = rand() % 52;
		Card* temp = baraja[num1];
		baraja[num1] = baraja[num2];
		baraja[num2] = temp;
	}
}

// Crea las 104 cartas de Spider: 8 juegos de 13, todos del mismo palo (picas).
void crearBarajaSpider(Card* baraja[104]) {
	int b = 0;
	for (int t = 0; t < 8; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, 3);
			b++;
		}
	}
}

// Revuelve las 104 cartas igual que en Golf.
void mezclarBarajaSpider(Card* baraja[104]) {
	for (int i = 0; i < 1000; i++) {
		int num1 = rand() % 104;
		int num2 = rand() % 104;
		Card* temp = baraja[num1];
		baraja[num1] = baraja[num2];
		baraja[num2] = temp;
	}
}

// Toma una foto del Spider actual y la guarda en el historial.
void guardarEstadoSpider(Spider& juego, Historial<EstadoSpider*>& historial) {
	EstadoSpider* estadoNuevo = new EstadoSpider(juego.getColumnas(), juego.getMazo(), juego.getEscalerasCompletadas(), juego.getVecesRepartido());
	historial.guardarEstado(estadoNuevo);
}

// Toma una foto del Golf actual y la guarda en el historial.
void guardarEstadoGolf(GolfGame& juego, Historial<EstadoGolf*>& historial) {
	EstadoGolf* estadoNuevo = new EstadoGolf(juego.getColumnas(), juego.getMazo(), juego.getPozo(), juego.getPuntuacionA());
	historial.guardarEstado(estadoNuevo);
}

// Pone el juego de Spider como estaba en la foto (el juego guarda su propia copia).
void cargarEstadoSpider(Spider& juego, EstadoSpider& estado) {
	juego.setColumnas(estado.columnasEstado);
	juego.setMazo(estado.mazoEstado);
	juego.setEscalerasCompletadas(estado.escalerasCompletadas);
	juego.setVecesRepartido(estado.vecesRepartido);
}

// Pone el juego de Golf como estaba en la foto (el juego guarda su propia copia).
void cargarEstadoGolf(GolfGame& juego, EstadoGolf& estado) {
	juego.setColumnas(estado.columnasEstado);
	juego.setMazo(estado.mazoEstado);
	juego.setPozo(estado.pozoEstado);
	juego.setPuntuacion(estado.puntuacion);
}

// Intenta volver un paso atrás en Golf. Devuelve false si no hay nada que deshacer.
bool deshacerGolf(GolfGame& juego, Historial<EstadoGolf*>& historial) {
	try {
		EstadoGolf* estado = historial.deshacer();
		cargarEstadoGolf(juego, *estado);
		return true;
	}
	catch (...) {
		return false;
	}
}

// Intenta avanzar un paso en Golf. Devuelve false si no hay nada que rehacer.
bool rehacerGolf(GolfGame& juego, Historial<EstadoGolf*>& historial) {
	try {
		EstadoGolf* estado = historial.rehacer();
		cargarEstadoGolf(juego, *estado);
		return true;
	}
	catch (...) {
		return false;
	}
}

// Muestra la pantalla de victoria de Spider y espera un ENTER.
void mostrarVictoriaSpider() {
	system("cls");
	cout << "\n\n";
	cout << "========================================================================\n";
	cout << "                               VICTORIA                                 \n";
	cout << "========================================================================\n";
	cout << "Presione ENTER para continuar...";
	string p; getline(cin, p);
}

// Si subió el número de escaleras, avisa al jugador que completó una.
void avisarEscalera(int antes, int despues) {
	if (despues > antes) {
		cout << "\n¡Escalera completa! Se retiró de la mesa. Presione ENTER...";
		string p; getline(cin, p);
	}
}

int main() {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	system("chcp 65001 > nul");
	srand(static_cast<unsigned int>(time(nullptr)));

	try {
		string opcion;
		do {
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|                BIENVENIDO AL SOLITARIO DE AXEL, MIKEL, ZACK y WIN ZHI                 |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|  ELIGA SU MODO DE JUEGO:                                                              |" << endl;
			cout << "|     1. SOLITARIO GOLF                                                                 |" << endl;
			cout << "|     2. SOLITARIO SPIDER                                                               |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;
			cout << "|  OPCIÓN:                                                                              |" << endl;
			cout << "|     3. Salir                                                                          |" << endl;
			cout << "+---------------------------------------------------------------------------------------+" << endl;

			// Pide la opción hasta que escriban 1, 2 o 3.
			do {
				cout << "OPCIÓN: ";
				getline(cin, opcion);
				if (opcion != "1" && opcion != "2" && opcion != "3") {
					cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, opción de menú principal no válida. Ingrese 1, 2 o 3.\n";
				}
			} while (opcion != "1" && opcion != "2" && opcion != "3");

			if (opcion == "1") {
				system("cls");
				GolfGame juegoGolf;
				bool juegoTerminado = false;

				// Se juegan hasta 9 rondas, cada una con baraja y historial nuevos.
				for (int ronda = 1; ronda <= 9 && !juegoTerminado; ronda++) {
					Historial<EstadoGolf*> historial = Historial<EstadoGolf*>();
					Card* barajaGolf[52];
					crearBarajaGolf(barajaGolf);
					mezclarBarajaGolf(barajaGolf);
					juegoGolf.reiniciarRonda();
					juegoGolf.iniciarJuego(barajaGolf);
					juegoGolf.setRondaActual(ronda);
					guardarEstadoGolf(juegoGolf, historial);

					string opcionGolf = "";
					bool rondaTerminada = false;
					bool rondaSumada = false; // Para no sumar los puntos de la ronda dos veces
					int ptsRondaFinal = 0;

					do {
						juegoGolf.mostrarTablero();

						// Sin jugadas y sin mazo: solo queda deshacer o abandonar la ronda.
						if (!juegoGolf.hayMovimientosDisponibles() && juegoGolf.mazoVacio()) {
							cout << "\n[!] ¡Atención! No hay movimientos disponibles y el mazo está vacío.\n";
							cout << "¿Desea [D]eshacer una jugada o [A]bandonar la ronda? (D/A): ";
							string decision;
							getline(cin, decision);
							if (decision == "A" || decision == "a") {
								if (!rondaSumada) {
									ptsRondaFinal = juegoGolf.getPuntuacion();
									juegoGolf.sumarPuntaje(ptsRondaFinal);
									rondaSumada = true;
								}
								rondaTerminada = true;
								break;
							}
							else if (decision == "D" || decision == "d") {
								if (!deshacerGolf(juegoGolf, historial)) {
									cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no hay más estados anteriores. Presione ENTER...";
									string p; getline(cin, p);
								}
							}
							continue;
						}

						// Si no hay jugadas pero aún hay mazo, se le avisa al jugador.
						if (!juegoGolf.hayMovimientosDisponibles()) {
							cout << "\n[!] No hay movimientos disponibles en las columnas. Saque una carta del mazo [2].\n";
						}

						cout << "\nSELECCIONE UNA OPCIÓN DEL JUEGO GOLF: ";
						getline(cin, opcionGolf);

						if (opcionGolf == "9") {
							juegoGolf.forzarVictoriaPrueba();
						}

						if (opcionGolf == "1" || opcionGolf == "9") {
							string colO = "1";
							if (opcionGolf != "9") {
								cout << "Columna de origen (1-7): ";
								getline(cin, colO);
							}

							try {
								if (opcionGolf != "9") {
									if (colO.empty()) throw runtime_error("La entrada no puede estar vacía.");
									for (char c : colO) {
										if (!isdigit(static_cast<unsigned char>(c))) throw runtime_error("Se ingresaron letras o caracteres no numéricos.");
									}
									int orig = stoi(colO) - 1;
									if (orig < 0 || orig >= 7) throw runtime_error("Número de columna fuera de rango (1-7).");
									
									if (juegoGolf.moverCarta(orig)) {
										guardarEstadoGolf(juegoGolf, historial);
									}
								}

								if ((juegoGolf.haGanado() || opcionGolf == "9") && !rondaSumada) {
									ptsRondaFinal = juegoGolf.getPuntuacion();
									juegoGolf.sumarPuntaje(ptsRondaFinal);
									rondaSumada = true;
									rondaTerminada = true;
									break;
								}
							}
							catch (const exception& ex) {
								cout << "\nERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, " << ex.what() << "\n";
								cout << "Presione ENTER para continuar...";
								string p; getline(cin, p);
							}
						}
						else if (opcionGolf == "2") {
							if (juegoGolf.sacarCartaMazo()) {
								guardarEstadoGolf(juegoGolf, historial);
							}
						}
						else if (opcionGolf == "3") {
							if (!deshacerGolf(juegoGolf, historial)) {
								cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no hay más estados anteriores. Presione ENTER...";
								string p; getline(cin, p);
							}
						}
						else if (opcionGolf == "4") {
							if (!rehacerGolf(juegoGolf, historial)) {
								cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no hay más estados siguientes. Presione ENTER...";
								string p; getline(cin, p);
							}
						}
						else if (opcionGolf == "5") {
							if (!rondaSumada) {
								int penalizacion = juegoGolf.getPuntuacion() + (9 - ronda) * 35;
								juegoGolf.sumarPuntaje(penalizacion);
								rondaSumada = true;
							}
							juegoTerminado = true;
							break;
						}
						else if (opcionGolf != "9") {
							cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, opción no válida. Presione ENTER...";
							string p; getline(cin, p);
						}
					} while (!rondaTerminada && opcionGolf != "5");

					if (juegoTerminado) break;

					// Fin de ronda: se muestra el puntaje y se avisa de la siguiente.
					cout << "\n========================================================================\n";
					cout << "  RONDA " << ronda << " TERMINADA. Puntaje de la ronda: " << ptsRondaFinal << "\n";
					cout << "  Puntaje acumulado: " << juegoGolf.getPuntajeAcumulado() << "\n";
					if (ronda < 9) cout << "  Se va a iniciar la ronda " << ronda + 1 << ".\n";
					cout << "========================================================================\n";
					cout << "Presione ENTER para continuar...";
					string pausa; getline(cin, pausa);
				}

				system("cls");
				int puntajeFinal = juegoGolf.getPuntajeAcumulado();
				cout << "\n\n";
				cout << "========================================================================\n";
				if (puntajeFinal <= 0) {
					cout << "                         VICTORIA PERFECTA                              \n";
				}
				else if (puntajeFinal <= 45) {
					cout << "                         VICTORIA REGULAR                               \n";
				}
				else {
					cout << "                              DERROTA                                   \n";
				}
				cout << "========================================================================\n";
				cout << "  Puntaje total obtenido en las 9 rondas: " << puntajeFinal << "\n";
				cout << "========================================================================\n";
				cout << "Presione ENTER para regresar al menú principal...";
				string p; getline(cin, p);
				system("cls");
			}
			else if (opcion == "2") {
				system("cls");
				Spider juegoSpider;
				Historial<EstadoSpider*> historialSpider = Historial<EstadoSpider*>();

				Card* barajaSpider[104];
				crearBarajaSpider(barajaSpider);
				mezclarBarajaSpider(barajaSpider);
				juegoSpider.iniciarJuego(barajaSpider);
				guardarEstadoSpider(juegoSpider, historialSpider);

				string opcionSpider = "";
				do {
					juegoSpider.mostrarTablero();
					cout << "\nSELECCIONE UNA OPCIÓN DEL JUEGO SPIDER: ";
					getline(cin, opcionSpider);

					if (opcionSpider == "1") {
						string colO, colD, cant;
						try {
							cout << "Columna origen (1-10): ";
							getline(cin, colO);
							if (colO.empty()) throw runtime_error("La columna de origen no puede estar vacía.");
							for (char c : colO) if (!isdigit(static_cast<unsigned char>(c))) throw runtime_error("Columna de origen inválida.");

							cout << "Columna destino (1-10): ";
							getline(cin, colD);
							if (colD.empty()) throw runtime_error("La columna de destino no puede estar vacía.");
							for (char c : colD) if (!isdigit(static_cast<unsigned char>(c))) throw runtime_error("Columna de destino inválida.");

							cout << "Cantidad de cartas: ";
							getline(cin, cant);
							if (cant.empty()) throw runtime_error("La cantidad de cartas no puede estar vacía.");
							for (char c : cant) if (!isdigit(static_cast<unsigned char>(c))) throw runtime_error("Cantidad de cartas inválida.");

							int orig = stoi(colO) - 1;
							int dest = stoi(colD) - 1;
							int cantidad = stoi(cant);

							if (orig < 0 || orig >= 10 || dest < 0 || dest >= 10)
								throw runtime_error("Las columnas deben estar entre 1 y 10.");

							int escalerasAntes = juegoSpider.getEscalerasCompletadas();
							// Solo se guarda en el historial si la jugada se hizo de verdad.
							if (juegoSpider.moverCartas(orig, dest, cantidad)) {
								guardarEstadoSpider(juegoSpider, historialSpider);
								avisarEscalera(escalerasAntes, juegoSpider.getEscalerasCompletadas());

								if (juegoSpider.haGanado()) {
									mostrarVictoriaSpider();
									break;
								}
							}
						}
						catch (const exception& ex) {
							cout << "\nERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, " << ex.what() << "\n";
							cout << "Presione ENTER para continuar...";
							string p; getline(cin, p);
						}
					}
					else if (opcionSpider == "2") {
						int escalerasAntes = juegoSpider.getEscalerasCompletadas();
						if (juegoSpider.repartirMazo()) {
							guardarEstadoSpider(juegoSpider, historialSpider);
							avisarEscalera(escalerasAntes, juegoSpider.getEscalerasCompletadas());

							if (juegoSpider.haGanado()) {
								mostrarVictoriaSpider();
								break;
							}
						}
						else {
							cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no se puede repartir: el mazo está vacío o hay una columna sin cartas. Presione ENTER...";
							string p; getline(cin, p);
						}
					}
					else if (opcionSpider == "3") {
						try {
							EstadoSpider* estado = historialSpider.deshacer();
							cargarEstadoSpider(juegoSpider, *estado);
						}
						catch (...) {
							cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no hay más estados. Presione ENTER...";
							string p; getline(cin, p);
						}
					}
					else if (opcionSpider == "4") {
						try {
							EstadoSpider* estado = historialSpider.rehacer();
							cargarEstadoSpider(juegoSpider, *estado);
						}
						catch (...) {
							cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, no hay más estados. Presione ENTER...";
							string p; getline(cin, p);
						}
					}
					else if (opcionSpider == "5") {
						system("cls");
						cout << "\n\n";
						cout << "========================================================================\n";
						cout << "                                DERROTA                                 \n";
						cout << "========================================================================\n";
						cout << "Presione ENTER para regresar al menú principal...";
						string p; getline(cin, p);
						break;
					}
					else if (opcionSpider == "9") {
						juegoSpider.forzarVictoriaPrueba();
						if (juegoSpider.haGanado()) {
							mostrarVictoriaSpider();
							break;
						}
					}
					else {
						cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, opción no válida. Presione ENTER...";
						string p; getline(cin, p);
					}

				} while (opcionSpider != "5");

				system("cls");
			}
			else if (opcion == "3") {
				system("cls");
				cout << "+------------------------------------------------------------------------------------------+" << endl;
				cout << "|                             GRACIAS POR JUGARSH, QUE DIOS LE BENDIGA                     |" << endl;
				cout << "+------------------------------------------------------------------------------------------+" << endl;
				return 0;
			}
		} while (opcion != "3");
	}
	catch (exception& e) {
		cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, error fatal en el sistema: " << e.what() << "\n";
	}
	return 0;
}