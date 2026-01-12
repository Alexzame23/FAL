#include <iostream>
using namespace std;
const int MAX_OBJS = 20;

// Lista con los objetos a portar 
typedef struct {
  int tamanios[MAX_OBJS];  // Array en cuyas primeras posiciones se guardan
                           // los tamaños de los objetos
  int n_objetos;           // Numero de objetos
} tObjetos;

/*
=========================================================================
ANÁLISIS DEL PROBLEMA DE VUELTA ATRÁS: MUDANZAS (BIN PACKING)
=========================================================================

DESCRIPCIÓN DEL PROBLEMA:
Dado un conjunto de objetos con tamaños específicos y cajas de capacidad fija,
determinar el MÍNIMO número de cajas necesarias para empaquetar todos los objetos,
donde la suma de tamaños en cada caja no puede exceder su capacidad.

TIPO DE PROBLEMA:
Es un problema de ASIGNACIÓN: para cada objeto debemos decidir en QUÉ CAJA colocarlo.
Es un problema de optimización: queremos MINIMIZAR el número de cajas usadas.


(1) GENERALIZACIÓN - PARÁMETROS Y SU SIGNIFICADO
-------------------------------------------------
La función vuelta_atras_gen necesita los siguientes parámetros:

- const tObjetos& objetos: Estructura con los tamaños de todos los objetos
                           y el número total de objetos
                           
- int capacidad_caja: Capacidad máxima de cada caja (constante C)

- int objeto_actual: Índice del objeto que estamos asignando ahora
                     Hemos asignado objetos 0..objeto_actual-1
                     Ahora debemos asignar objeto_actual
                     Valores: 0, 1, 2, ..., n_objetos
                     
- int cajas[]: Array que representa el estado actual de las cajas
               cajas[i] = espacio USADO en la caja i
               Si cajas[i] = 0 → caja vacía/no usada
               Si cajas[i] > 0 → caja contiene objetos que suman ese tamaño
               
- int num_cajas_usadas: Cuántas cajas estamos usando actualmente
                        Solo contamos cajas NO vacías
                        Este es el valor que queremos MINIMIZAR
                        
- int mejor: El MÍNIMO número de cajas encontrado hasta el momento
             Inicialmente = n_objetos (peor caso: una caja por objeto)
             Se actualiza cuando encontramos soluciones con menos cajas


(2) ¿CÓMO SON LAS SOLUCIONES PARCIALES?
----------------------------------------
Una solución parcial es una ASIGNACIÓN INCOMPLETA de objetos a cajas.
Representa las decisiones tomadas sobre los objetos 0..objeto_actual-1.

Ejemplo con 5 objetos [2, 6, 3, 8, 9] y capacidad 10:
- Inicial: objeto_actual=0, cajas=[0,0,0,0,0], num_cajas=0
  (No hemos asignado ningún objeto)
  
- Parcial: objeto_actual=2, cajas=[8,0,0,0,0], num_cajas=1
  (Objetos 0 y 1 asignados a caja 0: 2+6=8)
  
- Completa: objeto_actual=5, cajas=[10,9,9,0,0], num_cajas=3
  (Todos los objetos asignados: caja0:[2,8], caja1:[6,3], caja2:[9])

Cada solución parcial se caracteriza por:
- Qué objetos hemos asignado (implícito en objeto_actual)
- Estado de cada caja (cuánto espacio usado)
- Número de cajas que estamos usando


(3) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES VIABLE?
--------------------------------------------
Una solución parcial es VIABLE cuando respeta las restricciones del problema
y todavía tiene potencial de mejorar la mejor solución conocida.

Viabilidad al asignar un objeto a una caja:

✓ RESTRICCIÓN FÍSICA: El objeto debe CABER en la caja
  cajas[i] + tamaño_objeto <= capacidad_caja
  
✓ RESTRICCIÓN DE OPTIMIZACIÓN (poda): No usar demasiadas cajas
  num_cajas_usadas < mejor
  Si ya usamos tantas cajas como el mejor encontrado, NO podemos mejorar

Una rama es NO VIABLE y debe PODARSE si:
  - Intentamos poner un objeto que NO cabe en la caja (excede capacidad)
  - Ya usamos >= mejor cajas (no podemos encontrar una solución mejor)


(4) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES FINAL?
-------------------------------------------
Una solución parcial es FINAL cuando TODOS los objetos han sido asignados:

  objeto_actual == objetos.n_objetos

En ese momento, hemos completado una asignación válida.
Es una solución VÁLIDA que debemos considerar si:
  num_cajas_usadas < mejor
  
Si es válida, actualizamos:
  mejor = num_cajas_usadas


(5) GENERACIÓN DE SIGUIENTES SOLUCIONES PARCIALES VIABLES
----------------------------------------------------------
Para cada objeto tenemos múltiples OPCIONES de dónde colocarlo:

ESTRATEGIA: Probar el objeto_actual en diferentes cajas

OPCIÓN A - Poner en una CAJA EXISTENTE:
  Para cada caja i desde 0 hasta (num_cajas_usadas - 1):
    SI cajas[i] + tamaño_objeto <= capacidad_caja:
      // Cabe en esta caja
      cajas[i] += tamaño_objeto         // Poner objeto
      vuelta_atras_gen(..., objeto_actual+1, ..., num_cajas_usadas, ...)
      cajas[i] -= tamaño_objeto         // BACKTRACK: quitar objeto

OPCIÓN B - Poner en una NUEVA caja:
  SI num_cajas_usadas < mejor:  // Solo si podemos mejorar
    cajas[num_cajas_usadas] = tamaño_objeto  // Abrir nueva caja
    vuelta_atras_gen(..., objeto_actual+1, ..., num_cajas_usadas+1, ...)
    cajas[num_cajas_usadas] = 0              // BACKTRACK: vaciar caja

IMPORTANTE - Evitar simetrías:
Las cajas son INDISTINGUIBLES. Para evitar generar la misma solución
múltiples veces (solo con diferentes nombres de cajas), usamos las cajas
EN ORDEN: siempre la primera caja libre es la de índice num_cajas_usadas.

Ejemplo: Si usamos 2 cajas (índices 0 y 1), la nueva caja es la 2.


(6) ANÁLISIS DE CASOS
---------------------

(6.1) CASOS BASE:

BASE 1 - Todos los objetos asignados:
  SI objeto_actual == objetos.n_objetos:
    // Hemos completado una asignación
    SI num_cajas_usadas < mejor:
      mejor = num_cajas_usadas  // ¡Mejor solución encontrada!
    RETORNAR mejor

BASE 2 - Poda por optimización:
  SI num_cajas_usadas >= mejor:
    RETORNAR mejor  // Ya usamos demasiadas cajas, no podemos mejorar


(6.2) CASOS RECURSIVOS:

Para objeto_actual, explorar todas las asignaciones posibles:

CASO A - Probar en cajas EXISTENTES:
  PARA i desde 0 hasta num_cajas_usadas-1:
    SI cajas[i] + tamaños[objeto_actual] <= capacidad_caja:
      // El objeto CABE en la caja i
      cajas[i] += tamaños[objeto_actual]    // Asignar
      mejor = vuelta_atras_gen(..., objeto_actual+1, ..., num_cajas_usadas, mejor)
      cajas[i] -= tamaños[objeto_actual]    // Backtrack

CASO B - Probar en NUEVA caja:
  SI num_cajas_usadas + 1 < mejor:
    // Abrir nueva caja solo si podemos mejorar
    cajas[num_cajas_usadas] = tamaños[objeto_actual]
    mejor = vuelta_atras_gen(..., objeto_actual+1, ..., num_cajas_usadas+1, mejor)
    cajas[num_cajas_usadas] = 0           // Backtrack

RETORNAR mejor


(7) INMERSIÓN - DEFINICIÓN DEL ALGORITMO FINAL
-----------------------------------------------
La función min_numero_de_cajas es el punto de entrada (inmersión).
Inicializa el estado y llama a la generalización:

Inicialización:
- objeto_actual = 0 (empezamos con el primer objeto)
- cajas[] = [0, 0, ..., 0] (todas las cajas vacías)
- num_cajas_usadas = 0 (no estamos usando ninguna caja)
- mejor = n_objetos (peor caso: una caja por objeto)

Casos especiales:
- Si n_objetos == 0 → retornar 0 (no necesitamos cajas)

Retorna el resultado de la generalización.


OPTIMIZACIONES IMPLEMENTADAS:
------------------------------
1. PODA POR OPTIMIZACIÓN: Si num_cajas_usadas >= mejor, detener exploración
2. EVITAR SIMETRÍAS: Usar cajas en orden secuencial (0, 1, 2, ...)
3. EXPLORAR CAJAS EXISTENTES PRIMERO: Antes de abrir nueva caja
4. BACKTRACKING EXPLÍCITO: Restaurar estado para probar otras opciones


COMPLEJIDAD:
------------
En el peor caso: O(K^N) donde K es el número de cajas y N el número de objetos
Las podas reducen significativamente el espacio de búsqueda en la práctica.

=========================================================================
*/

// Función generalizada que implementa la vuelta atrás
int vuelta_atras_gen(const tObjetos& objetos, int capacidad_caja,
                     int objeto_actual, int cajas[], int num_cajas_usadas,
                     int mejor) {
    
    // CASO BASE 1: Todos los objetos han sido asignados
    if (objeto_actual == objetos.n_objetos) {
        // Hemos completado una asignación válida
        // Actualizamos el mejor si esta solución usa menos cajas
        if (num_cajas_usadas < mejor) {
            mejor = num_cajas_usadas;
        }
        return mejor;
    }
    
    // CASO BASE 2: Poda por optimización
    // Si ya estamos usando tantas cajas como el mejor encontrado,
    // no podemos mejorar, así que podamos esta rama
    if (num_cajas_usadas >= mejor) {
        return mejor;
    }
    
    // Obtener el tamaño del objeto actual a asignar
    int tamaño_objeto = objetos.tamanios[objeto_actual];
    
    // CASO RECURSIVO: Explorar todas las opciones de asignación
    
    // OPCIÓN A: Intentar poner el objeto en cada CAJA EXISTENTE
    // Probamos las cajas en uso (de 0 a num_cajas_usadas-1)
    for (int i = 0; i < num_cajas_usadas; i++) {
        // Verificar si el objeto CABE en esta caja
        if (cajas[i] + tamaño_objeto <= capacidad_caja) {
            // SÍ cabe, asignar objeto a esta caja
            cajas[i] += tamaño_objeto;
            
            // Recursión: asignar el siguiente objeto
            mejor = vuelta_atras_gen(objetos, capacidad_caja,
                                    objeto_actual + 1, cajas,
                                    num_cajas_usadas, mejor);
            
            // BACKTRACKING: quitar el objeto de esta caja
            // para probar otras asignaciones
            cajas[i] -= tamaño_objeto;
        }
    }
    
    // OPCIÓN B: Intentar poner el objeto en una NUEVA caja
    // Solo intentamos abrir una nueva caja si hay potencial de mejorar
    // Es decir, si usar (num_cajas_usadas + 1) cajas aún es mejor que "mejor"
    if (num_cajas_usadas + 1 < mejor) {
        // Abrir nueva caja (la de índice num_cajas_usadas)
        cajas[num_cajas_usadas] = tamaño_objeto;
        
        // Recursión: asignar el siguiente objeto
        // Ahora usamos una caja más
        mejor = vuelta_atras_gen(objetos, capacidad_caja,
                                objeto_actual + 1, cajas,
                                num_cajas_usadas + 1, mejor);
        
        // BACKTRACKING: vaciar la nueva caja
        cajas[num_cajas_usadas] = 0;
    }
    
    return mejor;
}

// PUNTO DE ENTRADA DEL ALGORITMO (Inmersión)
int min_numero_de_cajas(const tObjetos& objetos, int capacidad_caja) {
    // Caso especial: sin objetos no necesitamos cajas
    if (objetos.n_objetos == 0) {
        return 0;
    }
    
    // Inicializar array de cajas (todas vacías al principio)
    int cajas[MAX_OBJS];
    for (int i = 0; i < MAX_OBJS; i++) {
        cajas[i] = 0;
    }
    
    // Llamar a la función generalizada con estado inicial:
    // - objeto_actual = 0 (empezamos con el primer objeto)
    // - cajas[] = todas en 0 (todas vacías)
    // - num_cajas_usadas = 0 (no usamos ninguna caja aún)
    // - mejor = n_objetos (peor caso: una caja por objeto)
    //
    // El peor caso ocurriría si cada objeto necesitara su propia caja,
    // lo cual es imposible a menos que cada objeto tenga tamaño = capacidad
    return vuelta_atras_gen(objetos, capacidad_caja, 0, cajas, 0, 
                           objetos.n_objetos);
}

/* CODIGO PARA LEER Y EJECUTAR LOS CASOS DE PRUEBA */
bool ejecuta_caso() {
  int capacidad;
  cin >> capacidad;
  if (capacidad != -1) {
    tObjetos objetos;
    cin >> objetos.n_objetos;
    for (int i = 0; i < objetos.n_objetos; i++) {
      cin >> objetos.tamanios[i];
    }
    cout << min_numero_de_cajas(objetos,capacidad) << endl;
    return true;
  }
  else {
    return false;
  }
}

int main() {
  while (ejecuta_caso());
}
