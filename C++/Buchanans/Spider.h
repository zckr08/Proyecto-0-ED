#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "DLinkedList.h"
#include "LinkedStack.h"

using namespace std;

// Todos queremos a MIKEL... Atte: Equipo-Mikel
// comentarios desde aquí son de Mikel, para documentar como nunca...

class Spider {
private:
    DLinkedList<Card*> columnas[10]; // 10 columnas
    LinkedStack<Card*> mazo;        // Mazo en pila
    int escalerasCompletadas;       // Escaleras completas
    int vecesRepartido;             // Veces repartido

public:
    Spider() {
        escalerasCompletadas = 0;
        vecesRepartido = 0;
    }

    ~Spider() {
        for (int i = 0; i < 10; i++) {
            columnas[i].goToStart();
            while (!columnas[i].atEnd()) {
                delete columnas[i].getElement();
                columnas[i].next();
            }
            columnas[i].clear();
        }

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
                columnas[col].append(cartaActual);
            }
        }

        while (index < 104) {
            Card* cartaMazo = baraja[index++];
            mazo.push(cartaMazo);
        }
    }

    bool esMovimientoValido(int colOrigen, int colDestino, int cantidadCartas) {
        if (colOrigen < 0 || colOrigen >= 10 || colDestino < 0 || colDestino >= 10) {
            return false;
        }
        if (columnas[colOrigen].getSize() == 0 || cantidadCartas <= 0) {
            return false;
        }
        if (cantidadCartas > columnas[colOrigen].getSize()) {
            return false;
        }

        int tamOrigen = columnas[colOrigen].getSize();
        int posInicio = tamOrigen - cantidadCartas;

        for (int i = 0; i < cantidadCartas - 1; i++) {
            columnas[colOrigen].goToPos(posInicio + i);
            Card* cartaActual = columnas[colOrigen].getElement();
            columnas[colOrigen].goToPos(posInicio + i + 1);
            Card* cartaSiguiente = columnas[colOrigen].getElement();

            if (cartaActual->numero != cartaSiguiente->numero + 1) {
                return false;
            }
        }

        if (columnas[colDestino].getSize() == 0) {
            return true;
        }

        columnas[colOrigen].goToPos(posInicio);
        Card* primeraCartaMovida = columnas[colOrigen].getElement();

        columnas[colDestino].goToEnd();
        Card* cartaCimaDestino = columnas[colDestino].getElement();
        if (cartaCimaDestino->numero == primeraCartaMovida->numero + 1) {
            return true;
        }
        return false;
    }

    void moverCartas(int colOrigen, int colDestino, int cantidadCartas) {
        if (esMovimientoValido(colOrigen, colDestino, cantidadCartas)) {
            columnas[colOrigen].goToEnd();
            for (int i = 0; i < cantidadCartas; i++) {
                columnas[colOrigen].previous();
            }
            for (int i = 0; i < cantidadCartas; i++) {
                Card* c = columnas[colOrigen].remove();
                c->unCover();
                c->flip();
                columnas[colDestino].append(c);
                columnas[colOrigen].next();
            }
        }
    }

    void repartirMazo() {
        if (mazo.isEmpty()) return;
        for (int i = 0; i < 10; i++) {
            if (!mazo.isEmpty()) {
                Card* c = mazo.pop();
                c->unCover();
                c->flip();
                columnas[i].append(c);
            }
        }
    }

    bool verificarEscaleraCompleta(int col) {
        int revisar = 0;
        columnas[col].goToStart();
        for (int i = 0; i < 13; i++) {
            if (columnas[col].atEnd()) {
                columnas[col].goToEnd();
                return false;
            }

            if (columnas[col].getElement()->up == false) {
                columnas[col].goToEnd();
                return false;
            }

            revisar += columnas[col].getElement()->numero;
            columnas[col].next();
        }

        return (revisar == 91);
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
        cout << "  ";
        for (int c = 0; c < 10; c++) {
            string n = to_string(c + 1);
            if (c < 9) {
                cout << "  " << n << "    ";
            }
            else {
                cout << "  " << n << "   ";
            }
        }
        cout << "\n";

        // Vector para almacenar las líneas de texto generadas por cada columna
        vector<string> lineasColumna[10];
        int maxLineas = 0;

        for (int c = 0; c < 10; c++) {
            int tam = columnas[c].getSize();
            for (int i = 0; i < tam; i++) {
                columnas[c].goToPos(i);
                Card* carta = columnas[c].getElement();

                // CASO 1: Carta boca abajo -> Compacta de 1 sola línea (|###|)
                if (!carta->up) {
                    lineasColumna[c].push_back("|###|  ");
                }
                // CASO 2: Carta boca arriba -> Tamaño completo con cajita de 4 líneas
                else {
                    string simboloNum = "";
                    if (carta->numero == 1) simboloNum = "A";
                    else if (carta->numero == 11) simboloNum = "J";
                    else if (carta->numero == 12) simboloNum = "Q";
                    else if (carta->numero == 13) simboloNum = "K";
                    else simboloNum = to_string(carta->numero);

                    string cuerpo = "";
                    if (simboloNum.length() == 1) {
                        cuerpo = "|" + simboloNum + P + " |  ";
                    }
                    else {
                        cuerpo = "|" + simboloNum + P + "|  ";
                    }

                    lineasColumna[c].push_back("+---+  ");
                    lineasColumna[c].push_back(cuerpo);
                    lineasColumna[c].push_back(cuerpo);
                    lineasColumna[c].push_back("+---+  ");
                }
            }
            if (lineasColumna[c].size() > maxLineas) {
                maxLineas = lineasColumna[c].size();
            }
        }

        if (maxLineas == 0) {
            cout << "   [ Tablero Vacio o Inicializando... ]\n";
            return;
        }

        // Renderizado horizontal coordinado línea por línea
        for (int l = 0; l < maxLineas; l++) {
            cout << "  ";
            for (int c = 0; c < 10; c++) {
                if (l < lineasColumna[c].size()) {
                    cout << lineasColumna[c][l];
                }
                else {
                    cout << "       "; // Espacios de relleno alineados si la columna ya terminó
                }
            }
            cout << "\n";
        }

        cout << "\n ------------------------------------------------------------------------\n";
        cout << "  [1] Mover   [2] Repartir Mazo   [3] Deshacer   [4] Rehacer   [5] Abandonar\n";
        cout << " ------------------------------------------------------------------------\n";
    }
};

// Declaraciones de baraja Spider
void crearBarajaSpider(Card* baraja[104]);
void deleteBarajaSpider(Card* baraja[104]);
void mezclarBarajaSpider(Card* baraja[104]);