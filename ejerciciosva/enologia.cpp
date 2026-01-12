#include <iostream>
#include <algorithm>
using namespace std;

/* Estructura utilizada para almacenar la información de un tema */
typedef struct {
    float tiempo_req;  // tiempo requerido para estudiar el tema
    float nota_prim;   // aportación a la nota de la primera parte
    float nota_sec;    // aportación a la nota de la segunda parte
} tInfoTema;

/*
=========================================================================
DISEÑO DEL ALGORITMO DE VUELTA ATRÁS: EXAMEN DE MCVDO
=========================================================================

DESCRIPCIÓN DEL PROBLEMA:
BobEspojax debe elegir qué temas estudiar para maximizar su nota final,
cumpliendo las siguientes restricciones:
1. No puede exceder el tiempo disponible
2. Debe aprobar AMBAS partes del examen (nota ≥ 5 en cada una)
3. La nota final es la MEDIA de ambas partes
4. Si no aprueba ambas partes, retorna -1

TIPO DE PROBLEMA:
Es un problema de SELECCIÓN DE SUBCONJUNTO. Para cada tema debemos decidir:
- INCLUIRLO en nuestra selección (estudiarlo)
- NO INCLUIRLO (no estudiarlo)

GENERALIZACIÓN - PARÁMETROS:
La función vuelta_atras_gen necesita los siguientes parámetros:

1. const tInfoTema info_temas[]: Array con información de todos los temas
   (tiempo requerido, puntos parte 1, puntos parte 2)

2. unsigned int num_temas: Número total de temas disponibles

3. float tiempo_disponible: Tiempo total del que dispone BobEspojax

4. unsigned int tema_actual: Índice del tema que estamos considerando ahora
   - Hemos tomado decisiones sobre los temas 0..tema_actual-1
   - Ahora debemos decidir si estudiar o no el tema_actual
   - Valores: 0, 1, 2, ..., num_temas

5. float tiempo_usado: Tiempo acumulado de los temas que hemos decidido estudiar
   - Sirve para verificar si podemos añadir más temas
   - No debe exceder tiempo_disponible

6. float puntos_parte1: Puntos acumulados para la primera parte del examen
   - Suma de nota_prim de los temas seleccionados
   - Al final debe ser ≥ 5 para aprobar

7. float puntos_parte2: Puntos acumulados para la segunda parte del examen
   - Suma de nota_sec de los temas seleccionados
   - Al final debe ser ≥ 5 para aprobar

8. float mejor: La MEJOR nota final encontrada hasta el momento
   - Inicialmente -1 (significa "no hay solución válida")
   - Se actualiza cuando encontramos combinaciones que aprueban ambas partes


SOLUCIONES PARCIALES:
Una solución parcial es una SELECCIÓN INCOMPLETA de temas.
Representa las decisiones tomadas sobre los temas 0..tema_actual-1.

Ejemplo con 4 temas:
- Inicial: tema_actual=0, no hemos decidido nada
- Parcial: tema_actual=2, decidimos estudiar tema 0, no estudiar tema 1
- Completa: tema_actual=4, decidimos sobre todos los temas

Cada solución parcial se caracteriza por:
- Qué temas hemos decidido estudiar (implícito en el camino recursivo)
- Tiempo acumulado usado
- Puntos acumulados en cada parte


VIABILIDAD DE SOLUCIONES PARCIALES:
Una solución parcial es VIABLE si:

✓ tiempo_usado <= tiempo_disponible
  (No hemos excedido el tiempo disponible)

Si tiempo_usado > tiempo_disponible:
  → PODAR esta rama (no puede llevar a solución válida)


SOLUCIÓN FINAL VÁLIDA:
Una solución parcial es FINAL cuando:
  tema_actual == num_temas
  (Hemos considerado todos los temas)

Es VÁLIDA si además:
  puntos_parte1 >= 5  Y  puntos_parte2 >= 5
  (Aprobamos AMBAS partes)

Si es válida, calculamos:
  nota_final = (puntos_parte1 + puntos_parte2) / 2.0


GENERACIÓN DE SIGUIENTES SOLUCIONES PARCIALES:
Para cada tema tenemos 2 OPCIONES:

OPCIÓN 1 - NO estudiar el tema_actual:
  - No consumimos tiempo adicional
  - No ganamos puntos adicionales
  - Pasamos al siguiente tema

  Llamada: vuelta_atras_gen(..., tema_actual+1, tiempo_usado,
                            puntos_parte1, puntos_parte2, mejor)

OPCIÓN 2 - SÍ estudiar el tema_actual:
  - Verificar que haya tiempo suficiente
  - Si tiempo_usado + info_temas[tema_actual].tiempo_req <= tiempo_disponible:
      - Consumimos el tiempo del tema
      - Ganamos los puntos del tema en ambas partes
      - Pasamos al siguiente tema

  Llamada: vuelta_atras_gen(..., tema_actual+1,
                            tiempo_usado + tiempo_req,
                            puntos_parte1 + nota_prim,
                            puntos_parte2 + nota_sec, mejor)

IMPORTANTE: Exploramos AMBAS opciones para cada tema para encontrar la
combinación óptima. No sabemos de antemano cuál es mejor.


ANÁLISIS DE CASOS:

CASO BASE 1 - Todos los temas considerados:
  SI tema_actual == num_temas:
    // Hemos tomado decisiones sobre todos los temas
    SI puntos_parte1 >= 5.0 Y puntos_parte2 >= 5.0:
      // ¡Aprobamos ambas partes! Calcular media
      nota_final = (puntos_parte1 + puntos_parte2) / 2.0
      mejor = max(mejor, nota_final)
    // Si no aprueba ambas, no actualizamos mejor
    RETORNAR mejor

CASO BASE 2 - Poda por tiempo excedido:
  SI tiempo_usado > tiempo_disponible:
    RETORNAR mejor  // Rama inviable, no explorar más

CASO RECURSIVO - Explorar opciones para tema_actual:

  // OPCIÓN 1: NO estudiar tema_actual
  mejor = vuelta_atras_gen(..., tema_actual+1, tiempo_usado,
                          puntos_parte1, puntos_parte2, mejor)

  // OPCIÓN 2: SÍ estudiar tema_actual (si hay tiempo)
  SI tiempo_usado + info_temas[tema_actual].tiempo_req <= tiempo_disponible:
    nuevo_tiempo = tiempo_usado + info_temas[tema_actual].tiempo_req
    nuevos_p1 = puntos_parte1 + info_temas[tema_actual].nota_prim
    nuevos_p2 = puntos_parte2 + info_temas[tema_actual].nota_sec

    mejor = vuelta_atras_gen(..., tema_actual+1, nuevo_tiempo,
                            nuevos_p1, nuevos_p2, mejor)

  RETORNAR mejor


INMERSIÓN:
La función mejor_puntuacion es el punto de entrada (inmersión).
Inicializa el estado y llama a la generalización:

- tema_actual = 0 (empezamos con el primer tema)
- tiempo_usado = 0.0 (no hemos estudiado nada aún)
- puntos_parte1 = 0.0 (no tenemos puntos aún)
- puntos_parte2 = 0.0 (no tenemos puntos aún)
- mejor = -1.0 (indica "no hay solución válida")

Retorna el resultado de la generalización.


COMPLEJIDAD:
En el peor caso, exploramos todas las combinaciones de temas.
Complejidad temporal: O(2^n) donde n es el número de temas.
Las podas (tiempo excedido) reducen el espacio de búsqueda en la práctica.


EJEMPLO DE EJECUCIÓN:
4 temas, 25 horas
Tema 0: 20h → 1p (P1), 2p (P2)
Tema 1: 10h → 1.5p (P1), 2p (P2)
Tema 2: 10h → 2.5p (P1), 5p (P2)
Tema 3: 10h → 5p (P1), 1p (P2)

El algoritmo explora combinaciones como:
- [0,1,2,3]: tiempo excedido (50h > 25h) → inviable
- [2,3]: 20h, p1=7.5, p2=6 → válido, nota=6.75
- [0,1]: 30h > 25h → inviable
- etc.

Mejor solución encontrada: 6.75 (temas 2 y 3)

=========================================================================
*/

// Función generalizada que implementa la vuelta atrás
float vuelta_atras_gen(const tInfoTema info_temas[], unsigned int num_temas,
    float tiempo_disponible, unsigned int tema_actual,
    float tiempo_usado, float puntos_parte1,
    float puntos_parte2, float mejor) {

    // CASO BASE 1: Hemos considerado todos los temas
    if (tema_actual == num_temas) {
        if (puntos_parte1 >= 5.0 && puntos_parte2 >= 5.0) {
            float nota_final = (puntos_parte1 + puntos_parte2) / 2.0;
            if (nota_final > mejor) {
                mejor = nota_final;
            }
        }
        return mejor;
    }

    // CASO BASE 2: Poda por tiempo excedido
    if (tiempo_usado > tiempo_disponible) {
        return mejor;
    }

    // CASO RECURSIVO: Explorar las 2 opciones para tema_actual

    // *** CAMBIO IMPORTANTE: Ahora exploramos SÍ PRIMERO ***

    // OPCIÓN 1: SÍ estudiar el tema_actual (PRIMERO)
    // Esto nos ayuda a encontrar soluciones válidas más rápido
    float tiempo_tema = info_temas[tema_actual].tiempo_req;

    if (tiempo_usado + tiempo_tema <= tiempo_disponible) {
        float nuevo_tiempo = tiempo_usado + tiempo_tema;
        float nuevos_puntos_p1 = puntos_parte1 + info_temas[tema_actual].nota_prim;
        float nuevos_puntos_p2 = puntos_parte2 + info_temas[tema_actual].nota_sec;

        // Exploramos PRIMERO la opción de estudiar
        mejor = vuelta_atras_gen(info_temas, num_temas, tiempo_disponible,
            tema_actual + 1, nuevo_tiempo,
            nuevos_puntos_p1, nuevos_puntos_p2, mejor);
    }

    // OPCIÓN 2: NO estudiar el tema_actual (DESPUÉS)
    // Exploramos esta opción después
    mejor = vuelta_atras_gen(info_temas, num_temas, tiempo_disponible,
        tema_actual + 1, tiempo_usado,
        puntos_parte1, puntos_parte2, mejor);

    return mejor;
} 

/* PUNTO DE ENTRADA DEL ALGORITMO (Inmersión)
   temas: Información sobre los temas de los que consta el examen (ver tInfoTema)
   num_temas: Numero de temas de los que consta el examen -su información aparece
              al principio de info_temas
   tiempo_disponible: Tiempo total disponible para realizar el examen.
   Valor devuelto: La puntuación media máxima que se puede conseguir, o -1 si no
                   es posible aprobar el examen */
float mejor_puntuacion(const tInfoTema info_temas[], unsigned int num_temas,
    float tiempo_disponible) {

    // Caso especial: sin temas no hay examen
    if (num_temas == 0) {
        return -1.0;
    }

    // Llamar a la función generalizada con estado inicial:
    // - tema_actual = 0 (empezamos considerando el primer tema)
    // - tiempo_usado = 0.0 (no hemos estudiado nada aún)
    // - puntos_parte1 = 0.0 (no tenemos puntos en parte 1)
    // - puntos_parte2 = 0.0 (no tenemos puntos en parte 2)
    // - mejor = -1.0 (si no encontramos solución válida, retornamos -1)
    return vuelta_atras_gen(info_temas, num_temas, tiempo_disponible,
        0, 0.0, 0.0, 0.0, -1.0);
}

/* Código para leer y ejecutar casos de prueba: no debe modificarse */
const unsigned int MAX_TEMAS = 20;

bool procesa_caso() {
    int num_temas;
    tInfoTema info_temas[MAX_TEMAS];
    cin >> num_temas;
    if (num_temas != -1) {
        float tiempo_disponible;
        cin >> tiempo_disponible;
        for (int i = 0; i < num_temas; i++) {
            cin >> info_temas[i].tiempo_req;
            cin >> info_temas[i].nota_prim;
            cin >> info_temas[i].nota_sec;
        }
        cout << mejor_puntuacion(info_temas, num_temas, tiempo_disponible) << endl;
        return true;
    }
    else {
        return false;
    }
}

int main() {
    while (procesa_caso());
}
