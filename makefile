all:
	g++ -ggdb -Wall -Wno-unused-parameter -Wextra -no-pie -pedantic -m64 -O3 -march=native -c greedyHeuristics.cpp -std=c++14
	g++ -ggdb -Wall -Wno-unused-parameter -Wextra -no-pie -pedantic -m64 -O3 -march=native -c tabu_search.cpp -std=c++14
	g++ -ggdb -Wall -Wno-unused-parameter -Wextra -no-pie -pedantic -m64 -O3 -march=native -c definesAndGlobals.cpp -std=c++14
	g++ -ggdb -Wall -Wno-unused-parameter -Wextra -no-pie -pedantic -m64 -O3 -march=native -c pcmi.cpp -std=c++14
	g++ -ggdb -Wall -Wno-unused-parameter -Wextra -no-pie -pedantic -m64 -O3 -march=native *.o -o pcmi -std=c++14
	

clean:
	rm -f pcmi
	rm -f *.o