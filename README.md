# Problema de Coloreo de Máximo Impacto
El código que resuelve el problema del enunciado está en el archivo *pcmi.cpp*.
Además tenemos las funciones y defines globales en el archivo *definesAndGlobals.cpp*.
Mientras que las funciones utilizadas para las heuristicas de construcción golosa, se encuentran en *greedyHeuristics.cpp*.
Finalmente, las funciones utilizadas dentro de la metaheuristica _Tabú Search_ se encuentran en el archivo *tabu_search.cpp*.

Para compilarlo se debe ejecutar el comando
    *make all*
en la consola.

Al ejecutarlo se le debe pasar un parámetro que indique cuál algoritmo se desea utilizar entre las siguientes opciones:
	-[algoritmo] [params Tabú  -tamanoMemoria -porcentajeSubVecindad -IteracionesSinMejora -Iteraciones]

Además se debe pasar la instancia por entrada estándar. Por ejemplo, para la heuristica de Tabú Search con memoria por soluciones y vecindad de soluciones generadas por aplicaciones de swap y change:
	cat python/instancias/INST1.in | ./pcmi "TSS" 100 30 1000 1000
La salida del programa es doble. Por la salida estándar se devuelve el impacto pedido por el enunciado junto con el coloreo correspondiente a la solución. Por la STDERR se devuelve el tiempo de ejecución del algoritmo.

La generación de instancias y corrida de experimentos de manera secuencia está programada en Python utilizando Jupyter Lab (https://jupyter.org/). Para ver los notebooks se debe instalar Jupyter Labs siguiendo las instrucciones en su web. Se recomienda seguir los pasos de instalación utilizando pip3.

### Esto de abajo se encuentra sujeto a modificaciones
Una vez instalado Jupyter Lab, abrir una consola en la carpeta python y correr el comando:
	jupyter lab

Allí, existen tres archivos:
	- generar-instancias.ipynb: genera las instancias descriptas en el trabajo y las ubica en la carpeta python/instancias.
	- experimentos.ipynb: corre los experimentos del trabajo y deja el resultado de todas las ejecuciones en un CSV en la carpeta python/resultados
	- analysis.ipynb: toma los resultados de las ejecuciones y genera todos los gráficos correspondientes.
