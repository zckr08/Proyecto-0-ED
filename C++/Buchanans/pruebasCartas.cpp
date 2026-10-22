#include <cstdlib>
#include <iostream>
#include "Card.h"
#include "LinkedList.h"

using std::cout;
using std::endl;

void crearBaraja1(Card* baraja[52]) { //funcion para crear una baraja estandard de 52 cardas, mantener para la galeria del menu principal pls
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

int main() {
	Card* baraja[52];
	crearBaraja1(baraja);
	LinkedList<Card*>* lista1 = new LinkedList<Card*>();
	LinkedList<Card*>* lista2 = new LinkedList<Card*>();
	LinkedList<Card*>* lista3 = new LinkedList<Card*>();
	for (int i = 0; i < 5; i++) {
		baraja[i]->flip();
		lista1->append(baraja[i]);
	}
	for (int i = 0; i < 4; i++) {
		baraja[i + 10]->flip();
		lista2->append(baraja[i + 10]);
	}
	for (int i = 0; i < 3; i++) {
		baraja[i + 20]->flip();
		lista3->append(baraja[i + 20]);
	}
	lista1->goToStart();
	lista2->goToStart();
	lista3->goToStart();
	bool todasImpresas = false;
	bool mazo1 = false;
	bool imprimiendo1 = false;
	LinkedList<string>* representacion1 = nullptr;
	bool mazo2 = false;
	bool imprimiendo2 = false;
	LinkedList<string>* representacion2 = nullptr;
	bool mazo3 = false;
	bool imprimiendo3 = false;
	LinkedList<string>* representacion3 = nullptr;
	while (!todasImpresas) {
		string imprimir = "";
		if (!mazo1) {
			if (!imprimiendo1) {
				Card* carta = lista1->getElement();
				lista1->next();
				if (lista1->atEnd()) carta->unCover();
				representacion1 = carta->representacion;
				representacion1->goToStart();
				imprimiendo1 = true;
			}
			imprimir += representacion1->getElement();
			imprimir += " ";
			representacion1->next();
			if (representacion1->atEnd()) {
				imprimiendo1 = false;
				if (lista1->atEnd()) mazo1 = true;
			}
		}
		if (mazo1) imprimir += "          ";
		if (!mazo2) {
			if (!imprimiendo2) {
				Card* carta = lista2->getElement();
				lista2->next();
				if (lista2->atEnd()) carta->unCover();
				representacion2 = carta->representacion;
				representacion2->goToStart();
				imprimiendo2 = true;
			}
			imprimir += representacion2->getElement();
			imprimir += " ";
			representacion2->next();
			if (representacion2->atEnd()) {
				imprimiendo2 = false;
				if (lista2->atEnd()) mazo2 = true;
			}
		}
		if (mazo2) imprimir += "          ";
		if (!mazo3) {
			if (!imprimiendo3) {
				Card* carta = lista3->getElement();
				lista3->next();
				if (lista3->atEnd()) carta->unCover();
				representacion3 = carta->representacion;
				representacion3->goToStart();
				imprimiendo3 = true;
			}
			imprimir += representacion3->getElement();
			imprimir += " ";
			representacion3->next();
			if (representacion3->atEnd()) {
				imprimiendo3 = false;
				if (lista3->atEnd()) mazo3 = true;
			}
		}
		if (mazo3) imprimir += "          ";
		if (mazo1 && mazo2 && mazo3) todasImpresas = true;
		cout << imprimir << endl;
	}
	delete lista1;
	delete lista2;
	delete lista3;
	for (int i = 0; i < 52; i++) {
		delete baraja[i];
	}
}