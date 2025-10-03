/*
N� GRUPO:
NOMBRES Y APELLIDOS:


*/


#include <iostream>

using namespace std;


//n siempre ser� positivo (es decir, pod�is asumir n >= 1)
int diametro(int a[], int n) {
	 // A IMPLEMENTAR
}


/* CODIGO PARA LEER Y EJECUTAR LOS CASOS DE PRUEBA */

const int TMAX = 1000000;  // No se podr�n leer vectores de m�s de un mill�n de elementos

bool ejecuta_caso() {
	static int a[TMAX];
	int n;
	cin >> n;
	if (n == -1 || n > TMAX) {
		return false;
	}
	else {
		for (int i = 0; i < n; i++) {
			cin >> a[i];
		}
		cout << diametro(a, n) << endl;
		return true;
	}
}

int main() {
	while (ejecuta_caso());
}