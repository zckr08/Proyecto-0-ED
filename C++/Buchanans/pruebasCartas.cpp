#include <cstdlib>
#include <iostream>
#include "Card.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "Mazo.h"

using std::cout;
using std::endl;
using std::string;

void crearBaraja1(Card* baraja[52]) { //funcion para crear una baraja estandard de 52 cardas, mantener para la galeria del menu principal pls
	int b = 0;
	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			baraja[b] = new Card(i, t);
			b++;
		}
	}
}

void impresionBarajas(ArrayList<LinkedList<Mazo*>*>* mazos) {
	bool todasImpresas = false;
	int cantidadMazos = mazos->getSize();
	int listos;
	while (!todasImpresas) {
		string imprimir = "";
		for (int i = 0; i < mazos->getSize();i++) {
			mazos->goToPos(i);
			Mazo* mazo = mazos->getElement()->getElement();
			string parte = "";
			if (!mazo->mazoImpreso) {
				if (!mazo->imprimiendo) {
					Card* carta = mazo->cartas->getElement();
					mazo->cartas->next();
					mazo->representacion = carta->representacion;
					mazo->representacion->goToStart();
					mazo->imprimiendo = true;
				}
				parte = mazo->representacion->getElement();
				mazo->representacion->next();
				if (mazo->representacion->atEnd()) {
					mazo->imprimiendo = false;
					if (mazo->cartas->atEnd()) mazo->mazoImpreso = true;
				}
			}
			imprimir += parte + "          ";
		}
		listos = 0;
		for (int i = 0; i < mazos->getSize();i++) {
			mazos->goToPos(i);
			if (mazos->getElement()->getElement()->mazoImpreso) listos += 1;
		}
		if (listos == cantidadMazos) todasImpresas = true;
		cout << imprimir << endl;
	}
}

int main() {
	Card* baraja[52];
	crearBaraja1(baraja);
	int tamanos[3] = { 5, 4, 3 };
	int inicio[3] = { 0, 10, 20 };
	ArrayList<LinkedList<Mazo*>*>* mazos = new ArrayList<LinkedList<Mazo*>*>();
	for (int m = 0; m < 3; m++) {
		LinkedList<Card*>* lista = new LinkedList<Card*>();
		for (int i = 0; i < tamanos[m]; i++) {
			baraja[inicio[m] + i]->flip();
			lista->append(baraja[inicio[m] + i]);
		}
		baraja[inicio[m] + tamanos[m] - 1]->unCover();   // NUEVO: destapar la última carta antes de imprimir
		lista->goToStart();
		LinkedList<Mazo*>* grupo = new LinkedList<Mazo*>();
		grupo->append(new Mazo(lista));
		grupo->goToStart();
		mazos->append(grupo);
	}

	impresionBarajas(mazos);

	for (int m = 0; m < mazos->getSize(); m++) {
		mazos->goToPos(m);
		LinkedList<Mazo*>* grupo = mazos->getElement();
		Mazo* mazo = grupo->getElement();
		delete mazo->cartas;
		delete mazo;
		delete grupo;
	}
	delete mazos;
	for (int i = 0; i < 52; i++) {
		delete baraja[i];
	}
}