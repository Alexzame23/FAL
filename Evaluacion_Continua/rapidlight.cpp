/*
NOMBRES Y APELLIDOS DE LOS MIEMBROS DEL GRUPO QUE HAN REALIZADO LA PRACTICA:


Si alguno de los miembros del grupo no ha realizado
la práctica, indicarlo aquí, junto con el motivo:


*/

#include <iostream>

using namespace std;


/*
(1) GENERALIZACION (EN CASO DE QUE PROCEDA): Entradas y salidas

(2) DISEÑO

(3) IMPLEMENTACION POR INMERSION DEL ALGORITMO, SI PROCEDE:

(4) DETERMINACION DEL COSTE DEL ALGORITMO EN EL PEOR CASO:

*/


int min_capacidad(int etapas[], int n, int repostajes_permitidos) {
	 // A IMPLEMENTAR
}


const int MAX_ETAPAS = 100000;


bool ejecuta_caso() {
	static int v[MAX_ETAPAS];
	int n, c;
	cin >> n;
	if (n == -1) return false;
	else {
		cin >> c;
		for (int i = 0; i < n; ++i)
			cin >> v[i];
		cout << min_capacidad(v, n, c) << endl;
		return true;
	}
}

int main() {
	while (ejecuta_caso());
	return 0;
}

