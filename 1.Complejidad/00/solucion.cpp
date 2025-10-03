#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;



bool esta_ordenado(int a[], int n) {
	bool resul = true;
	int i = 1;
	while (resul && i < n) {
		resul = (a[i] >= a[i - 1]);
		i++;
	}
	return resul;
}

// Soluci�n cuadr�tica

bool esta_ordenado_naif(int a[], int n) {
	bool resul = true;
	int i = 0;
	while (resul && i < n) {
		int j = i + 1;
		while (resul && j < n) {
			resul = (a[i] <= a[j]);
			j++;
		}
		i++;
	}
	return resul;
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
		if (esta_ordenado(a, n)) cout << "SI" << endl;
		else cout << "NO" << endl;
		return true;
	}
}

int main() {
	while (ejecuta_caso());
}

