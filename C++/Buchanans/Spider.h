#pragma once
#include <iostream>
#include "Card.h"
#include "DLinkedList.h"
#include "LinkedStack.h"

// Todos queremos a MIKEL... Atte: Equipo-Mikel
// comentarios desde aquí son de Mikel, para documentar como nunca...
class Spider {
private:
    DLinkedList<Card*> columnas[10]; // esto pa las 10 columnas (fácil de recorrer)
    LinkedStack<Card*> mazo;        // esto pal mazo, pila es útil
    int escalerasCompletadas;       // esto pa saber si ganamos
    int vecesRepartido;             // esto por papá Dios y para contar repartidas

public:
    Spider() {
        escalerasCompletadas = 0;
        vecesRepartido = 0;
    }

    ~Spider() {
        // tenemos que limpiarsh cada una de las 10 columnas
        for (int i = 0; i < 10; i++) {
            columnas[i].goToStart();
            while (!columnas[i].atEnd()) {
                delete columnas[i].getElement();
                columnas[i].next();
            }
            columnas[i].clear(); // vaciar todito 
        }

        // ahora hay que limpiar las cartas que quedaron en el mazo por si aquello
        while (!mazo.isEmpty()) {
            delete mazo.pop();
        }
    }

    void iniciarJuego(Card* baraja[104]) {
        int index = 0;
        // Las primeras 4 columnas (del 0 al 3) tienen 6 cartas
        // Las siguientes 6 columnas (del 4 al 9) tienen 5 cartas
        for (int col = 0; col < 10; col++) {
            int cantidadCartas;
            if (col < 4) {
                cantidadCartas = 6;
            }
            else {
                cantidadCartas = 5;
            }

            for (int f = 0; f < cantidadCartas; f++) {
                Card* cartaActual = baraja[index++];

                // okey si esta es la última carta de la pila va boca arriba
                if (f == cantidadCartas - 1) {
                    cartaActual->unCover();
                    cartaActual->flip(); // La pone boca arriba
                }
                else {
                    // Las demás quedan boca abajo (cover = true por defecto, no ocupan unCover aquí)
                }

                // Añadimos la carta a la lista doblemente enlazada de la columna
                columnas[col].append(cartaActual);
            }
        }

        // El resto de las cartas de la baraja van al mazo boca abajo
        while (index < 104) {
            Card* cartaMazo = baraja[index++];
            mazo.push(cartaMazo);
        }
    }

    bool esMovimientoValido(int colOrigen, int colDestino, int cantidadCartas) {
        // revisar que los índices de las columnas estén dentro del rango permitido (0 al 9)
        if (colOrigen < 0|| colOrigen >= 10|| colDestino < 0 ||colDestino >=10) {
            return false;
        }
        // la wea no se puede mover desde una columna vacía ni mover 0 o menos cartas
        if (columnas[colOrigen].getSize() == 0 || cantidadCartas <= 0) {
            return false;
        }
         //revisar  que la cantidad a mover no sea mayor a las cartas que tiene la columna origen
        if (cantidadCartas > columnas[colOrigen].getSize()) {
            return false;
        }

        //otener la posición de inicio del grupo que se quiere mover en la columna origen
        int tamOrigen = columnas[colOrigen].getSize();
        int posInicio = tamOrigen - cantidadCartas;

        //recorremos el subgrupo que se quiere mover para verificar que esté ordenado internamente 
        for (int i = 0; i < cantidadCartas - 1; i++) {
            columnas[colOrigen].goToPos(posInicio + i);
            Card* cartaActual = columnas[colOrigen].getElement();
            columnas[colOrigen].goToPos(posInicio + i + 1);
            Card* cartaSiguiente = columnas[colOrigen].getElement();

            // Validamos que estén boca arriba y que sigan la regla de escalera
           
            if (cartaActual->numero != cartaSiguiente->numero + 1) {
                return false; // El grupo no está ordenado correctamente para ser movido como bloque
            }
        }

        //vamos a validar la compatibilidad con la columna de destino
        if (columnas[colDestino].getSize() == 0) {
            return true;
        }

        // Si la columna de destino no está vacía, la primera carta del grupo a mover 
        //debe colocarse sobre una carta en la cima de la columna destino que sea inmediatamente mayor
        columnas[colOrigen].goToPos(posInicio);
        Card* primeraCartaMovida = columnas[colOrigen].getElement();

        columnas[colDestino].goToEnd(); // Nos vamos a la cima de la columna destino
        Card* cartaCimaDestino = columnas[colDestino].getElement();
        if (cartaCimaDestino->numero == primeraCartaMovida->numero + 1) {
            return true;
        }
        return false;
    
    }

    void moverCartas(int colOrigen, int colDestino, int cantidadCartas) {
        if (esMovimientoValido(colOrigen, colDestino, cantidadCartas)) {            //revisa que el movimiento sea valido
            columnas[colOrigen].goToEnd();                                          //va al final por si acaso
            for (int i = 0; i < cantidadCartas; i++) {                              //recorre hacia atras para apuntar a la carta mas grande a mover
                columnas[colOrigen].previous();
            }
            for (int i = 0; i < cantidadCartas; i++) {                              //vuelve a recorrer hacia abajo, removiendo y appendeando el elemento a la columna destino
                columnas[colDestino].append(columnas[colOrigen].remove());          //se entiende?
                columnas[colOrigen].next();
            }
        }
    }

    void repartirMazo() {
        for (int i = 0; i < 10; i++)                        //Win: recorrer las 10 columas y appendearle un mazo .pop
            columnas[i].append(mazo.pop());
    }


    bool verificarEscaleraCompleta(int col) {
        int revisar = 0;                                    //Win: Esta es la logica que me plante para revisar si esta completa, reviso los numeros de cada carta por como
        columnas[col].goToStart();                          //esta hechas intermanete, es decir, AS = 1, 2 = 2 etc. Si los sumo en un acumulador, para que este completa tendria que ser
        for (int i = 0; i < 14; i++) {                      // 1 + 2 + 3 ... + 13 = 91, si el acumulador es igual a 91 esta completa. 
            if (columnas[col].atEnd())
                columnas[col].goToEnd();                    //Si ya esta al final de la lista, retorna false
                return false;

            if (columnas[col].getElement()->up == false)    //Si esta boca abajo, no se a despejado por ende tambien es falso
                columnas[col].goToEnd();
                return false;

            revisar += columnas[col].getElement()->numero;  //Aqui va sumando el acumulador
            columnas[col].next();                           //pasa a la siguiente carta
        }

        if (revisar == 91)                                  //si la suma da 91, esta completa
            return true;

        else
            return false;
    }


    bool haGanado() {
        if (escalerasCompletadas == 10)
            return true;
        else
            return false;
    }


    void mostrarTablero();


};