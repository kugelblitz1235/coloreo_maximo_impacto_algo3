#include "definesAndGlobals.hpp"

//===================================================== AUXILIARES ==========================================================
int impactoDeColoreo(Coloreo& colores, Grafo H) {
	int impacto = 0;
	// Buscamos las aristas que coinciden en color
	for(long unsigned j = 0; j < H.size(); j++) //O(mG + mH)
		for(long unsigned int k = 0; k < H[j].size(); k++)		//NOTA : esto tal vez con la matriz de adyacencia hubise sido mucho mas facil
			if(colores[j] == colores[H[j][k]-1]) impacto++;
	return impacto/2;
}

//dado un coloreo, verifica si es válido. 
//debug : si es true se generan prints, sino no
bool verificadorColoreo(Coloreo coloreo, bool debug, Grafo G){
	bool coloreoInvalido = false;
	 
	for(long unsigned i = 0 ; i < G.size() ; i++)
		for(long unsigned j = 0 ; j < G[i].size() ; j++ )
			if(coloreo[i] == coloreo[G[i][j]-1]){
				if(debug)cout<<"Hay un coloreo inválido en los vertices :"<<i+1<<" y "<<G[i][j]<<endl;
				coloreoInvalido = true;
			}
	if(coloreoInvalido){

		cout << "Grafo G :"<<endl;
		for(long unsigned j = 0 ; j < G.size() ; j++){ cout << "vertice :"<< j+1 <<endl; 
			for(long unsigned int k = 0 ; k < G[j].size() ; k++){ cout<<G[j][k]<<" ";}cout<<endl;
		}
		
		cout<<"Coloreo obtenido :"<<endl;
		for(long unsigned i = 0; i < G.size(); i++){cout << coloreo[i] << " ";} cout<<endl;
	}
	return coloreoInvalido;
}

// Retorna el vertice que comparten las aristas, en caso de no compartir ninguno, retornamos UNDEFINED
Vertice verticeCompartido(Arista a, Arista b) {
	if(a.first == b.first || a.first == b.second)
		return a.first;
	else if(a.second == b.first || a.second == b.second)
		return a.second;
	else
		return UNDEFINED;
}

//Chequea si el segundo vertice pertenece al vector de adyacencia
bool seEncuentra(Vertice vertice, vector<Vertice> vertices) {
	long unsigned i = 0;
	while(i < vertices.size() && vertices[i]!=vertice) i++;
	return i < vertices.size();	
}

bool colorValido(Vertice vertice, int color, Coloreo ordenDeColores, Grafo G){
	long unsigned j = 0 ;
	while( j < G[vertice-1].size() && color != ordenDeColores[G[vertice-1][j]-1])j++;
	return j == G[vertice-1].size();
}