import subprocess
import os
from os import remove
from random import randrange
import sys


###Este script calcula el gap para cada heuristica dadas las instancias provistas por la catedra


def gap(alg, t,p,l1,l2):
	files = ['python/instancias/instancias-calidad/CMI_n'+str(i)+'.in ; cat python/instancias/instancias-calidad/CMI_n'+str(i)+'.out' for i in [6,8,10,12,14,16,18,20,22,24,26,28,30]]
	optimos = []
	obtenidos = []
	if alg[0] == 'T':
		gaps = [0 for i in range(13)]
		repeticiones = 30
		command = './pcmi ' + '"' + alg + '"' + ' ' + str(t) + ' ' + str(p) + ' ' + str(l1) + ' ' + str(l2)
		for i in range(13):
			f = files[i]
			for r in range(repeticiones):
				res = (subprocess.getstatusoutput('cd .. && ' + command + ' < ' + f))[1].splitlines()
				optimos.append(int(res[3]))
				obtenidos.append(int(res[1]))
				gap = (int(res[3]) - int(res[1]))*100/int(res[3])
				gaps[i] += gap/repeticiones
	else:
		gaps = []
		command = './pcmi ' + '"' + alg + '"'
		for f in files:
			res = (subprocess.getstatusoutput('cd .. && ' + command + ' < ' + f))[1].splitlines()
			optimos.append(int(res[3]))
			obtenidos.append(int(res[1]))
			gap = (int(res[3]) - int(res[1]))*100/int(res[3])
			gaps.append(gap)

	return gaps

if __name__ == '__main__':
	algoritmos = ['HS1','HS1B','HS2','HS2B','HGA','HGV','TSS','TSE','TSSA','TSSB','TSEB','TSSAB']
	alg = str(sys.argv[1])
	if alg not in algoritmos:
		print('Se introdujo un algoritmo inexistente')
	else:
		if alg[0] == 'T':
			try:
				t = sys.argv[2]
				p = sys.argv[3]
				l1 = sys.argv[4]
				l2 = sys.argv[5]
			except:
				print('Argumentos faltantes para la ejecución')
		else:
			t = p = l1 = l2 = 0

		gaps = gap(alg,t,p,l1,l2)
		print('Gap del algoritmo, en porcentajes, para cada instancia:')
		print(gaps)
		print('\n Promedio de gap, en porcentaje:')
		print(sum(gaps)/float(len(gaps)))
		
	

