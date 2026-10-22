#pragma once
#include <string>
#include <iostream>
#include <windows.h>
#include "LinkedList.h"

using std::string;
using std::to_string;
using std::cout;
class Card {
public:
	int numero;
	bool up;
	bool cover;
	int palo;
	LinkedList<string>* representacion;

	Card(int numero, int palo) {
		SetConsoleOutputCP(CP_UTF8);
		this->numero = numero;
		this->palo = palo;
		up = false;
		cover = true;
		representacion = new LinkedList<string>();
		repre();
	}

	~Card() {
		delete representacion;
	}

	void flip() { //ahora flip puede volver a que este cubierto
		up = !up;
		repre();
	}

	void unCover() {
		cover = false;
		repre();
	}

	void draw() {
		string tipoPalo;
		string simbolo;
		if (!cover) {
			if (palo == 0)
				tipoPalo = "\xE2\x99\xA5";
			else if (palo == 1)
				tipoPalo = "\xE2\x99\xA6";
			else if (palo == 2)
				tipoPalo = "\xE2\x99\xA3";
			else if (palo == 3)
				tipoPalo = "\xE2\x99\xA0";
			if (up) {
				if (numero >= 2 && numero < 10) {
					cout << "+-------+" << "\n";
					cout << "|" << numero << "    " << tipoPalo << " | " << "\n";
					cout << "|       |" << "\n";
					cout << "|   " << tipoPalo << "   |" << "\n";
					cout << "|       |" << "\n";
					cout << "|      " << numero << "|" << "\n";
					cout << "+-------+" << "\n";
				}

				else if (numero == 10) {
					cout << "+-------+" << "\n";
					cout << "|" << numero << "   " << tipoPalo << " | " << "\n";
					cout << "|       |" << "\n";
					cout << "|   " << tipoPalo << "   |" << "\n";
					cout << "|       |" << "\n";
					cout << "|     " << numero << "|" << "\n";
					cout << "+-------+" << "\n";
				}

				else {
					if (numero == 1)
						simbolo = "A";
					else if (numero == 11)
						simbolo = "J";
					else if (numero == 12)
						simbolo = "Q";
					else if (numero == 13)
						simbolo = "K";

					cout << "+-------+" << "\n";
					cout << "|" << simbolo << "    " << tipoPalo << " | " << "\n";
					cout << "|       |" << "\n";
					cout << "|   " << tipoPalo << "   |" << "\n";
					cout << "|       |" << "\n";
					cout << "|      " << simbolo << "|" << "\n";
					cout << "+-------+" << "\n";
				}
			}
			else {
				cout << "+-------+" << "\n";
				cout << "|" << "\xE9\xBE\x8D" << "     | " << "\n";
				cout << "|       |" << "\n";
				cout << "|       |" << "\n";
				cout << "|       |" << "\n";
				cout << "|     " << "\xE9\xBE\x8D" << "|" << "\n";
				cout << "+-------+" << "\n";
			}
		}
		else {
			if (palo == 0)
				tipoPalo = "\xE2\x99\xA5";

			else if (palo == 1)
				tipoPalo = "\xE2\x99\xA6";
			else if (palo == 2)
				tipoPalo = "\xE2\x99\xA3";
			else if (palo == 3)
				tipoPalo = "\xE2\x99\xA0";
			if (up) {
				if (numero >= 2 && numero < 10) {
					cout << "+-------+" << "\n";
					cout << "|" << numero << "    " << tipoPalo << " | " << "\n";
				}

				else if (numero == 10) {
					cout << "+-------+" << "\n";
					cout << "|" << numero << "   " << tipoPalo << " | " << "\n";
				}
				else {
					if (numero == 1)
						simbolo = "A";
					else if (numero == 11)
						simbolo = "J";
					else if (numero == 12)
						simbolo = "Q";
					else if (numero == 13)
						simbolo = "K";

					cout << "+-------+" << "\n";
					cout << "|" << simbolo << "    " << tipoPalo << " | " << "\n";
				}
			}
			else {
				cout << "+-------+" << "\n";
				cout << "|" << "\xE9\xBE\x8D" << "     | " << "\n";
			}
		}
	}
	void repre() {
		string tipoPalo;
		string simbolo;
		representacion->clear();

		if (!cover) {
			if (palo == 0)
				tipoPalo = "\xE2\x99\xA5";
			else if (palo == 1)
				tipoPalo = "\xE2\x99\xA6";
			else if (palo == 2)
				tipoPalo = "\xE2\x99\xA3";
			else if (palo == 3)
				tipoPalo = "\xE2\x99\xA0";

			if (up) {
				if (numero >= 2 && numero < 10) {
					representacion->append("+-------+");
					representacion->append("|" + to_string(numero) + "    " + tipoPalo + " |");
					representacion->append("|       |");
					representacion->append("|   " + tipoPalo + "   |");
					representacion->append("|       |");
					representacion->append("|      " + to_string(numero) + "|");
					representacion->append("+-------+");
				}
				else if (numero == 10) {
					representacion->append("+-------+");
					representacion->append("|" + to_string(numero) + "   " + tipoPalo + " |");
					representacion->append("|       |");
					representacion->append("|   " + tipoPalo + "   |");
					representacion->append("|       |");
					representacion->append("|     " + to_string(numero) + "|");
					representacion->append("+-------+");
				}
				else {
					if (numero == 1)
						simbolo = "A";
					else if (numero == 11)
						simbolo = "J";
					else if (numero == 12)
						simbolo = "Q";
					else if (numero == 13)
						simbolo = "K";

					representacion->append("+-------+");
					representacion->append("|" + simbolo + "    " + tipoPalo + " |");
					representacion->append("|       |");
					representacion->append("|   " + tipoPalo + "   |");
					representacion->append("|       |");
					representacion->append("|      " + simbolo + "|");
					representacion->append("+-------+");
				}
			}
			else {
				representacion->append("+-------+");
				representacion->append("|" + string("\xE9\xBE\x8D") + "     |");
				representacion->append("|       |");
				representacion->append("|       |");
				representacion->append("|       |");
				representacion->append("|     " + string("\xE9\xBE\x8D") + "|");
				representacion->append("+-------+");
			}
		}
		else {
			if (palo == 0)
				tipoPalo = "\xE2\x99\xA5";
			else if (palo == 1)
				tipoPalo = "\xE2\x99\xA6";
			else if (palo == 2)
				tipoPalo = "\xE2\x99\xA3";
			else if (palo == 3)
				tipoPalo = "\xE2\x99\xA0";

			if (up) {
				if (numero >= 2 && numero < 10) {
					representacion->append("+-------+");
					representacion->append("|" + to_string(numero) + "    " + tipoPalo + " |");
				}
				else if (numero == 10) {
					representacion->append("+-------+");
					representacion->append("|" + to_string(numero) + "   " + tipoPalo + " |");
				}
				else {
					if (numero == 1)
						simbolo = "A";
					else if (numero == 11)
						simbolo = "J";
					else if (numero == 12)
						simbolo = "Q";
					else if (numero == 13)
						simbolo = "K";

					representacion->append("+-------+");
					representacion->append("|" + simbolo + "    " + tipoPalo + " |");
				}
			}
			else {
				representacion->append("+-------+");
				representacion->append("|" + string("\xE9\xBE\x8D") + "     |");
			}
		}
	}
};

