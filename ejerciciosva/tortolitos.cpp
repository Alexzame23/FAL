#include <iostream>
using namespace std;
const unsigned int MAX_CLIENTES = 10; // maximo numero de clientes

// Representación de la matriz de afinidades
typedef struct {
    unsigned int n_clientes;                              // numero de clientes. 
    unsigned int afinidades[MAX_CLIENTES][MAX_CLIENTES];  // la matriz se almacena en las 'n_clientes' primeras filas,
    // 'n_clientes' primeras columnas                                                        
} tMatrizAfinidad;

/*
=========================================================================
ANÁLISIS DEL PROBLEMA DE VUELTA ATRÁS: TORTOLITOS
=========================================================================

(1) GENERALIZACIÓN - PARÁMETROS Y SU SIGNIFICADO
-------------------------------------------------
Necesitamos una función generalizada porque construimos el emparejamiento
PROGRESIVAMENTE, cliente por cliente, manteniendo información sobre quién
ya está emparejado y la afinidad acumulada.

PARÁMETROS de vuelta_atras_gen(...):

- bool emparejado[]: Array que indica si cada cliente ya tiene pareja
                     emparejado[i] = true  → cliente i ya emparejado
                     emparejado[i] = false → cliente i disponible

- unsigned int afinidad_actual: Suma acumulada de las afinidades de las
                                parejas formadas hasta el momento

- const tMatrizAfinidad& as: Matriz de afinidades (constante, no cambia)
                             Contiene: as.n_clientes y as.afinidades[][]

- int mejor: La MEJOR afinidad encontrada hasta ahora en la exploración
             Se va actualizando conforme encontramos mejores soluciones


(2) ¿CÓMO SON LAS SOLUCIONES PARCIALES?
----------------------------------------
Una solución parcial es un EMPAREJAMIENTO INCOMPLETO donde algunos clientes
ya tienen pareja y otros aún no.

Ejemplo con 4 clientes:
- Estado inicial: nadie emparejado [F, F, F, F]
- Parcial 1: clientes 0 y 1 emparejados [T, T, F, F]
- Parcial 2: todos emparejados [T, T, T, T] ← solución completa

Cada solución parcial se caracteriza por:
- El conjunto de clientes ya emparejados (array emparejado[])
- Las parejas formadas (implícito en el camino de decisiones)
- La afinidad acumulada de las parejas formadas


(3) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES VIABLE?
--------------------------------------------
Una solución parcial es VIABLE cuando TODAVÍA ES POSIBLE emparejar a todos
los clientes restantes respetando las restricciones.

Viabilidad local al formar una pareja {i, j}:

✓ Ambos clientes deben estar DISPONIBLES (sin pareja aún)
  emparejado[i] == false Y emparejado[j] == false

✓ Debe haber COMPATIBILIDAD MUTUA:
  as.afinidades[i][j] > 0  (i siente afinidad por j)
  Y
  as.afinidades[j][i] > 0  (j siente afinidad por i)

Poda adicional (optimización):
  Si el primer cliente sin emparejar NO tiene ningún otro cliente compatible
  disponible, la rama NO es viable (no podemos completar el emparejamiento).


(4) ¿CUÁNDO UNA SOLUCIÓN PARCIAL ES FINAL?
-------------------------------------------
Una solución parcial es FINAL cuando TODOS los clientes tienen pareja.

Esto se detecta cuando:
  buscar_primer_sin_emparejar() retorna -1

Es decir, no queda ningún cliente sin emparejar.

En ese momento:
  - Tenemos un emparejamiento completo y válido
  - Actualizamos el MÁXIMO si afinidad_actual > mejor
  - Es una SOLUCIÓN VÁLIDA al problema


(5) ¿CÓMO SE GENERAN LAS SIGUIENTES SOLUCIONES PARCIALES VIABLES?
------------------------------------------------------------------
Desde una solución parcial, generamos nuevas soluciones siguiendo esta
estrategia SISTEMÁTICA:

PASO 1: Encontrar el primer cliente SIN emparejar
  cliente_actual = buscar_primer_sin_emparejar(emparejado, n)

  Estrategia: Siempre tomamos el cliente con MENOR ÍNDICE disponible
  Ventaja: Evitamos generar duplicados del mismo emparejamiento

PASO 2: Probar emparejarlo con CADA cliente compatible disponible
  Para cada otro_cliente desde (cliente_actual + 1) hasta (n - 1):

  ¿Por qué desde (cliente_actual + 1)?
  - Para evitar duplicados
  - Si cliente_actual=0, probamos 0-1, 0-2, 0-3
  - Nunca probaremos 1-0 después porque ya se consideró como 0-1

PASO 3: Para cada pareja {cliente_actual, otro_cliente} VIABLE:

  a) Verificar que otro_cliente esté disponible
  b) Verificar compatibilidad mutua (afinidades > 0)
  c) Si pasa las verificaciones:
     - Calcular afinidad_pareja = A[i][j] + A[j][i]
     - Marcar ambos como emparejados
     - Llamar recursivamente con nueva afinidad acumulada
     - BACKTRACK: desmarcar ambos para probar otras opciones


(6) ANÁLISIS DE CASOS
---------------------

(6.1) CASOS BASE:

BASE 1 - Emparejamiento completo:
  SI no hay clientes sin emparejar (todos tienen pareja):
    mejor = max(mejor, afinidad_actual)
    RETORNAR mejor

  Este es el caso exitoso: hemos formado un emparejamiento válido completo.

BASE 2 - No hay más opciones viables:
  SI el cliente_actual no puede emparejarse con nadie disponible:
    RETORNAR mejor (sin modificar)

  Esto ocurre cuando el bucle termina sin encontrar parejas viables.


(6.2) CASOS RECURSIVOS:

Para cada otro_cliente desde (cliente_actual + 1) hasta (n_clientes - 1):

  SI emparejado[otro_cliente] == false Y
     afinidades[cliente_actual][otro_cliente] > 0 Y
     afinidades[otro_cliente][cliente_actual] > 0:

    // Pareja viable encontrada
    afinidad_pareja = afinidades[cliente_actual][otro_cliente] +
                      afinidades[otro_cliente][cliente_actual]

    // Marcar como emparejados
    emparejado[cliente_actual] = true
    emparejado[otro_cliente] = true

    // RECURSIÓN: Continuar emparejando al resto
    mejor = vuelta_atras_gen(emparejado,
                            afinidad_actual + afinidad_pareja,
                            as, mejor)

    // BACKTRACKING: Deshacer para probar otras opciones
    emparejado[cliente_actual] = false
    emparejado[otro_cliente] = false

RETORNAR mejor


(7) INMERSIÓN - DEFINICIÓN DEL ALGORITMO FINAL
-----------------------------------------------
El algoritmo final maxima_afinidad es una INMERSIÓN de la generalización.

Inicializa el estado para comenzar la búsqueda:

1. Crear array emparejado[] con todos en false (nadie tiene pareja)
2. afinidad_actual = 0 (no hemos formado ninguna pareja)
3. mejor = 0 (peor caso: no hay emparejamientos factibles)

Casos especiales:
- Si n_clientes es 0 → retornar 0
- Si n_clientes es impar → retornar 0 (imposible emparejar a todos)

Luego llama a la generalización y retorna el mejor valor encontrado.

=========================================================================
*/

// Función auxiliar: Busca el primer cliente que aún no tiene pareja
// Retorna: índice del primer cliente sin emparejar, o -1 si todos tienen pareja
int buscar_primer_sin_emparejar(const bool emparejado[], unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (!emparejado[i]) {
            return i;  // Primer cliente disponible
        }
    }
    return -1;  // Todos tienen pareja
}

// Función generalizada que resuelve el problema mediante vuelta atrás
// Retorna: la mejor afinidad encontrada explorando desde el estado actual
int vuelta_atras_gen(bool emparejado[], unsigned int afinidad_actual,
    const tMatrizAfinidad& as, int mejor) {

    // PASO 1: Buscar el primer cliente sin emparejar
    int cliente_actual = buscar_primer_sin_emparejar(emparejado, as.n_clientes);

    // CASO BASE: Todos los clientes tienen pareja
    if (cliente_actual == -1) {
        // Hemos completado un emparejamiento válido
        // Actualizamos el mejor si este es superior
        if ((int)afinidad_actual > mejor) {
            mejor = afinidad_actual;
        }
        return mejor;
    }

    // CASO RECURSIVO: Probar emparejar cliente_actual con cada disponible
    // Solo consideramos clientes POSTERIORES para evitar duplicados
    for (unsigned int otro_cliente = cliente_actual + 1;
        otro_cliente < as.n_clientes;
        otro_cliente++) {

        // RESTRICCIÓN 1: El otro cliente debe estar disponible
        if (emparejado[otro_cliente]) {
            continue;  // Ya tiene pareja, probar siguiente
        }

        // RESTRICCIÓN 2: Debe haber compatibilidad MUTUA
        // cliente_actual debe sentir afinidad por otro_cliente
        if (as.afinidades[cliente_actual][otro_cliente] == 0) {
            continue;  // No son compatibles
        }

        // otro_cliente debe sentir afinidad por cliente_actual
        if (as.afinidades[otro_cliente][cliente_actual] == 0) {
            continue;  // No son compatibles
        }

        // PAREJA VIABLE: Podemos emparejar cliente_actual con otro_cliente

        // Calcular la afinidad de esta pareja
        unsigned int afinidad_pareja =
            as.afinidades[cliente_actual][otro_cliente] +
            as.afinidades[otro_cliente][cliente_actual];

        // Marcar ambos clientes como emparejados
        emparejado[cliente_actual] = true;
        emparejado[otro_cliente] = true;

        // RECURSIÓN: Continuar emparejando a los clientes restantes
        mejor = vuelta_atras_gen(emparejado,
            afinidad_actual + afinidad_pareja,
            as, mejor);

        // BACKTRACKING: Deshacer el emparejamiento para probar otras opciones
        emparejado[cliente_actual] = false;
        emparejado[otro_cliente] = false;
    }

    // Retornar el mejor valor encontrado
    return mejor;
}

// PUNTO DE ENTRADA DEL ALGORITMO (Inmersión)
int maxima_afinidad(const tMatrizAfinidad& as) {
    // Caso especial: número impar de clientes
    // No es posible emparejar a todos (cada pareja necesita 2 personas)
    if (as.n_clientes % 2 != 0) {
        return 0;
    }

    // Caso especial: 0 clientes
    if (as.n_clientes == 0) {
        return 0;
    }

    // Inicializar el array de emparejados
    // Al inicio, ningún cliente tiene pareja
    bool emparejado[MAX_CLIENTES];
    for (unsigned int i = 0; i < as.n_clientes; i++) {
        emparejado[i] = false;
    }

    // Llamar a la función generalizada con estado inicial:
    // - emparejado[] = todos false (nadie tiene pareja)
    // - afinidad_actual = 0 (no hemos formado ninguna pareja)
    // - mejor = 0 (si no hay emparejamientos factibles, retornamos 0)
    return vuelta_atras_gen(emparejado, 0, as, 0);
}

void ejecuta_caso() {
    tMatrizAfinidad as;
    cin >> as.n_clientes;
    for (unsigned int i = 0; i < as.n_clientes; i++) {
        for (unsigned int j = 0; j < as.n_clientes; j++) {
            cin >> as.afinidades[i][j];
        }
    }
    cout << maxima_afinidad(as) << endl;
}

int main() {
    unsigned int num_casos;
    cin >> num_casos;
    for (unsigned int i = 0; i < num_casos; i++) {
        ejecuta_caso();
    }
}
