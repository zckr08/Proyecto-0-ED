#pragma once
#include <iostream>
#include <string>
#include "Card.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "Mazo.h"
#include "LinkedStack.h"

using namespace std;

// Todos queremos a MIKEL... Atte: Equipo-Mikel
// comentarios desde aquí son de Mikel, para documentar como nunca...

class Spider {
private:
    ArrayList<LinkedList<Mazo*>*>* columnas; // 10 columnas
    LinkedStack<Card*> mazo;        // Mazo en pila
    int escalerasCompletadas;       // Escaleras completas
    int vecesRepartido;             // Veces repartido

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
                Mazo* columna = mazos->getElement()->getElement();
                string parte = "";
                if (!columna->mazoImpreso) {
                    if (!columna->imprimiendo) {
                        Card* carta = columna->cartas->getElement();
                        columna->cartas->next();
                        columna->representacion = carta->representacion;
                        columna->representacion->goToStart();
                        columna->imprimiendo = true;
                    }
                    parte = columna->representacion->getElement();
                    columna->representacion->next();
                    if (columna->representacion->atEnd()) {
                        columna->imprimiendo = false;
                        if (columna->cartas->atEnd()) columna->mazoImpreso = true;
                    }
                }
                if (parte == "") parte = "         ";
                imprimir += parte + "  ";
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
    Spider() {
        columnas = new ArrayList<LinkedList<Mazo*>*>();
        for (int i = 0; i < 10; i++) {
            LinkedList<Mazo*>* listaMazo = new LinkedList<Mazo*>();
            LinkedList<Card*>* listaCartas = new LinkedList<Card*>();
            Mazo* columna = new Mazo(listaCartas);
            listaMazo->append(columna);
            columnas->append(listaMazo);
        }
        escalerasCompletadas = 0;
        vecesRepartido = 0;
    }

    ~Spider() {
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
    }

    void iniciarJuego(Card* baraja[104]) {
        int index = 0;
        for (int col = 0; col < 10; col++) {
            int cantidadCartas = (col < 4) ? 6 : 5;

            for (int f = 0; f < cantidadCartas; f++) {
                Card* cartaActual = baraja[index++];
                if (f == cantidadCartas - 1) {
                    cartaActual->unCover();
                    cartaActual->flip();
                }
                columnas->goToPos(col);
                LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
                cartas->append(cartaActual);
            }
        }

        while (index < 104) {
            Card* cartaMazo = baraja[index++];
            mazo.push(cartaMazo);
        }
    }

    bool esMovimientoValido(int colOrigen, int colDestino, int cantidadCartas) {
        if (colOrigen < 0 || colOrigen >= 10 || colDestino < 0 || colDestino >= 10 || colOrigen == colDestino) {   // origen != destino
            return false;
        }
        columnas->goToPos(colOrigen);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        if (cartas->isEmpty() || cantidadCartas <= 0) {
            return false;
        }
        if (cantidadCartas > cartas->getSize()) {
            return false;
        }

        int tamOrigen = cartas->getSize();
        int posInicio = tamOrigen - cantidadCartas;

        cartas->goToPos(posInicio);
        Card* primeraCartaMovida = cartas->getElement();
        if (!primeraCartaMovida->up) return false;               // debe estar boca arriba

        for (int i = 0; i < cantidadCartas - 1; i++) {
            cartas->goToPos(posInicio + i);
            Card* cartaActual = cartas->getElement();
            cartas->goToPos(posInicio + i + 1);
            Card* cartaSiguiente = cartas->getElement();

            if (cartaActual->numero != cartaSiguiente->numero + 1) {
                return false;
            }
        }
        columnas->goToPos(colDestino);
        LinkedList<Card*>* cartasDestino = columnas->getElement()->getElement()->cartas;
        if (cartasDestino->isEmpty()) {
            return true;
        }

        cartasDestino->goToPos(cartasDestino->getSize() - 1);
        Card* cartaCimaDestino = cartasDestino->getElement();
        return cartaCimaDestino->numero == primeraCartaMovida->numero + 1;
    }
     
    // funcion para remover cartas de una escalera completada
    void retirarEscalera(int col) {
        columnas->goToPos(col);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        cartas->goToPos(cartas->getSize() - 13);               // primera carta de la escalera
        for (int i = 0; i < 13; i++) {
            delete cartas->remove();
        }
        escalerasCompletadas++;
        if (!cartas->isEmpty()) {                              // la nueva última pasa a descubierta
            cartas->goToPos(cartas->getSize() - 1);
            Card* nueva = cartas->getElement();
            nueva->unCover();
            if (!nueva->up) nueva->flip();
        }
    }

    void moverCartas(int colOrigen, int colDestino, int cantidadCartas) {
        if (esMovimientoValido(colOrigen, colDestino, cantidadCartas)) {
            columnas->goToPos(colDestino);
            LinkedList<Card*>* cartasDestino = columnas->getElement()->getElement()->cartas;
            columnas->goToPos(colOrigen);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            if (!cartasDestino->isEmpty()) {
                cartasDestino->goToPos(cartasDestino->getSize() - 1);
                cartasDestino->getElement()->unCover();
            }
            cartas->goToPos(cartas->getSize() - cantidadCartas);
            for (int i = 0; i < cantidadCartas; i++) {
                cartasDestino->append(cartas->remove());
            }
            if (!cartas->isEmpty()) {
                cartas->goToPos(cartas->getSize() - 1);
                Card* nueva = cartas->getElement();
                nueva->unCover();
                if (!nueva->up) nueva->flip();
            }
            if (verificarEscaleraCompleta(colDestino)) retirarEscalera(colDestino);
        }
    }

    void repartirMazo() {
        if (mazo.isEmpty()) return;
        for (int i = 0; i < 10; i++) {                         // no se reparte con columnas vacías
            columnas->goToPos(i);
            if (columnas->getElement()->getElement()->cartas->isEmpty()) return;
        }
        for (int i = 0; i < 10; i++) {
            columnas->goToPos(i);
            LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
            cartas->goToPos(cartas->getSize() - 1);
            cartas->getElement()->unCover();
            if (!mazo.isEmpty()) {
                Card* c = mazo.pop();
                c->unCover();
                c->flip();
                cartas->append(c);
            }
        }
        for (int i = 0; i < 10; i++) {
            if (verificarEscaleraCompleta(i)) retirarEscalera(i);
        }
        vecesRepartido++;
    }

    bool verificarEscaleraCompleta(int col) {
        columnas->goToPos(col);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        if (cartas->getSize() < 13) return false;              // MODIFICADO: cubre también la columna vacía
        int posInicio = cartas->getSize() - 13;                // MODIFICADO: últimas 13, no primeras
        for (int i = 0; i < 13; i++) {
            cartas->goToPos(posInicio + i);
            Card* c = cartas->getElement();
            if (!c->up || c->numero != 13 - i) return false;  // MODIFICADO: K, Q, J... hasta A, todas boca arriba
        }
        return true;
    }

    bool haGanado() {
        return (escalerasCompletadas == 8);
    }

    void mostrarTablero() {
        system("cls");

        const string P = "\xE2\x99\xA0"; // Símbolo de Picas

        cout << " ========================================================================\n";
        cout << "                                  SPIDER                                  \n";
        cout << " ========================================================================\n";
        cout << "  Escaleras completas: " << escalerasCompletadas << " / 8\n";
        cout << "  Cartas en el mazo: " << mazo.getSize() << " (Repartos disponibles: " << (mazo.getSize() / 10) << ")\n";
        cout << " ------------------------------------------------------------------------\n\n";

        // Cabecera de columnas alineada
        for (int c = 0; c < 10; c++) {
            string n = to_string(c + 1);
            cout << "    " << n << string(7 - n.size(), ' '); 
        }
        cout << "\n";

        impresionBarajas(columnas);
        
        cout << "\n ------------------------------------------------------------------------\n";
        cout << "  [1] Mover   [2] Repartir Mazo   [3] Deshacer   [4] Rehacer   [5] Abandonar\n";
        cout << " ------------------------------------------------------------------------\n";
    }
};

// Declaraciones de baraja Spider
void crearBarajaSpider(Card* baraja[104]);
void deleteBarajaSpider(Card* baraja[104]);
void mezclarBarajaSpider(Card* baraja[104]);