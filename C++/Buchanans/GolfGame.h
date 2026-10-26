#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Card.h"
#include "LinkedStack.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "Mazo.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

class GolfGame {
private:
    ArrayList<LinkedList<Mazo*>*>* columnas;
    LinkedStack<Card*> mazo;
    LinkedStack<Card*> pozo;
    int puntuacion;
    int rondaActual;
    int puntajeAcumulado;

    // Borra las cartas y todo lo que forma las columnas que le pasen.
    // Suelta el puntero de dibujo del Mazo antes, porque pertenece a la carta.
    void liberarColumnas(ArrayList<LinkedList<Mazo*>*>* cols) {
        for (int c = 0; c < cols->getSize(); c++) {
            cols->goToPos(c);
            LinkedList<Mazo*>* grupo = cols->getElement();
            Mazo* columna = grupo->getElement();
            columna->cartas->goToStart();
            while (!columna->cartas->atEnd()) {
                delete columna->cartas->getElement();
                columna->cartas->next();
            }
            delete columna->cartas;
            columna->representacion = nullptr;
            delete columna;
            delete grupo;
        }
        delete cols;
    }

    // Hace una copia nueva de las columnas, con cartas nuevas e iguales.
    // Así la copia y el original nunca comparten nada.
    ArrayList<LinkedList<Mazo*>*>* copiarColumnas(ArrayList<LinkedList<Mazo*>*>* origen) {
        ArrayList<LinkedList<Mazo*>*>* copia = new ArrayList<LinkedList<Mazo*>*>();
        for (int i = 0; i < origen->getSize(); i++) {
            origen->goToPos(i);
            LinkedList<Card*>* listaOriginal = origen->getElement()->getElement()->cartas;
            LinkedList<Card*>* listaNueva = new LinkedList<Card*>();
            listaOriginal->goToStart();
            while (!listaOriginal->atEnd()) {
                listaNueva->append(new Card(*listaOriginal->getElement()));
                listaOriginal->next();
            }
            LinkedList<Mazo*>* grupo = new LinkedList<Mazo*>();
            grupo->append(new Mazo(listaNueva));
            copia->append(grupo);
        }
        return copia;
    }

    // Saca y borra todas las cartas de una pila hasta dejarla vacía.
    void vaciarPila(LinkedStack<Card*>& pila) {
        while (!pila.isEmpty()) delete pila.pop();
    }

    // Copia una pila carta por carta en otra, respetando el orden.
    // Usa una pila temporal y deja la de origen como estaba.
    void copiarPila(LinkedStack<Card*>* origen, LinkedStack<Card*>& destino) {
        LinkedStack<Card*> temp;
        while (!origen->isEmpty()) temp.push(origen->pop());
        while (!temp.isEmpty()) {
            Card* c = temp.pop();
            origen->push(c);
            destino.push(new Card(*c));
        }
    }

    // Dibuja las 7 columnas lado a lado, línea por línea de cada carta.
    // Va recorriendo todas las columnas a la vez hasta que todas terminan.
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
                Mazo* mazoCol = mazos->getElement()->getElement();
                string parte = "";
                if (!mazoCol->mazoImpreso) {
                    if (!mazoCol->imprimiendo) {
                        Card* carta = mazoCol->cartas->getElement();
                        mazoCol->cartas->next();
                        mazoCol->representacion = carta->representacion;
                        mazoCol->representacion->goToStart();
                        mazoCol->imprimiendo = true;
                    }
                    parte = mazoCol->representacion->getElement();
                    mazoCol->representacion->next();
                    if (mazoCol->representacion->atEnd()) {
                        mazoCol->imprimiendo = false;
                        if (mazoCol->cartas->atEnd()) mazoCol->mazoImpreso = true;
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
    // Crea el juego con 7 columnas vacías y los puntajes en cero.
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
        rondaActual = 1;
        puntajeAcumulado = 0;
    }

    // Borra todo lo que el juego tenga: columnas, mazo y pozo.
    ~GolfGame() {
        liberarColumnas(columnas);
        vaciarPila(mazo);
        vaciarPila(pozo);
    }

    // Reparte las 52 cartas: 5 por columna, 1 al pozo y el resto al mazo.
    // Solo la última de cada columna queda destapada.
    void iniciarJuego(Card* baraja[52]) {
        int i = 0;
        for (int col = 0; col < 7; col++) {
            columnas->goToPos(col);
            for (int fila = 0; fila < 5; fila++) {
                Card* cartaActual = baraja[i++];
                cartaActual->flip();
                if (fila == 4) cartaActual->unCover();
                columnas->getElement()->getElement()->cartas->append(cartaActual);
            }
        }

        Card* cartaInicial = baraja[i++];
        cartaInicial->unCover();
        cartaInicial->flip();
        pozo.push(cartaInicial);

        while (i < 52) {
            Card* cartaMazo = baraja[i++];
            cartaMazo->unCover();
            mazo.push(cartaMazo);
        }

        // La carta de arriba del mazo queda boca arriba, para que se vea.
        if (!mazo.isEmpty()) mazo.topValue()->flip();
    }

    // Truco de pruebas: borra todas las cartas de las columnas para ganar al instante.
    // Es la opción 9 del menú.
    void forzarVictoriaPrueba() {
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            while (!cartas->isEmpty()) {
                cartas->goToStart();
                delete cartas->remove();
            }
        }
    }

    // Dice si la carta de arriba de esa columna es un número más o menos que la del pozo.
    // El As y el Rey cuentan como vecinos.
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
        bool cicloAsRey = (v1 == 1 && v2 == 13) || (v1 == 13 && v2 == 1);

        return diferenciaNormal || cicloAsRey;
    }

    // Pasa la carta de arriba de la columna al pozo si se puede, y destapa la siguiente.
    // Devuelve true si la jugada se hizo y false si no era válida.
    bool moverCarta(int entradaJuego) {
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
            cout << "¡Movimiento exitoso!\n";
            return true;
        }
        cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, movimiento no válido. La carta superior de la columna debe ser una unidad mayor o menor que la del pozo.\n";
        cout << "Presione ENTER para continuar...";
        string p; getline(cin, p);
        return false;
    }

    // Saca la carta de arriba del mazo y la pone boca arriba en el pozo.
    // Devuelve false si el mazo ya no tenía cartas.
    bool sacarCartaMazo() {
        if (mazo.isEmpty()) {
            cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, el mazo de reserva está vacío y no se pueden sacar más cartas.\n";
            cout << "Presione ENTER para continuar...";
            string p; getline(cin, p);
            return false;
        }
        // La carta de arriba del mazo ya está boca arriba, así que pasa al pozo tal cual.
        Card* cartaMazo = mazo.pop();
        pozo.push(cartaMazo);
        // La nueva carta de arriba del mazo se voltea para que se vea.
        if (!mazo.isEmpty() && !mazo.topValue()->up) mazo.topValue()->flip();
        cout << "Se ha sacado una nueva carta del mazo al pozo.\n";
        return true;
    }

    // Dice si ya no queda ninguna carta en las columnas (ronda ganada).
    bool haGanado() {
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            if (!columnas->getElement()->getElement()->cartas->isEmpty()) {
                return false;
            }
        }
        return true;
    }

    // Revisa si alguna columna tiene una carta jugable contra el pozo.
    // Devuelve true apenas encuentra una.
    bool hayMovimientosDisponibles() {
        if (pozo.isEmpty()) return false;
        int valorPozo = pozo.topValue()->numero;
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            if (!cartas->isEmpty()) {
                cartas->goToPos(cartas->getSize() - 1);
                int valorColumna = cartas->getElement()->numero;
                if (valorColumna == valorPozo + 1 || valorColumna == valorPozo - 1 ||
                    (valorColumna == 1 && valorPozo == 13) || (valorColumna == 13 && valorPozo == 1)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Dice si ya no quedan cartas en el mazo.
    bool mazoVacio() {
        return mazo.isEmpty();
    }

    // Un día me les voy a morir por estas cosas
    // Borra todo lo de la ronda anterior para poder repartir una nueva.
    // Hay que llamarlo antes de iniciarJuego.
    void reiniciarRonda() {
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            while (!cartas->isEmpty()) {
                cartas->goToStart();
                delete cartas->remove();
            }
        }
        vaciarPila(mazo);
        vaciarPila(pozo);
    }

    // Calcula los puntos de la ronda: cartas que quedan en columnas.
    // Si no queda ninguna, es negativo por cada carta del mazo.
    int getPuntuacion() {
        int cartasPendientes = 0;
        for (int i = 0; i < 7; i++) {
            columnas->goToPos(i);
            cartasPendientes += columnas->getElement()->getElement()->cartas->getSize();
        }
        if (cartasPendientes == 0) {
            return -mazo.getSize();
        }
        return cartasPendientes;
    }

    // Limpia la pantalla y dibuja todo: ronda, puntaje, pozo, columnas, mazo y menú.
    // Del mazo se dibuja la carta de arriba (boca arriba) y cuántas cartas quedan.
    void mostrarTablero() {
        system("cls");
        cout << "========================================================================\n";
        cout << "  RONDA ACTUAL: " << rondaActual << " / 9   |   PUNTAJE ACUMULADO: " << puntajeAcumulado << "\n";
        cout << "========================================================================\n";

        cout << "\nPOZO \n";
        if (!pozo.isEmpty()) {
            cout << "Cima del Pozo ->\n";
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
        cout << "---------------------------------------------------------------------------------------------------------------------------\n";

        cout << "\nMAZO\n";
        if (!mazo.isEmpty()) {
            mazo.topValue()->draw();
        }
        else {
            cout << "[Mazo Vacío]\n";
        }
        cout << "Cartas restantes en mazo: " << mazo.getSize() << "\n";
        cout << "\n --------------------------------------------------------------------------------------\n";
        cout << "  [1] Mover al Pozo   [2] Sacar Del Mazo   [3] Deshacer   [4] Rehacer   [5] Abandonar\n";
        cout << " --------------------------------------------------------------------------------------\n";
    }

    // Guarda el número de ronda que se está jugando.
    void setRondaActual(int r) { rondaActual = r; }
    // Devuelve el número de ronda actual.
    int getRondaActual() { return rondaActual; }
    // Suma puntos al total de todas las rondas.
    void sumarPuntaje(int p) { puntajeAcumulado += p; }
    // Devuelve el total de puntos acumulado.
    int getPuntajeAcumulado() { return puntajeAcumulado; }

    // Devuelve las columnas reales del juego (el historial las copia).
    ArrayList<LinkedList<Mazo*>*>* getColumnas() { return columnas; }
    // Devuelve el mazo real; el historial lo vacía y lo vuelve a llenar igual.
    LinkedStack<Card*>* getMazo() { return &mazo; }
    // Devuelve el pozo real; el historial lo trata igual que al mazo.
    LinkedStack<Card*>* getPozo() { return &pozo; }
    // Igual que getPuntuacion, se usa al guardar el estado.
    int getPuntuacionA() { return getPuntuacion(); }

    // Cambia las columnas por una copia de las que le den y borra las viejas.
    // Así el juego nunca comparte cartas con el historial.
    void setColumnas(ArrayList<LinkedList<Mazo*>*>* cols) {
        ArrayList<LinkedList<Mazo*>*>* nuevas = copiarColumnas(cols);
        liberarColumnas(columnas);
        columnas = nuevas;
    }
    // Reemplaza el mazo por una copia del que le den.
    void setMazo(LinkedStack<Card*>* mazoC) {
        if (mazoC == &mazo) return;
        vaciarPila(mazo);
        copiarPila(mazoC, mazo);
    }
    // Reemplaza el pozo por una copia del que le den.
    void setPozo(LinkedStack<Card*>* pozoC) {
        if (pozoC == &pozo) return;
        vaciarPila(pozo);
        copiarPila(pozoC, pozo);
    }
    // Guarda el valor de puntuación que traía un estado.
    void setPuntuacion(int p) { puntuacion = p; }
};