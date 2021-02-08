#ifndef GREEDY_H
#define GREEDY_H

#include "definesAndGlobals.hpp"
//Parametros:	
// orden : vector<vertice> donde los vertices representan el orden de como se ejecuta el coloreo secuencial 
// ordenDeColores : Coloreo donde el i esimo elemento tiene el color asociado al i+1 vertice
// G : Grafo con el cual se chequea la valide de coloreo en los vertices
// output : maximo color utilizado
int coloreoSecuencial(vector<Vertice>& orden, Coloreo& ordenDeColores, Grafo& G) ;

/*
Parametros:
    maxColor : mayor color utilizado hasta el momento
    mayorImpacto : impacto obtenido en el coloreo realizado
    ordenDeColores : coloreo realizado
    H : grafo donde se mide el impacto
    G : grafo donde se valida el coloreo
Esta función va a realizar una búsqueda local de manera golosa en búsqueda de un incremento del impacto para la solucion obtenida.
*/
void busquedaLocalGolosa(int maxColor, int mayorImpacto,Coloreo& ordenDeColores,Grafo& H, Grafo& G);

/*
Parametros:
    verticesAColorear : vertices que vamos a colorear
    cr : color utilizado hasta el momento
    ordenDeColores : coloreo realizado
    H : grafo donde se mide el impacto
    G : grafo donde se valida el coloreo
Este algoritmo va a buscar colorear el ultimo vertice de verticesAColorear, 
intentando ponerle el mismo color que ya utilice otro vertice o, en caso de no poder, poniendole un color nuevo 
*/
void coloreoVerticeSegunH(vector<Vertice> verticesAColorear, int& cr, Coloreo& ordenDeColores, Grafo& H,Grafo& G);

/*
Parametros:
    aristasQueNoEstanEnG : aristas que no estan en G, las cuales vamos a intentar colorear para incrementar el impacto
    cr : color utilizado hasta el momento
    ordenDeColores : coloreo realizado
    H : grafo donde se mide el impacto
    G : grafo donde se valida el coloreo
Este algoritmo va a buscar colorear el ultimo vertice de aristasQueNoEstanEnG, 
intentando ponerle el mismo color que ya utilice otro vertice en una arista adyacente a el en H, en caso de no poder, poniendole un color nuevo 
*/
void coloreoAristaSegunH(vector<Arista> aristasQueNoEstanEnG, int& cr, Coloreo& ordenDeColores, Grafo& G);

#endif //GREEDY_H