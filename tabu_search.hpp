#ifndef TABU_H
#define TABU_H

#include "definesAndGlobals.hpp"
//esta estructura va a representar una solucion dentro la memoria de soluciones de la metaheuristica de tabu search
struct Solucion {
    pair<int,int> indiceDeSwap; // Change: <indice, indice>. Swap: <indice, indice>
	pair<int,int> aplicacionDeChange; //intenta poner dos vertices incidentes a una arista del mismo color, agregando un color nuevo
	int colorChange;
	Coloreo coloreo;  // Coloreo resultado

	//constructor default
	Solucion(){
		indiceDeSwap = make_pair(UNDEFINED, UNDEFINED);
		aplicacionDeChange = make_pair(UNDEFINED, UNDEFINED);
		colorChange = UNDEFINED;
		coloreo = Coloreo();
	}
};

//criterio de parada utilizando iteraciones e iteraciones sin mejora
bool continuarTabuSearch(int iteracionesSinMejora, int iteraciones, int limiteIteraciones, int limiteIteracionesSinMejora);

//evalua si son la misma solucion dependiendo del criterio que estemos utilizando
bool mismaEstructuraDeSolucion(Solucion a, Solucion b,bool memoriaUltimasSoluciones);

//nos indica si la solucion ya fue explorada
bool solucionExplorada(Solucion solucion, vector<Solucion>& memoriaTabu, bool memoriaUltimasSoluciones);

//genera la subvecindad de la solOriginal, utilizando el porcentajeSubVecindad 
/*
	conChange : indica si si va a aplicar change previamente a generar la vecindad por swap
	vecindadPorColoreoEnH : utilizando un criterio de  generacion de vecinos por coloreo en H
	memoriaUltimasSoluciones : para indicar que tipo de soluciones estamos almacenando
	memoriaTabu : es la memoria que contiene las solciones exploradas hasta el momento
*/
vector<Solucion> subVecindad(Solucion solOriginal,bool change, int porcentajeSubvecindad,bool vecindadPorColoreoEnH,bool memoriaUltimasSoluciones,vector<Solucion>& memoriaTabu,Grafo& H, Grafo& G);

//genera a partir de una solucion todas las soluciones vecinas posibles aplicando swap entre los colores
vector<Solucion> vecindadPorSwap(Solucion solucion,bool change,bool memoriaUltimasSoluciones, vector<Solucion>& memoriaTabu, Grafo& G, Grafo& H);

//genera a partir de una solucion, todas las soluciones vecinas en las cuales se puede colorear una arista de H del mismo color
void vecindadPorAristasEnH(vector<Solucion>& vecinos, bool memoriaUltimasSoluciones,Solucion solOriginal, Grafo& H, Grafo& G, vector<Solucion>& memoriaTabu);

//seleccionamos el procentaje de la subvecindad indicado, utilizando un criterio por random de elementos
vector<Solucion> seleccionoSubvecindadPorRandom(vector<Solucion>& vecinos,int porcentajeSubvecindad, bool memoriaUltimasSoluciones);

//aplicamos el operador change sobre una solucion, eligiendo el vertice de manera random
Solucion aplicoChange(Solucion solucion, Grafo& H,Grafo& G);

//dada la solucion actual y las soluciones que podemos explorar, que no esten prohibidas, obtenemos la que nos da un mayor impacto
Solucion mejorOrdenEntreVecinos(vector<Solucion> vecinos,Solucion solucionActual, Grafo& H, vector<Solucion>& memoriaTabu, bool memoriaUltimasSoluciones);

#endif //TABU_H