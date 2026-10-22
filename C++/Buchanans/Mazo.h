#pragma once
#include "LinkedList.h"
#include "Card.h"

class Mazo {
public:
	LinkedList<Card*>* cartas;
	bool mazoImpreso;
	bool imprimiendo;
	LinkedList<string>* representacion;
	Mazo(LinkedList<Card*>* cartas) {
		this->cartas = cartas;
		mazoImpreso = false;
		imprimiendo = false;
		representacion = nullptr;
	}
	void estadoImpreso() {
		mazoImpreso = !mazoImpreso;
	}
	void estadoImprimiendo() {
		imprimiendo = !imprimiendo;
	}
};

