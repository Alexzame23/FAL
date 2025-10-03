/*
N� DE GRUPO: G31
NOMBRE Y APELLIDOS DE LOS ESTUDIANTES QUE
HAN REALIZADO LA PRACTICA: Alejandro Zamorano Méndez

EN CASO DE QUE ALGUNO DE LOS MIEMBROS DEL
GRUPO NO HAYA PARTICIPADO EN LA PRACTICA,
INDICARLO AQUI, EXPLICANDO TAMBI�N EL MOTIVO:



*/

#include <iostream>

using namespace std;

const int TMAX = 1000000;  // No habr� vectores de m�s de un mill�n de elementos


/*

PRECONDICION DE LA FUNCION: 
  ---Escribe aqu� la precondici�n de la funci�n.
  P: {n >= 0 && n < tamaño de a[]}

*/
bool divertida(const int a[], int n); 

/*
POSTCONDICION DE LA FUNCION: 
 
  Q: { resul = (  (PARATODO i:0<=i<n:(SUMA j:0<=j<=i:a[j])>=0) && ( (SUMA i:0<=i<n:a[i])=0 ) ) }
   
 */ 
 
 bool divertida(const int a[], int n) {
    /* IMPLEMENTACION: COMPLETA EL CUERPO DE LA FUNCION
       CON EL CODIGO QUE IMPLEMENTA EL ALGORITMO */
    if(n > 0){
        int acumulada = a[0];
        int i = 1;
        while (i < n && acumulada >= 0){
            acumulada += a[i];
            ++i;
        }
        return (i == n && acumulada == 0) ? true : false;	
    }
    else{
        return true;
    }

} 

/* COMPLEJIDAD:
   -- Determina justificadamente el orden de complejidad en el peor caso 
   -- de este algoritmo, indicando claramente cu�l es el tama�o del problema

   La complejidad del problema en el peor de los casos es de O(n), este se da cuando la primera parte de la postcondicion se cumple ((PARATODO i:0<=i<n:(SUMA j:0<=j<=i:a[j])>=0)), ya que es la condicion que 
   debera cumplir el bucle while, por tanto seria recorrer todas las posiciones del array que n indique.  Para el tamaño del problema es n, ya que representa el numero de elementos del array que se deben
   procesar
   
*/

/* CODIGO PARA LEER Y EJECUTAR LOS CASOS DE PRUEBA. NO MODIFICAR! */

void lee_vector(int a[], int & n) {
	cin >> n; 
   for (int i=0; i < n; i++) {
	   cin >> a[i]; 
   }   
}

int main() {
   static int a[TMAX];
   int n;
   do {
	   lee_vector(a,n); 
	   if (n>=0) {
		 if (divertida(a,n)) 
           cout << "SI" << endl;
         else  
           cout << "NO" << endl;			 
	   }
   }
   while (n != -1);
}

