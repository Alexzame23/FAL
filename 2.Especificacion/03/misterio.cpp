#include <iostream>

using namespace std;

const int TMAX = 1000000;  // No habr� vectores de m�s de un mill�n de elementos


/*

PRECONDICION DE LA FUNCION: 
  ---Escribe aqu� la precondici�n de la funci�n.
  P: {n > 0 && (PARATODO i: 0 ≤ i < n-1: a[i] ≤ a[i+1]) }
  

*/

 
int misterio(const int a[], int n) {
    /* IMPLEMENTACION: COMPLETA EL CUERPO DE LA FUNCION
       CON EL CODIGO QUE IMPLEMENTA EL ALGORITMO */
   int result = a[0];
   int rep = 1;
   int aux = 1;
   for(int i = 1; i<n; ++i){
      if(a[i-1] == a[i]){
         ++aux;
      }
      else{
         if(rep < aux){
            rep = aux;
            result = a[i-1];
         }
         aux = 1;
      }
   }
   return (rep < aux) ? a[n-1] : result;
} 

/* COMPLEJIDAD:
   -- Determina justificadamente el orden de complejidad en el peor caso 
   -- de este algoritmo, indicando claramente cu�l es el tama�o del problema
   El algoritmo se recorre de manera completa en funcion de n, ya que el bucle for siempre recorrera la n entera ya que no
   podemos terminarlo antes ya que el ultimo numero puede ser el mas repetido. Como las operaciones de dentro del bucle son
   de tiempo constante la complejidad final del algoritmo en cualquiera de los casos es de O(n)   
*/


/* CODIGO PARA LEER Y EJECUTAR LOS CASOS DE PRUEBA. NO MODIFICAR */

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
		   cout << misterio(a, n) << endl;
	   }
   }
   while (n != -1);
}

