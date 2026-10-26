#pragma once
#include <iostream>
#include <string>
#include "Card.h"
#include "LinkedList.h"
#include "ArrayList.h"
#include "Mazo.h"
#include "LinkedStack.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

class Spider {
private:
    ArrayList<LinkedList<Mazo*>*>* columnas;
    LinkedStack<Card*> mazo;
    int escalerasCompletadas;
    int vecesRepartido;

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

    // Dibuja las 10 columnas lado a lado, línea por línea de cada carta.
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
    // Crea el juego con 10 columnas vacías y los contadores en cero.
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

    // Borra todo lo que el juego tenga: columnas y mazo.
    ~Spider() {
        liberarColumnas(columnas);
        vaciarPila(mazo);
    }

    // Reparte las 104 cartas: 6 en las primeras 4 columnas, 5 en las demás.
    // Solo la última de cada columna queda boca arriba y el resto va al mazo.
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

    // Revisa si se pueden mover esas cartas: boca arriba, en escalera bajando de uno en uno,
    // y que caigan sobre una carta un número mayor (o en columna vacía).
    bool esMovimientoValido(int colOrigen, int colDestino, int cantidadCartas) {
        if (colOrigen < 0 || colOrigen >= 10 || colDestino < 0 || colDestino >= 10 || colOrigen == colDestino) {
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
        if (!primeraCartaMovida->up) return false;

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

    // Método para pruebas rápidas porque soy un hombre ocupado de 21 años
    // Marca las 8 escaleras como hechas para ganar al instante.
    void forzarVictoriaPrueba() {
        escalerasCompletadas = 8;
    }

    // Borra las 13 cartas de arriba de la columna (la escalera completa) y suma una escalera.
    // Después voltea la carta que quedó al descubierto.
    void retirarEscalera(int col) {
        columnas->goToPos(col);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        cartas->goToPos(cartas->getSize() - 13);
        for (int i = 0; i < 13; i++) {
            delete cartas->remove();
        }
        escalerasCompletadas++;
        if (!cartas->isEmpty()) {
            cartas->goToPos(cartas->getSize() - 1);
            Card* nueva = cartas->getElement();
            nueva->unCover();
            if (!nueva->up) nueva->flip();
        }
    }

    // Mueve el grupo de cartas de una columna a otra si la jugada es válida.
    // Devuelve true si se movió y false si no (y avisa el error).
    bool moverCartas(int colOrigen, int colDestino, int cantidadCartas) {
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
            return true;
        }
        cout << "ERROR!! SE HA USTED EQUIVOCADO CUAL CHAVISTA, movimiento no válido en Spider (verifique que las cartas estén en escalera ascendente y boca arriba).\n";
        cout << "Presione ENTER para continuar...";
        string p; getline(cin, p);
        return false;
    }

    // Pone una carta del mazo boca arriba sobre cada una de las 10 columnas.
    // Devuelve false si el mazo está vacío o si alguna columna no tiene cartas.
    bool repartirMazo() {
        if (mazo.isEmpty()) return false;
        for (int i = 0; i < 10; i++) {
            columnas->goToPos(i);
            if (columnas->getElement()->getElement()->cartas->isEmpty()) return false;
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
        return true;
    }

    // Mira las últimas 13 cartas de la columna: deben ser boca arriba y bajar de K a A.
    // Devuelve true si forman una escalera completa.
    bool verificarEscaleraCompleta(int col) {
        columnas->goToPos(col);
        LinkedList<Card*>* cartas = columnas->getElement()->getElement()->cartas;
        if (cartas->getSize() < 13) return false;
        int posInicio = cartas->getSize() - 13;
        for (int i = 0; i < 13; i++) {
            cartas->goToPos(posInicio + i);
            Card* c = cartas->getElement();
            if (!c->up || c->numero != 13 - i) return false;
        }
        return true;
    }

    // Dice si ya se completaron las 8 escaleras (victoria).
    bool haGanado() {
        return (escalerasCompletadas == 8);
    }

    // Limpia la pantalla y dibuja escaleras, mazo, repartos disponibles, columnas y menú.
    void mostrarTablero() {
        system("cls");
        cout << " ========================================================================\n";
        cout << "                                SPIDER                                   \n";
        cout << " ========================================================================\n";
        cout << "  Escaleras completas: " << escalerasCompletadas << " / 8\n";
        cout << "  Cartas en el mazo: " << mazo.getSize() << " (Repartos disponibles: " << (mazo.getSize() / 10) << ")\n";
        cout << " ------------------------------------------------------------------------\n\n";

        for (int c = 0; c < 10; c++) {
            string n = to_string(c + 1);
            cout << "    " << n << string(7 - n.size(), ' ');
        }
        cout << "\n";

        impresionBarajas(columnas);

        cout << "\n --------------------------------------------------------------------------------\n";
        cout << "  [1] Mover     [2] Repartir Mazo    [3] Deshacer    [4] Rehacer    [5] Abandonar\n";
        cout << " --------------------------------------------------------------------------------\n";
    }

    // Devuelve las columnas reales del juego (el historial las copia).
    ArrayList<LinkedList<Mazo*>*>* getColumnas() {
        return columnas;
    }
    // Devuelve el mazo real; el historial lo vacía y lo vuelve a llenar igual.
    LinkedStack<Card*>* getMazo() {
        return &mazo;
    }
    // Devuelve cuántas escaleras van completas.
    int getEscalerasCompletadas() {
        return escalerasCompletadas;
    }
    // Devuelve cuántas veces se ha repartido el mazo.
    int getVecesRepartido() {
        return vecesRepartido;
    }
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
    // Guarda la cantidad de escaleras completadas.
    void setEscalerasCompletadas(int escalerasCompletadas) {
        this->escalerasCompletadas = escalerasCompletadas;
    }
    // Guarda cuántas veces se ha repartido.
    void setVecesRepartido(int vecesRepartido) {
        this->vecesRepartido = vecesRepartido;
    }
};