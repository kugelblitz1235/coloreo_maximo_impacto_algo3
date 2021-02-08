#ifndef DEFINE_H
#define DEFINE_H

#include <random>
#include <iterator>
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <math.h> 
#include <set>
#include <map>
#define DBG(x) cerr << #x << " = " << (x) <<"\n"

using namespace std;

//=============== Defines para más declaración
constexpr int INFTY = -1*(1 << 28);
constexpr int UNDEFINED = -1;

using Vertice = int;
using Grafo = vector<vector<Vertice>>; // Representacion del grafo con listas de adyacencia 
using Coloreo = vector<Vertice>; // El i-ésimo elemento del vector representa el color que tiene el i-ésimo vertice en el grafo
using Arista = pair<int,int>;
 
//===================================================== AUXILIARES ==========================================================

//nos devuelve el impacto del coloreo sobre el grafo H
int impactoDeColoreo(Coloreo& colores, Grafo H) ;

//dado un coloreo, verifica si es válido. 
//debug : si es true se generan prints, sino no
bool verificadorColoreo(Coloreo coloreo, bool debug, Grafo G);

// Retorna el vertice que comparten las aristas, en caso de no compartir ninguno, retornamos UNDEFINED
Vertice verticeCompartido(Arista a, Arista b) ;

//Chequea si el vertice se encuenta entre los vertices pasados por parametro
bool seEncuentra(Vertice vertice, vector<Vertice> vertices) ;

//chequea si el vertice puede ser coloreado con "color" y esto es valido en el grafo G
bool colorValido(Vertice vertice, int color, Coloreo ordenDeColores, Grafo G);

#endif //DEFINE_H