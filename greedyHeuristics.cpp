#include "greedyHeuristics.hpp"

int coloreoSecuencial(vector<Vertice>& orden, Coloreo& ordenDeColores, Grafo& G) {
	int cantVertices = (int)orden.size();
    int maxColor = -1;
	// Inicializo el vector que indica los colores usados y sin usar
	vector<bool> coloresSinUsar(cantVertices, false);
	Vertice v;
	int cr;
	// Recorro el grafo
	for (int i = 0; i < cantVertices; i++) { // O(n+m)
		v = orden[i];
		// Todos los colores asignados en la iteración anterior se ponen como "en uso" (true)	
		for (long unsigned int j = 0; j < G[v-1].size(); j++)
			if (ordenDeColores[G[v-1][j]-1] != UNDEFINED)
				coloresSinUsar[ordenDeColores[G[v-1][j]-1]] = true;
		// Se utiliza el primer color sin utilizar para colorear el vertice
		cr = 0;
		while (cr < cantVertices && coloresSinUsar[cr])
			cr++;
		if(ordenDeColores[v-1]==UNDEFINED)ordenDeColores[v-1] = cr;
		// Actualizamos el color maximo utilizado hasta ahora
		if(cr > maxColor) maxColor = cr;
   }
   return maxColor;
}
void busquedaLocalGolosa(int maxColor, int mayorImpacto,Coloreo& ordenDeColores,Grafo& H, Grafo& G){
	// Heuristica de busqueda local
	int cantVertices = (int)H.size();
    //pra cada arista en H
	for(int j = 0; j < cantVertices; j++) { 
		for(long unsigned int k = 0; k < H[j].size(); k++) {
			//si no tienen el mismo color intentamos colorearlos iguales
			if(ordenDeColores[j] != ordenDeColores[H[j][k]-1] && !seEncuentra(H[j][k],G[j])) {
				// guardamos los colores de los vertices actuales
				int colorAnteriorColores = ordenDeColores[j];
				int colorAnteriorGrafoH = ordenDeColores[H[j][k]-1];
				// Si puedo colorear ambos con un nuevo color sin excederme lo hago
				if(maxColor + 1 < cantVertices && colorValido(H[j][k],maxColor+1,ordenDeColores,G) && colorValido(j+1,maxColor+1,ordenDeColores,G)){
					ordenDeColores[H[j][k]-1] = maxColor+1;
					ordenDeColores[j] = maxColor + 1;
					maxColor++;
				}
				// Si el impacto luego de agregar el color es menor o igual, entonces lo dejamos como estaba
				//sino actualizamos el impacto 
				int impactoEncontrado = impactoDeColoreo(ordenDeColores,H);
				if(impactoEncontrado > mayorImpacto) {
					mayorImpacto = impactoEncontrado;
				} else {
					ordenDeColores[H[j][k]-1] = colorAnteriorGrafoH;
					ordenDeColores[j] = colorAnteriorColores;
					maxColor--;
				}
			}
		}
	}
}
void coloreoVerticeSegunH(vector<Vertice> verticesAColorear, int& cr, Coloreo& ordenDeColores, Grafo& H,Grafo& G){
	int color = UNDEFINED;
	long unsigned i = 0;
	//vamos a colorear el ultimo vertice
	Vertice ultimo = verticesAColorear[verticesAColorear.size()-1];
	while(i < verticesAColorear.size()-1 && color == UNDEFINED){
		Vertice iesimo = verticesAColorear[i];
		//si son adyacentes intento colorearlos con el mismo color
		if(seEncuentra(iesimo, H[ultimo-1])){
			//si iesimo ya tiene asigando un color intentamos ponerle el mismo color
			if( ordenDeColores[iesimo-1]!=UNDEFINED && colorValido(ultimo, ordenDeColores[iesimo-1],ordenDeColores,G))
				color = ordenDeColores[iesimo-1];
			
			
		}
		i++;
	}
	// Si no compartia vertice con ninguno, le pongo un color nuevo
	ordenDeColores[ultimo-1] = color == UNDEFINED ? cr : color;
	if(color == UNDEFINED)cr++;	
}
void coloreoAristaSegunH(vector<Arista> aristasQueNoEstanEnG, int& cr, Coloreo& ordenDeColores, Grafo& G){
	int color = UNDEFINED;
	long unsigned i = 0;
	Arista ultimaAristaAgregada = aristasQueNoEstanEnG[aristasQueNoEstanEnG.size()-1];
	Vertice restante = UNDEFINED;
	while(i < aristasQueNoEstanEnG.size()-1 && color == UNDEFINED){
		Arista iesima = aristasQueNoEstanEnG[i];
		Vertice compartido = verticeCompartido(iesima, ultimaAristaAgregada);
		// Si comparten un vertice
		if(compartido != UNDEFINED && ordenDeColores[compartido-1]!= UNDEFINED){
			restante = compartido != ultimaAristaAgregada.first ? ultimaAristaAgregada.first : ultimaAristaAgregada.second;
			//vamos a ver si lo podemos colorear con el mismo color y resulta válido en G
			//si es valido colorear el restante del mismo color, lo hacemos
			if(ordenDeColores[restante-1]==UNDEFINED && colorValido(restante,ordenDeColores[compartido-1],ordenDeColores,G)){
				color = ordenDeColores[compartido - 1];
			}

		}
		i++;
	}
	// Si no compartia vertice con ninguno, le pongo un color nuevo
	if(color == UNDEFINED){
		ordenDeColores[ultimaAristaAgregada.first-1] = cr;
		ordenDeColores[ultimaAristaAgregada.second-1] = cr;
		cr++;
	}else 
		ordenDeColores[restante-1] = color;
}
