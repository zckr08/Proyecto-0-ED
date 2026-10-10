
#include <iostream>
#include "Card.h"

int main() {
	Card test = Card(2, 0);
	test.draw();

	for (int t = 0; t < 4; t++) {
		for (int i = 1; i < 14; i++) {
			Card ejemplo = Card(i, t);
			ejemplo.flip();
			if (i == 13) ejemplo.unCover();
			ejemplo.draw();
		}
	}
}