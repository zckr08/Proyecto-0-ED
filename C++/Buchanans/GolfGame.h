#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Card.h"
#include "LinkedStack.h"


// Funciones auxiliares de la baraja para el Golf (52 cartas)
void crearBarajaGolf(Card* baraja[52]) {   //Son como las del main, las pasé para acá para no llamarlas desde allá
    int b = 0;
    for (int t = 0; t < 4; t++) {
        for (int i = 1; i < 14; i++) {
            baraja[b] = new Card(i, t);
            b++;
        }
    }
}
void deleteBarajaGolf(Card* baraja[52]) {
    for (int i = 0; i < 52; i++) {
        delete baraja[i];
    }
}
void mezclarBarajaGolf(Card* baraja[52]) {  //
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 1000; i++) {
        int num1 = rand() % 52;
        int num2 = rand() % 52;
        if (num1 != num2) {
            Card* temp = baraja[num1];
            baraja[num1] = baraja[num2];
            baraja[num2] = temp;
        }
    }
}
struct MoveRecord {
    int tipoAccion;    // 1 = columna a pozo, 2 = mazo a pozo
    int columnaIndex;  // Índice de la columna (0 a 6)
    Card* carta;       // La carta que se movió
};

class GolfGame {
private:
    LinkedStack<Card*> columnas[7]; // Las 7 columnas del tablero (fácil de recorrer)
    LinkedStack<Card*> mazo;       // Esto pal mazo de reserva
    LinkedStack<Card*> pozo;       // Pila de descarte
    LinkedStack<MoveRecord*> historialMovimientos; // este guarda el historial
public:
    GolfGame() {}

    ~GolfGame() { //el destructosh
        // Limpiamos cada una de las 7 columnas
        for (int i = 0; i < 7; i++) {
            while (!columnas[i].isEmpty()) {
                delete columnas[i].pop();
            }
        }
        // Limpiamos el mazo por si quedaron cartas
        while (!mazo.isEmpty()) {
            delete mazo.pop();
        }
        // Limpiamos el pozo
        while (!pozo.isEmpty()) {
            delete pozo.pop();
        }
    }

    void iniciarJuego(Card* baraja[52]) {
        int i = 0; // index de las barajas

        // 1. Repartir 5 cartas a cada una de las 7 columnas 
        for (int col = 0; col < 7; col++) {
            for (int fila = 0; fila < 5; fila++) {
                Card* cartaActual = baraja[i++];
                cartaActual->unCover();
                cartaActual->flip();
                columnas[col].push(cartaActual);
            }
        }

        // 2. Pasar la primera carta restante al pozo
        Card* cartaInicial = baraja[i++];
        cartaInicial->unCover();
        cartaInicial->flip();
        pozo.push(cartaInicial);

        // 3. El resto de las cartas van al mazo de reserva
        while (i < 52) {
            Card* cartaMazo = baraja[i++];
            cartaMazo->unCover();
            mazo.push(cartaMazo);
        }
    }

    bool esMovimientoValido(int entradaJuego) {
        if (entradaJuego < 0 || entradaJuego >= 7) return false;
        if (columnas[entradaJuego].isEmpty() || pozo.isEmpty()) {
            return false;
        }

        Card* cartaColumna = columnas[entradaJuego].topValue();
        Card* cartaPozoActual = pozo.topValue();

        int v1 = cartaColumna->numero;
        int v2 = cartaPozoActual->numero;

        bool diferenciaNormal = (v1 == v2 + 1) || (v1 == v2 - 1);

        // A y K
        bool cicloAsRey = (v1 == 1 && v2 == 13) || (v1 == 13 && v2 == 1); // esto es la variacion de si sale A o K se puedan conectar entre ellos.

        return diferenciaNormal || cicloAsRey; 
    }

    void moverCarta(int entradaJuego) {
        if (esMovimientoValido(entradaJuego)) {
            Card* cartaMovida = columnas[entradaJuego].pop();
            pozo.push(cartaMovida);
            cout << "¡Movimiento exit de exitante!\n";

            // Crear el registro del movimiento
            MoveRecord* registro = new MoveRecord{ 1, entradaJuego, cartaMovida };
            historialMovimientos.push(registro);
        }
        else {
            cout << "Movimiento no válido. La carta debe ser una unidad mayor o menor que la del pozo monguis.\n";
        }
    }
    

    void sacarCartaMazo() {
        if (mazo.isEmpty()) {
            cout << "¡El mazo de reserva está vacío! No se pueden sacar más cartas.\n";
            return;
        }

        Card* cartaMazo = mazo.pop();
        cartaMazo->flip();
        cartaMazo->unCover();
        pozo.push(cartaMazo);
        cout << "Se ha sacado una nueva carta del mazo al pozo.\n";
        MoveRecord* registro = new MoveRecord{ 2, -1, cartaMazo };
        historialMovimientos.push(registro);
    }
    void deshacerMovimiento() {
        if (historialMovimientos.isEmpty()) {
            cout << "¡No hay movimientos para deshacer!\n";
            return;
        }

        // Sacamos el último movimiento realizado
        MoveRecord* ultimoMovimiento = historialMovimientos.pop();

        // Verificamos de dónde vino la carta para revertirla
        if (ultimoMovimiento->tipoAccion == 1) {
            // la sacamos del pozo y la regresamos a su columna
            Card* cartaRegresada = pozo.pop();
            columnas[ultimoMovimiento->columnaIndex].push(cartaRegresada);
            cout << "Se ha deshecho el movimiento. \n";
        }
        else if (ultimoMovimiento->tipoAccion == 2) {
            //  la sacamos del pozo y la regresamos al mazo
            Card* cartaRegresada = pozo.pop();
            
            cartaRegresada->flip(); // le damos vuelta para que no se vea en el mazo
            mazo.push(cartaRegresada);
            cout << "Se ha devuelto la carta al mazo.\n";
        }

        //Liberar la memoria del registro del movimiento
        delete ultimoMovimiento;
    }

    bool haGanado() {            // pa saber si ganó 
        for (int i = 0; i < 7; i++) {
            if (!columnas[i].isEmpty()) {
                return false;
            }
        }
        return true;
    }

    bool hayMovimientosDisponibles() { //Esta va a comparar el valor de la carta que hay en el pozo
        if (pozo.isEmpty()) return false;

        int valorPozo = pozo.topValue()->numero;

        for (int i = 0; i < 7; i++) {
            if (!columnas[i].isEmpty()) {
                int valorColumna = columnas[i].topValue()->numero;  // va columna por columna comparando si hay más movimientos validos
                if (valorColumna == valorPozo + 1 || valorColumna == valorPozo - 1) {
                    return true;
                }
            }
        }
        return false;
    }

    bool mazoVacio() {
        return mazo.isEmpty();
    }

    void mostrarTablero() {
        cout << "\nPOZO \n";
        if (!pozo.isEmpty()) {
            cout << "Cima del Pozo -> ";
            pozo.topValue()->draw();
        }
        else {
            cout << "[Pozo Vacío]\n";
        }

        cout << "\nCOLUMNAS \n";
        for (int i = 0; i < 7; i++) {
            cout << "Columna " << i + 1 << " (Cartas restantes: " << columnas[i].getSize() << "):\n";
            if (!columnas[i].isEmpty()) {
                cout << "  Cima -> ";
                columnas[i].topValue()->draw();
            }
            else {
                cout << "  [Columna Vacía]\n";
            }
        }
        cout << "----------------------------------------\n";
        cout << "Cartas restantes en mazo: " << mazo.getSize() << "\n";
    }
};