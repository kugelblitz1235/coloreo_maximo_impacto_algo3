#include "tabu_search.hpp"
//evalua si se cumple el criterio de corte por iteraciones sin mejora o iteraciones
bool continuarTabuSearch(int iteracionesSinMejora, int iteraciones, int limiteIteraciones, int limiteIteracionesSinMejora){ 
	return iteracionesSinMejora < limiteIteracionesSinMejora && iteraciones < limiteIteraciones;
}

bool mismaEstructuraDeSolucion(Solucion a, Solucion b,bool memoriaUltimasSoluciones){
	if(memoriaUltimasSoluciones){
		return a.coloreo == b.coloreo;
	}
	else
		return a.indiceDeSwap == b.indiceDeSwap && (a.aplicacionDeChange == b.aplicacionDeChange && a.colorChange == b.colorChange);
	
}

bool solucionExplorada(Solucion solucion, vector<Solucion>& memoriaTabu, bool memoriaUltimasSoluciones){
	long unsigned j = 0; 
	while (j < memoriaTabu.size() ){
		if(mismaEstructuraDeSolucion(solucion, memoriaTabu[j],memoriaUltimasSoluciones))
			break;
		j++;
	} 
	return j < memoriaTabu.size();	
}

void vecindadPorAristasEnH(vector<Solucion>& vecinos, bool memoriaUltimasSoluciones,Solucion solOriginal, Grafo& H, Grafo& G, vector<Solucion>& memoriaTabu){
	int cantVertices = (int)solOriginal.coloreo.size();
	vector<bool> coloresUsados = vector<bool>(cantVertices, false);
	for(int i = 0; i < cantVertices; i++)coloresUsados[solOriginal.coloreo[i]]=true;
	
	//recorremos las aristas de H
	for(int j = 0; j < cantVertices; j++) { 
		for(long unsigned int k = 0; k < H[j].size(); k++) {
			//intentamos colorear aristas de H del mismo color
			if(solOriginal.coloreo[j] != solOriginal.coloreo[H[j][k]-1]) {
				// Si no son adyacentes en G

				if(!seEncuentra(H[j][k],G[j])){
					int colorAnteriorGrafoH = solOriginal.coloreo[H[j][k]-1];
					int colorAnteriorColores = solOriginal.coloreo[j];

					if( colorValido(j+1,solOriginal.coloreo[H[j][k]-1],solOriginal.coloreo, G))
						solOriginal.coloreo[j] = solOriginal.coloreo[H[j][k]-1];

					//intento colorear ambos vertices del mismo color sin agregar uno nuevo
					else if (colorValido(H[j][k],solOriginal.coloreo[j],solOriginal.coloreo, G))
						solOriginal.coloreo[H[j][k]-1] = solOriginal.coloreo[j];
					//si no podemos colorearlas sin usar un color nuevo intentamos agregar un color nuevo
					if(solOriginal.coloreo[j] != solOriginal.coloreo[H[j][k]-1]){
						//coloreamos ambos extremos de un color que no se haya utilizado
						int color = 0;
						while( color < cantVertices && coloresUsados[color])color++;
						solOriginal.coloreo[j] = color;
						solOriginal.coloreo[H[j][k]-1] = color;
					}
					//agrego la solucion con la arista del mismo color
					if(!solucionExplorada(solOriginal,memoriaTabu,memoriaUltimasSoluciones))vecinos.push_back(solOriginal);
					
					solOriginal.coloreo[H[j][k]-1] = colorAnteriorGrafoH;
					solOriginal.coloreo[j] = colorAnteriorColores;
					
				}
			}
		}
	}
		
}

vector<Solucion> vecindadPorSwap(Solucion solucion,bool change, bool memoriaUltimasSoluciones, vector<Solucion>& memoriaTabu, Grafo& G, Grafo& H){
	int cantVertices = (int)solucion.coloreo.size();
	vector<Solucion> vecindad = vector<Solucion>();
	if(change)
		solucion = aplicoChange(solucion,H,G);
	
	for(int i = 0 ; i < cantVertices - 1; i++){
		for(int j = i+1 ; j < cantVertices ; j++){
			//antes de aplicar el swap chequeamos que el coloreo producido sea válido, de no serlo. No se aplica la operacion
			
			if(solucion.coloreo[i]!=solucion.coloreo[j] && colorValido(i+1,solucion.coloreo[j],solucion.coloreo, G) && colorValido(j+1,solucion.coloreo[i],solucion.coloreo, G)){
	
				iter_swap(solucion.coloreo.begin() + i, solucion.coloreo.begin() + j);
				Solucion solVecina;
				solVecina.coloreo = Coloreo(solucion.coloreo) ;
				if(!memoriaUltimasSoluciones){
					solVecina.indiceDeSwap = make_pair(j+1,i+1);
					solVecina.aplicacionDeChange = solucion.aplicacionDeChange;
					solVecina.colorChange = solucion.colorChange;
				}
				if(!solucionExplorada(solVecina,memoriaTabu, memoriaUltimasSoluciones)){
					vecindad.push_back(solVecina);
				}
				iter_swap(solucion.coloreo.begin()+j, solucion.coloreo.begin()+i);
			}
		}
	}
	
	return vecindad;
}
vector<Solucion> seleccionoSubvecindadPorRandom(vector<Solucion>& vecinos,int porcentajeSubvecindad, bool memoriaUltimasSoluciones){
	if(porcentajeSubvecindad == 100) 
		return vecinos;
	//calculamos la cantidad de elementos a seleccionar
	float porcentaje = (float)porcentajeSubvecindad;
	int elementosASeleccionar = ceil((vecinos.size()*(porcentaje / 100)));
	vector<Solucion> subVecindad(elementosASeleccionar, Solucion());
	// genera un vector con indices random los cuales indican el elemento a seleccionar de la vecindad :
	vector<int> index(elementosASeleccionar);
	random_device rnd_device;
    // Especificamos la distribucion 
    mt19937 mersenne_engine {rnd_device()};  // Generamos enteros random
    uniform_int_distribution<int> dist {0, (int)vecinos.size()-1};
	auto gen = [&dist, &mersenne_engine](){return dist(mersenne_engine);};
	
    generate(begin(index), end(index), gen); 

	for(int i = 0 ; i < elementosASeleccionar ; i++){
		subVecindad[i] = vecinos[index[i]];
	}

	return subVecindad;
	
}

Solucion aplicoChange(Solucion solucion, Grafo& H, Grafo& G) {
	//este vector va a ser utilizado para indicar que colores estan siendo utilizados en la solucion actual
    int cantVertices = (int) solucion.coloreo.size();
	int impactoActual = impactoDeColoreo(solucion.coloreo,H);
	pair<int,int> change=make_pair(UNDEFINED,UNDEFINED);
	Coloreo nuevoColoreo = Coloreo(solucion.coloreo);
	vector<bool> coloresUsados = vector<bool>(cantVertices, false);
	bool incremento = false;
	//indico que colores estan siendo utilizados
	for(int i = 0; i < cantVertices; i++)coloresUsados[solucion.coloreo[i]]=true;
	
	//busco el primer color si usar
	int color = 0;
	while( color < cantVertices && coloresUsados[color])color++;
	
	//en el caso de que todos los colores esten utilizados devuelvo la solucion sin aplicar change
	if(color==cantVertices) 
		return solucion;

	for(int i = 0 ; i<cantVertices && !incremento; i++){
		for(long unsigned k = 0 ; k < H[i].size() && !incremento ; k++){
			
			if(nuevoColoreo[i] != nuevoColoreo[H[i][k]-1]) {
				// Si no son adyacentes en G

				if(!seEncuentra(H[i][k],G[i])){
					int colorAnteriorGrafoH = nuevoColoreo[H[i][k]-1];
					int colorAnteriorColores = nuevoColoreo[i];
					//coloreamos ambos extremos de un color que no se haya utilizado
					nuevoColoreo[i] = color;
					nuevoColoreo[H[i][k]-1] = color;
					
					//agrego la solucion con la arista del mismo color
					if(impactoDeColoreo(nuevoColoreo,H) > impactoActual){
						change = make_pair(i+1, H[i][k]);
						incremento = true;
					}
					else{
						nuevoColoreo[H[i][k]-1] = colorAnteriorGrafoH;
						nuevoColoreo[i] = colorAnteriorColores;
					}
					
				}
			}
		}
	}	
	if(incremento) {
		solucion.coloreo = nuevoColoreo;
		solucion.aplicacionDeChange = change;
		solucion.colorChange = color;
		solucion.indiceDeSwap = solucion.indiceDeSwap;
	}
	return solucion;
}

vector<Solucion> subVecindad(Solucion solOriginal,bool change, int porcentajeSubvecindad,bool vecindadPorColoreoEnH,bool memoriaUltimasSoluciones,vector<Solucion>& memoriaTabu,Grafo& H, Grafo& G){
	//validamos que el porcentaje ingresado sea valido
	vector<Solucion> vecinos = vector<Solucion>();
    if(porcentajeSubvecindad > 100 || porcentajeSubvecindad < 0) {
		cout<<"ingresó un porcentaje a explorar inválido."<<endl;
		return vecinos;
	}

	if(!vecindadPorColoreoEnH){
		vecinos = vecindadPorSwap(solOriginal,change ,memoriaUltimasSoluciones,memoriaTabu,G,H);
	}else
		vecindadPorAristasEnH(vecinos,memoriaUltimasSoluciones, solOriginal,H,G,memoriaTabu);
	//retorno subvecindad
	return seleccionoSubvecindadPorRandom(vecinos,porcentajeSubvecindad, memoriaUltimasSoluciones);
}


Solucion mejorOrdenEntreVecinos(vector<Solucion> vecinos,Solucion solucionActual ,Grafo& H, vector<Solucion>& memoriaTabu, bool memoriaUltimasSoluciones) {
	int mejorImpacto = -1;
	int impactoVecinoActual;
	for (long unsigned int i = 0; i < vecinos.size(); i++){
		impactoVecinoActual = impactoDeColoreo(vecinos[i].coloreo, H);
		if(impactoVecinoActual > mejorImpacto && ! solucionExplorada(vecinos[i], memoriaTabu, memoriaUltimasSoluciones)) {
			solucionActual.coloreo = Coloreo(vecinos[i].coloreo);
			solucionActual.indiceDeSwap = vecinos[i].indiceDeSwap;
			solucionActual.aplicacionDeChange = vecinos[i].aplicacionDeChange;
			mejorImpacto = impactoVecinoActual;
		}
	}
	return solucionActual;
}
