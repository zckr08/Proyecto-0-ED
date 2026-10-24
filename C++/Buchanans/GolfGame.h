#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Card.h"
#include "LinkedStack.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "Mazo.h"


class GolfGame {
private:
    ArrayList<LinkedList<Mazo*>*>* columnas;
    LinkedStack<Card*> mazo;
    LinkedStack<Card*> pozo;
    int puntuacion;

    void impresionBarajas(ArrayList<LinkedList<Mazo*>*>* mazos) {
        for (int i = 0; i < mazos->getSize(); i++) {
            mazos->goToPos(i);
            Mazo* columna = mazos->getElement()->getElement();
            columna->cartas->goToStart();
            columna->imprimiendo = false;
            columna->mazoImpreso = (columna->cartas->getSize() == 0);
        }
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
                if (parte == "") parte = "         ";
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
public:

    GolfGame() {
        columnas = new ArrayList<LinkedList<Mazo*>*>();
        for (int i = 0; i < 7; i++) {
            LinkedList<Mazo*>* listaMazo = new LinkedList<Mazo*>();
            LinkedList<Card*>* listaCartas = new LinkedList<Card*>();
            Mazo* columna = new Mazo(listaCartas);
            listaMazo->append(columna);
            columnas->append(listaMazo);
        }
        puntuacion = 0;
    }

    ~GolfGame() {
        for (int c = 0; c < columnas->getSize(); c++) {
            columnas->goToPos(c);
            LinkedList<Mazo*>* grupo = columnas->getElement();
            Mazo* columna = grupo->getElement();
            columna->cartas->goToStart();
            while (!columna->cartas->atEnd()) {
                delete columna->cartas->getElement();
                columna->cartas->next();
            }
            delete columna->cartas;
            delete columna;
            delete grupo;
        }
        delete columnas;
        while (!mazo.isEmpty()) {
            delete mazo.pop();
        }
        while (!pozo.isEmpty()) {
            delete pozo.pop();
        }
    }
    void iniciarJuego(Card* baraja[52]) {
        int i = 0; // index de las barajas

        // 1. Repartir 5 cartas a cada una de las 7 columnas 
        for (int col = 0; col < 7; col++) {
            columnas->goToPos(col);
            for (int fila = 0; fila < 5; fila++) {
                Card* cartaActual = baraja[i++];
                cartaActual->unCover();
                cartaActual->flip();
                if (fila == 4) cartaActual->unCover();
                columnas->getElement()->getElement()->cartas->append(cartaActual);
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
    bool esMovimientoValido(int entradaC) {
        if (entradaC < 0 || entradaC >= 7) return false;
        columnas->goToPos(entradaC);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        if (cartas->isEmpty() || pozo.isEmpty()) {
            return false;
        }
        cartas->goToPos(cartas->getSize() - 1);
        Card* cartaColumna = cartas->getElement();
        Card* cartaActual = pozo.topValue();

        int v1 = cartaColumna->numero;
        int v2 = cartaActual->numero;


        bool diferenciaNormal = (v1 == v2 + 1) || (v1 == v2 - 1);

        // A y K
        bool cicloAsRey = (v1 == 1 && v2 == 13) || (v1 == 13 && v2 == 1); // esto es la variacion de si sale A o K se puedan conectar entre ellos.

        return diferenciaNormal || cicloAsRey;
    }
    void moverCarta(int entradaJuego) {
        if (esMovimientoValido(entradaJuego)) {
            columnas->goToPos(entradaJuego);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            cartas->goToPos(cartas->getSize() - 1);
            Card* cartaMovida = cartas->remove();
            pozo.push(cartaMovida);
            if (!cartas->isEmpty()) {
                cartas->goToPos(cartas->getSize() - 1);
                cartas->getElement()->unCover();
            }
            std::cout << "¡Movimiento exit de exitante!\n";
        }
        else {
            std::cout << "Movimiento no válido. La carta debe ser una unidad mayor o menor que la del pozo monguis.\n";
        }
    }

    // funcion que manda al pozo 
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
    }


    bool haGanado() {     // pa saber si ganó.
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            if (!columnas->getElement()->getElement()->cartas->isEmpty()) {
                return false;
            }
        }
        return true;
    }

    // si no ganó (Todavia)
    bool hayMovimientosDisponibles() {
        if (pozo.isEmpty()) return false;

        int valorPozo = pozo.topValue()->numero;

        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            if (!cartas->isEmpty()) {
                cartas->goToPos(cartas->getSize() - 1);
                int valorColumna = cartas->getElement()->numero;
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
    int getPuntuacion() {
        int cartasPendientes = 0;
        columnas->goToStart();
        for (int i = 0; i < 7; i++) {
            puntuacion += columnas->getElement()->getElement()->cartas->getSize();
        }
        return puntuacion;
    }
    // para ver como va el jueguito
    void mostrarTablero() {
        cout << "\nPOZO \n";
        if (!pozo.isEmpty()) {
            cout << "Cima del Pozo ->\n";                  // MODIFICADO: sin "->" y con salto de línea
            if (!pozo.topValue()->up) pozo.topValue()->flip();
            pozo.topValue()->draw();
        }
        else {
            cout << "[Pozo Vacío]\n";
        }

        cout << "\nCOLUMNAS \n";
        for (int c = 0; c < columnas->getSize(); c++) cout << "    " << c + 1 << "              ";
        cout << endl;
        impresionBarajas(columnas);
        cout << "----------------------------------------\n";

        cout << "\nMAZO \n";
        cout << "Cartas restantes en mazo: " << mazo.getSize() << "\n";
        if (!mazo.isEmpty()) {
            cout << "Cima del Mazo ->\n";
            if (!mazo.topValue()->up) mazo.topValue()->flip();
            mazo.topValue()->draw();
        }
    }
};
