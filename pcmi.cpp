#include "definesAndGlobals.hpp"
#include "tabu_search.hpp"
#include "greedyHeuristics.hpp"
//============== Variables globales
int cantVertices;
int cantAristasH;
int cantAristasG;

// G : grafo sobre el que debe realizarse el coloreo
Grafo G;
// H : grafo sobre el que debe analizarse el impacto del coloreo anterior
Grafo H;
Coloreo ordenDeColores; // Inicializado antes de cada algoritmo con la cantidad de vertices y todos los valores en -1
//variables globales de tabu
bool memoriaUltimasSoluciones; //Tipo de tabu search
vector<Solucion> memoriaTabu;
int tamanoMemoria;
int porcentajeSubvecindad;
int limiteIteracionesSinMejora;
int limiteIteraciones;
bool vecindadPorColoreoEnH;

bool comparacionPorGradoH(const int& a, const int& b) {
	return H[a-1].size() > H[b-1].size() || (H[a-1].size() == H[b-1].size() && a >= b);
}

bool comparacionPorGradoHAristas(const Arista& a, const Arista& b) {
	long unsigned mayorGradoA = H[a.first-1].size() > H[a.second-1].size() ? H[a.first-1].size() : H[a.second-1].size();
	long unsigned mayorGradoB = H[b.first-1].size() > H[b.second-1].size() ? H[b.first-1].size() : H[b.second-1].size();
	return mayorGradoA > mayorGradoB || (mayorGradoA == mayorGradoB && a > b);
}

bool comparacionPorGradoG(const int& a, const int& b) {
	return G[a-1].size() > G[b-1].size() || (G[a-1].size() == G[b-1].size() && a >= b);
}

//================================================= FIN AUXILIARES ==========================================================
//como la heurística tiene que ser solamente constructiva, usamos el parametro bool en default false. Pero lo dejamos para utilizarlo en la experimentacion
int heuristicaSecuencial(vector<Vertice> ordenVertices, bool conBusquedaLocal, bool porGradoG) {
		// Inicializamos los vertices que vamos a usar para representar el orden
		for(int i = 1; i < cantVertices + 1; i++)ordenVertices[i-1] = i;
		if(porGradoG)
			sort(ordenVertices.begin(), ordenVertices.end(), comparacionPorGradoG);
		else
			sort(ordenVertices.begin(), ordenVertices.end(), comparacionPorGradoH);
		// Ver complejidad del sort en std
		// Realizamos el coloreo de manera secuencial segun orden de vertices por adyacencia en G y desempatando por adyacencia en H de mayor a menor 
		int maxColor = coloreoSecuencial(ordenVertices,ordenDeColores,G);
		if(conBusquedaLocal) {
			int mayorImpacto = impactoDeColoreo(ordenDeColores,H);
			busquedaLocalGolosa(maxColor, mayorImpacto, ordenDeColores, H,G);
		}
		
		return impactoDeColoreo(ordenDeColores,H);
		
}


int heuristicaSegunAristasH() {
	vector<Arista> aristasQueNoEstanEnG = vector<Arista>();
	int cr = 0;
	for(int j = 0; j < cantVertices; j++) { 
		for(long unsigned int k = 0; k < H[j].size(); k++) {
			// Si esta arista no esta en G y no fue coloreada
			if(!seEncuentra(H[j][k],G[j]) && ordenDeColores[j]==UNDEFINED && ordenDeColores[H[j][k]-1]==UNDEFINED && cr < cantVertices) {
				aristasQueNoEstanEnG.push_back(make_pair(j+1, H[j][k]));
				// Coloreamos los vertices de la arista de manera que sea valido ese coloreo en G pero tengan el mismo color
				// Si es imposible ese coloreo ponemos un color nuevo 
				coloreoAristaSegunH(aristasQueNoEstanEnG, cr,ordenDeColores,G);
			}
		}
	}
    vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
    for(int i = 1; i < cantVertices + 1; i++)ordenVertices[i-1] = i;		
    coloreoSecuencial(ordenVertices,ordenDeColores,G); 
    
    return impactoDeColoreo(ordenDeColores,H);
}

int heuristicaSegunVerticesH() {
	//obtengo los vertices que son incidentes a alguna arista en H
	vector<Vertice> verticesConAristasEnH = vector<Vertice>();
	for(int i = 0; i < cantVertices; i++)if(H[i].size() > 0 && !seEncuentra(i+1,verticesConAristasEnH))verticesConAristasEnH.push_back(i+1);
	//los ordeno segun su grado
	sort(verticesConAristasEnH.begin(),verticesConAristasEnH.end(),comparacionPorGradoH);
	vector<Vertice> verticesAColorear = vector<Vertice>();
	Vertice kesimo;
	int cr = 0;
	for(long unsigned j = 0; j < verticesConAristasEnH.size(); j++) { 
		Vertice v = verticesConAristasEnH[j]; 
		if(!seEncuentra(v, verticesAColorear)){

			verticesAColorear.push_back(v);
			coloreoVerticeSegunH(verticesAColorear, cr,ordenDeColores,H,G);
			for(long unsigned int k = 0; k < H[v-1].size(); k++) {
				kesimo = H[v-1][k]; 
				// Si esta arista no esta en G
				if(!seEncuentra(kesimo,G[v-1]) && !seEncuentra(kesimo, verticesAColorear) && cr < cantVertices) {
					verticesAColorear.push_back(kesimo);
					coloreoVerticeSegunH(verticesAColorear, cr,ordenDeColores,H,G);
				}
			}
		}
	}
	//coloreamos los vertices restantes en el grafo de manera secuencial
    vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
    for(int i = 1; i < cantVertices + 1; i++)ordenVertices[i-1] = i;		
    coloreoSecuencial(ordenVertices,ordenDeColores,G); 
    
    return impactoDeColoreo(ordenDeColores,H);
}

int tabuSearch(vector<Vertice> orden) {
	memoriaTabu.resize(tamanoMemoria, Solucion());                        //Memoria de tamaño definido como global
	int mejorImpacto = orden.size()==0? heuristicaSegunVerticesH() : heuristicaSecuencial(orden,false,true );      			  //Empezamos con el impacto que nos da la heuristica que elijamos, puede ser en base al gap                              
	int proximoIndexMemoria = 0;
	int iteracionesSinMejora = 0;	//Condicion de salida
	int impactoActual;
	int iteraciones = 0;
	bool change = false;
	Solucion solucionActual;
	solucionActual.coloreo = ordenDeColores;   // El orden que nos dio la segunda heuristica
	
	vector<Solucion> solucionesVecinas; // Soluciones "vecinas" a la actual

	while(continuarTabuSearch(iteracionesSinMejora,iteraciones,limiteIteraciones,limiteIteracionesSinMejora)) { 	// Criterio para parar. Lo dejo modularizado para futuras ideas
		solucionesVecinas = subVecindad(solucionActual,change,porcentajeSubvecindad,vecindadPorColoreoEnH,memoriaUltimasSoluciones,memoriaTabu,H,G);		//Busco soluciones vecinas
		solucionActual = mejorOrdenEntreVecinos(solucionesVecinas,solucionActual,H,memoriaTabu,memoriaUltimasSoluciones);   // Calculo el mejor entre ellos
		//recuerdo la solucion obtenida
		Solucion sol; 
		if(memoriaUltimasSoluciones){
			sol.coloreo = solucionActual.coloreo;
		}else {
			sol.aplicacionDeChange = solucionActual.aplicacionDeChange;
			sol.indiceDeSwap = solucionActual.indiceDeSwap;
		}
		//si la solucion no se encuetra almacenada en la memoria evaluamos si el impacto obtenido es mejor
		//dado que si se encuentra quiere decir que ya la evaluamos en alguna iteracion anterior
		memoriaTabu[proximoIndexMemoria] = sol; 	// Lo guardo en la memoria para futuras exploraciones
		proximoIndexMemoria = (proximoIndexMemoria + 1) % tamanoMemoria; // Avanzamos el proximo slot a pisar
		impactoActual = impactoDeColoreo(solucionActual.coloreo,H);
		if (impactoActual > mejorImpacto) {               //Si es el mejor, nos lo guardamos
			mejorImpacto = impactoActual;
			ordenDeColores = solucionActual.coloreo; //Junto a su coloreo
			iteracionesSinMejora = 0;
			change = false;
		} else {
			iteracionesSinMejora++;
			change = true;
		}
		iteraciones++;
	}
	return mejorImpacto;

}



// Recibe por parámetro qué algoritmos utilizar para la ejecución separados por espacios.
// Imprime por clog la información de ejecución de los algoritmos.
// Imprime por cout el resultado de algun algoritmo ejecutado.
int main(int argc, char** argv){
	// Leemos el parametro que indica el algoritmo a ejecutar.
	map<string, string> algoritmos_implementados = {
		{"HS1", "Heuristica golosa constructiva secuencial por grado en G"},{"HS1B", "Heuristica golosa constructiva secuencial por grado en G con busqueda local"},
		{"HS2", "Heuristica golosa constructiva secuencial por grado en H"},{"HS2B", "Heuristica golosa constructiva secuencial por grado en H con busqueda local"}, 
		{"HGA", "Heuristica golosa coloreando G por las aristas de H."}, {"HGV", "Heuristica golosa coloreando G por los vertices de H."},
		{"TSS", "Metaheuristica Tabú search con memoria de vertices utilizando la mejor heurística golosa."},
		{"TSE", "Metaheuristica Tabú search con memoria de soluciones utilizando la mejor heurística golosa."},
		{"TSSA", "Metaheuristica Tabú search con memoria de vertices con soluciones vecinas alternativas utilizando la mejor heurística golosa."},
		{"TSSB", "Metaheuristica Tabú search con memoria de vertices utilizando la peor heurística golosa."},
		{"TSEB", "Metaheuristica Tabú search con memoria de soluciones utilizando la peor heurística golosa."},
		{"TSSAB", "Metaheuristica Tabú search con memoria de vertices con soluciones vecinas alternativas utilizando la peor heurística golosa."}
	};

	// Verificar que el algoritmo pedido exista.
	if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end())
	{
		cerr << "Algoritmo no encontrado: " << argv[1] << endl;
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algoritmos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
		return 0;
	}
	string algoritmo = argv[1];
	if(algoritmo[0] == 'T'){
		tamanoMemoria = atoi(argv[2]);
		porcentajeSubvecindad = atoi(argv[3]);
		limiteIteracionesSinMejora = atoi(argv[4]);
		limiteIteraciones = atoi(argv[5]);
	}
    // Leemos el input.
    cin >> cantVertices >> cantAristasG >> cantAristasH;
    G.assign(cantVertices, vector<int>());
    int i,j;
    for (int k = 0; k < cantAristasG; k++){
    	cin >> i >> j;
		G[i-1].push_back(j);
		G[j-1].push_back(i);	
    }

    H.assign(cantVertices, vector<int>());
    for (int k = 0; k < cantAristasH; k++){
    	cin >> i >> j;
		H[i-1].push_back(j);
		H[j-1].push_back(i);	
    }

	//lo inicializamos con valores basura
	ordenDeColores.assign(cantVertices, -1);
    
	// Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.
	int mayor_impacto = INFTY;
	auto start = chrono::steady_clock::now();
if (algoritmo == "HS1") {
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
		mayor_impacto = heuristicaSecuencial(ordenVertices, false, true); 
	}
	else if (algoritmo == "HS1B")
	{
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
		mayor_impacto = heuristicaSecuencial(ordenVertices, true, true); 
	}
	else if (algoritmo == "HS2")
	{
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
		mayor_impacto = heuristicaSecuencial(ordenVertices, false, false); 
	}
	else if (algoritmo == "HS2B")
	{
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, -1); 
		mayor_impacto = heuristicaSecuencial(ordenVertices, true, false); 
	}
	else if (algoritmo == "HGA")
	{
		mayor_impacto = heuristicaSegunAristasH();
	}
	else if (algoritmo == "HGV")
	{
		mayor_impacto = heuristicaSegunVerticesH();
	}
	else if (algoritmo == "TSS")
	{	
		memoriaUltimasSoluciones = true;
		vecindadPorColoreoEnH = false;
		mayor_impacto = tabuSearch(vector<Vertice>());
	}
	else if (algoritmo == "TSE")
	{	
		memoriaUltimasSoluciones = false;
		vecindadPorColoreoEnH = false;
		mayor_impacto = tabuSearch(vector<Vertice>());
	}else if (algoritmo == "TSSA")
	{	
		memoriaUltimasSoluciones = true;
		vecindadPorColoreoEnH = true;
		mayor_impacto = tabuSearch(vector<Vertice>());
	}
	else if (algoritmo == "TSSB")
	{	
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, UNDEFINED); 
		memoriaUltimasSoluciones = true;
		vecindadPorColoreoEnH = false;
		mayor_impacto = tabuSearch(ordenVertices);
	}
	else if (algoritmo == "TSEB")
	{	
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, UNDEFINED); 
		memoriaUltimasSoluciones = false;
		vecindadPorColoreoEnH = false;
		mayor_impacto = tabuSearch(ordenVertices);
	}
	else if (algoritmo == "TSSAB")
	{	
		vector<Vertice> ordenVertices = vector<Vertice>(cantVertices, UNDEFINED); 
		memoriaUltimasSoluciones = true;
		vecindadPorColoreoEnH = true;
		mayor_impacto = tabuSearch(ordenVertices);
	}
	auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();

	// Imprimimos el tiempo de ejecución por stderr.
	std::clog << total_time << endl;

	bool coloreoValido = !verificadorColoreo(ordenDeColores,true,G);
    std::cout << (!coloreoValido ? UNDEFINED : mayor_impacto) <<endl;
    // Imprimimos el resultado por stdout.
    if(coloreoValido){
    	for(int i = 0 ; i < cantVertices ; i++){
    		std::cout << ordenDeColores[i] << " ";
    	}
		std::cout<<endl;
	}
    else{
		std::cout<<"No hubo coloreo válido"<<endl;
	}
    return 0;
}
/*
codigo para debuggear
	cout << "Grafo H :"<<endl;
	for(int j = 0 ; j < cantVertices ; j++){ //O(mG + mH)
		cout << "vertice :"<< j+1 <<endl; 
		for(long unsigned int k = 0 ; k < H[j].size() ; k++){
			cout<<H[j][k]<<" ";

		}cout<<endl;
	}

	cout << "Grafo G :"<<endl;
	for(int j = 0 ; j < cantVertices ; j++){ cout << "vertice :"<< j+1 <<endl; 
		for(long unsigned int k = 0 ; k < G[j].size() ; k++){ cout<<G[j][k]<<" ";}cout<<endl;
	}
	for(int i = 0; i < cantVertices; i++){cout << ordenDeColores[i] << " ";} cout<<endl;

	for(int i = 0; i < cantVertices; i++){
			cout<< ordenVertices[i] << " ";
	} cout<<endl;
	

//para colorear los restantes de G en la tercer heuristica
// Inicializo el vector que indica los colores usados y sin usar
    vector<bool> coloresSinUsar(cantVertices, false);
	for(int i = 0; i < cantVertices; i++) { // O(n+m)
		// Todos los colores asignados en la iteración anterior se ponen como "en uso" (true)	
		for (long unsigned int j = 0; j < G[i].size(); j++)
			if (ordenDeColores[G[i][j]-1] != UNDEFINED)
				coloresSinUsar[ordenDeColores[G[i][j]-1]] = true;
		// Se utiliza el primer color sin utilizar para colorear el vertice
		while (cr < cantVertices && coloresSinUsar[cr])cr++;
		if(ordenDeColores[i]==UNDEFINED)ordenDeColores[i] = cr;
    }
	/pcmi "M1H" 100 30 1000 1000 < python/instancias/CMI_n16.in ; cat python/instancias/CMI_n16.out 
	./pcmi "G3" < python/instancias/CMI_n12.in ; cat python/instancias/CMI_n12.out 
*/