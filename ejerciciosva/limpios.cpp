#include <iostream>
#include <string>
using namespace std;
typedef unsigned long long t_num;

/*
=========================================================================
ANÁLISIS DEL PROBLEMA DE VUELTA ATRÁS: NÚMEROS LIMPIOS
=========================================================================

(1) GENERALIZACIÓN - PARÁMETROS Y SU SIGNIFICADO
-------------------------------------------------
Necesitamos una función generalizada porque vamos construyendo el número
DÍGITO POR DÍGITO de izquierda a derecha, manteniendo información de estado.

PARÁMETROS de vuelta_atras_gen(...):

- string n_str: El número límite n convertido a string para acceder fácilmente
                a cada dígito por posición

- unsigned short pos: Posición actual donde estamos construyendo el número
                      (0 = primer dígito, 1 = segundo, etc.)

- unsigned int suma_actual: Suma acumulada de los dígitos que llevamos hasta ahora
                            Sirve para verificar si podemos alcanzar s o ya nos pasamos

- unsigned short consecutivos: Cuántos dígitos 'd' llevamos AL FINAL del número parcial
                               Ejemplo: si vamos "1553" y d=5, consecutivos=0 (el último es 3)
                               Si vamos "1555" y d=5, consecutivos=3

- bool es_menor: ¿Ya sabemos con certeza que nuestro número es < n?
                 TRUE si en alguna posición anterior pusimos un dígito menor
                 FALSE si todavía vamos "empatados" con n

- unsigned short d: El dígito "prohibido" en secuencias largas
- unsigned short alpha: Longitud máxima de secuencias de 'd'
- unsigned int s: La suma objetivo


(2) ¿CÓMO SON LAS SOLUCIONES PARCIALES?
----------------------------------------
Una solución parcial es un NÚMERO INCOMPLETO que estamos construyendo.

Ejemplo: Si n=4567, una solución parcial podría ser:
- "3" (solo el primer dígito)
- "34" (dos dígitos)
- "345" (tres dígitos)

Cada solución parcial se caracteriza por:
- Los dígitos que llevamos (implícito en 'pos' y decisiones previas)
- La suma acumulada
- Cuántos 'd's consecutivos hay al final
- Si ya es menor que n o aún puede igualarlo


(3) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES VIABLE?
--------------------------------------------
Una solución parcial es VIABLE si todavía es posible que conduzca a una
solución válida. Se debe cumplir:

✓ suma_actual <= s  (si ya nos pasamos de s, imposible alcanzarlo exacto)
✓ No violamos la restricción de d-alpha-limpio
  (no podemos tener alpha o más 'd's seguidos)
✓ No nos hemos pasado de n (esto lo controla 'es_menor' y el límite en cada pos)


(4) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES FINAL?
-------------------------------------------
Una solución parcial es FINAL cuando:

pos == n_str.length()  (hemos construido un número completo)

Y es una solución VÁLIDA si además:
suma_actual == s  (la suma es exactamente la pedida)

Si suma_actual != s, es final pero NO válida (no la contamos).


(5) ¿CÓMO SE GENERAN LAS SIGUIENTES SOLUCIONES PARCIALES VIABLES?
------------------------------------------------------------------
Desde una solución parcial, generamos nuevas soluciones probando TODOS los
dígitos del 1 al 9 en la posición actual.

Para cada dígito 'dig' del 1 al 9:

  PODA 1 (Restricción de n):
    Si !es_menor Y dig > n_str[pos]:
      DESCARTAR (nos pasaríamos de n)

  PODA 2 (Restricción d-alpha-limpio):
    Si dig == d Y consecutivos == alpha-1:
      DESCARTAR (tendríamos alpha 'd's consecutivos)

  Si pasa las podas:
    Calcular nuevo estado:
      - nueva_suma = suma_actual + dig
      - nuevos_consecutivos = (dig == d) ? consecutivos+1 : 0
      - nuevo_es_menor = es_menor O (dig < n_str[pos])

    Llamar recursivamente con el nuevo estado


(6) ANÁLISIS DE CASOS
---------------------

(6.1) CASOS BASE:

BASE 1 - Número completo:
  SI pos == n_str.length():
    SI suma_actual == s:
      RETORNAR 1  (solución válida encontrada)
    SINO:
      RETORNAR 0  (número completo pero suma incorrecta)

BASE 2 - Poda por suma excedida:
  SI suma_actual > s:
    RETORNAR 0  (ya no podemos alcanzar exactamente s)


(6.2) CASOS RECURSIVOS:

Para cada dígito dig del 1 al 9:
  Si es viable (pasa las restricciones):
    Acumular las soluciones del subproblema:
    total += vuelta_atras_gen(n_str, pos+1, nueva_suma,
                              nuevos_consec, nuevo_es_menor,
                              d, alpha, s)

RETORNAR total


(7) INMERSIÓN - DEFINICIÓN DEL ALGORITMO FINAL
-----------------------------------------------
El algoritmo final num_limpios es una INMERSIÓN de la generalización.

Inicializa el estado para comenzar la búsqueda:
- pos = 0 (comenzamos en el primer dígito)
- suma_actual = 0 (no hemos sumado nada aún)
- consecutivos = 0 (no hemos puesto ningún 'd' aún)
- es_menor = false (aún no sabemos si somos menores que n)

Y convierte n a string para facilitar el acceso a cada dígito.

=========================================================================
*/

// Función generalizada que resuelve el problema mediante vuelta atrás
int vuelta_atras_gen(const string& n_str, unsigned short pos,
    unsigned int suma_actual, unsigned short consecutivos,
    bool es_menor, unsigned short d, unsigned short alpha,
    unsigned int s) {

    // CASO BASE 1: Hemos construido un número completo
    if (pos == n_str.length()) {
        // Solo cuenta si la suma es exactamente s
        return (suma_actual == s) ? 1 : 0;
    }

    // CASO BASE 2: Poda por suma excedida
    if (suma_actual > s) {
        return 0; // Ya nos pasamos, no puede ser solución
    }

    int total = 0;

    // CASO RECURSIVO: Probar cada dígito del 1 al 9
    for (unsigned short dig = 1; dig <= 9; dig++) {

        // PODA 1: Restricción de no exceder n
        // Si aún no somos menores que n, solo podemos poner dígitos
        // hasta el que tiene n en esta posición
        if (!es_menor && dig > (n_str[pos] - '0')) {
            continue; // Nos pasaríamos de n
        }

        // PODA 2: Restricción d-alpha-limpio
        // No podemos poner 'd' si ya tenemos (alpha-1) 'd's consecutivos
        if (dig == d && consecutivos == alpha - 1) {
            continue; // Tendríamos alpha 'd's consecutivos
        }

        // Calcular el nuevo estado después de poner 'dig'
        unsigned int nueva_suma = suma_actual + dig;

        // Si ponemos 'd', incrementamos contador; si no, reiniciamos a 0
        unsigned short nuevos_consecutivos = (dig == d) ? consecutivos + 1 : 0;

        // Si ponemos un dígito menor que el de n, ya somos definitivamente menores
        bool nuevo_es_menor = es_menor || (dig < (n_str[pos] - '0'));

        // Llamada recursiva
        total += vuelta_atras_gen(n_str, pos + 1, nueva_suma,
            nuevos_consecutivos, nuevo_es_menor,
            d, alpha, s);
    }

    return total;
}

// PUNTO DE ENTRADA DEL ALGORITMO (Inmersión)
int num_limpios(unsigned short d, unsigned short alpha, t_num n, unsigned int s) {
    // Convertir n a string para acceder fácilmente a cada dígito
    string n_str = to_string(n);

    // Llamar a la generalización con estado inicial:
    // - pos = 0 (empezamos en el primer dígito)
    // - suma_actual = 0 (no hemos sumado nada)
    // - consecutivos = 0 (no hay 'd's consecutivos al principio)
    // - es_menor = false (aún no sabemos si somos menores que n)
    return vuelta_atras_gen(n_str, 0, 0, 0, false, d, alpha, s);
}

bool ejecuta_caso() {
    int d;
    cin >> d;
    if (d == -1) {
        return false;
    }
    else {
        unsigned short alpha;
        unsigned int s;
        t_num n;
        cin >> alpha >> s >> n;
        cout << num_limpios(d, alpha, n, s) << endl;
        return true;
    }
}

int main() {
    while (ejecuta_caso());
}
