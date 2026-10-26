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

	// Crea una carta nueva, boca abajo y tapada, y arma su dibujo.
	// Palos: 0 corazones, 1 diamantes, 2 tréboles, 3 picas.
	Card(int numero, int palo) {
		this->numero = numero;
		this->palo = palo;
		up = false;
		cover = true;
		representacion = new LinkedList<string>();
		repre();
	}

	// Hace una copia exacta de otra carta, incluyendo su dibujo.
	// Se usa para sacar las "fotos" del historial.
	Card(const Card& carta) {
		numero = carta.numero;
		palo = carta.palo;
		up = carta.up;
		cover = carta.cover;

		representacion = new LinkedList<string>();
		carta.representacion->goToStart();

		while (!carta.representacion->atEnd()) {
			representacion->append(carta.representacion->getElement());
			carta.representacion->next();
		}
	}

	// Copia los datos de otra carta en esta, sin que compartan el dibujo.
	// Evita que se borre dos veces la misma lista.
	Card& operator=(const Card& otra) {
		if (this == &otra) return *this;
		numero = otra.numero;
		palo = otra.palo;
		up = otra.up;
		cover = otra.cover;

		representacion->clear();
		otra.representacion->goToStart();
		while (!otra.representacion->atEnd()) {
			representacion->append(otra.representacion->getElement());
			otra.representacion->next();
		}
		return *this;
	}

	// Libera la lista donde se guarda el dibujo de la carta.
	~Card() {
		delete representacion;
	}

	// Voltea la carta (boca arriba <-> boca abajo) y rehace su dibujo.
	void flip() {
		up = !up;
		repre();
	}

	// Cambia entre carta tapada (solo se ve la esquina) y destapada. Ojo: alterna, no siempre destapa.
	void unCover() {
		cover = !cover;
		repre();
	}

	// Imprime la carta en pantalla, línea por línea, usando el dibujo ya armado.
	void draw() {
		representacion->goToStart();
		while (!representacion->atEnd()) {
			cout << representacion->getElement() << "\n";
			representacion->next();
		}
	}

	// Arma el dibujo de la carta según esté boca arriba/abajo y tapada/destapada.
	// Tapada: 2 líneas. Destapada: 7 líneas.
	void repre() {
		representacion->clear();
		string p = simboloPalo();
		string n = simboloNumero();
		string dragon = "\xE9\xBE\x8D";

		// El 10 ocupa dos caracteres, así que lleva un espacio menos.
		string espArriba = (n.size() == 2) ? "   " : "    ";
		string espAbajo = (n.size() == 2) ? "     " : "      ";

		if (!cover) {
			if (up) {
				representacion->append("+-------+");
				representacion->append("|" + n + espArriba + p + " |");
				representacion->append("|       |");
				representacion->append("|   " + p + "   |");
				representacion->append("|       |");
				representacion->append("|" + espAbajo + n + "|");
				representacion->append("+-------+");
			}
			else {
				representacion->append("+-------+");
				representacion->append("|" + dragon + "     |");
				representacion->append("|       |");
				representacion->append("|       |");
				representacion->append("|       |");
				representacion->append("|     " + dragon + "|");
				representacion->append("+-------+");
			}
		}
		else {
			if (up) {
				representacion->append("+-------+");
				representacion->append("|" + n + espArriba + p + " |");
			}
			else {
				representacion->append("+-------+");
				representacion->append("|" + dragon + "     |");
			}
		}
	}

private:
	// Devuelve el dibujito del palo de esta carta.
	string simboloPalo() {
		if (palo == 0) return "\xE2\x99\xA5";
		if (palo == 1) return "\xE2\x99\xA6";
		if (palo == 2) return "\xE2\x99\xA3";
		if (palo == 3) return "\xE2\x99\xA0";
		return "";
	}

	// Devuelve el número como texto, cambiando 1, 11, 12 y 13 por A, J, Q y K.
	string simboloNumero() {
		if (numero == 1) return "A";
		if (numero == 11) return "J";
		if (numero == 12) return "Q";
		if (numero == 13) return "K";
		return to_string(numero);
	}
};