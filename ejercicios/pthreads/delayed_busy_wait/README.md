# Enunciado del ejercicio
Espera activa con retraso. ¿Se corrige el problema de la espera activa si en lugar de tener un ciclo vacío, se espera un cierto tiempo? Copie su carpeta ejemplos/pthreads/hello_order_busywait a ejercicios/pthreads/delayed_busy_wait. Permita que el usuario pueda invocar su programa dos argumentos de línea de comandos: la cantidad de hilos a crear, y la cantidad de microsegundos a esperar cuando no es el turno del hilo de ejecución.

## Espera activa con retraso constante. 
Si no es el turno de un hilo, éste espera una cantidad constante de microsegundos, algo como:

<p>// Constant delayed busy waiting: wait until it is my turn<br>
<p>while (next_thread < my_thread_id) { <br>
  usleep(delay);
<p>}

Recuerde probar la calidad de su código (sanitizers, linter). Luego ejecute al menos tres veces su solución (sin sanitizers) con la cantidad máxima de hilos de ejecución que su sistema operativo le permite crear y una espera de 50µs. Escriba en el readme.md del ejercicio la mayor de las duraciones que obtuvo de sus corridas. ¿Hubo una mejora de la espera constante respecto a la espera activa pura?

Al correr el programa en la version de espera activa con retraso constante 
1. Primera Duracion: 5.701099971s
2. Segunda Duracion: 5.696040950s
3. Tercera Duracion: 5.910783169s

Si hubo una mejora comparado con la espera activa pura


## Espera activa con retraso pseudoaleatorio. 
Altere su solución al ejercicio para que en lugar de esperar exactamente la cantidad de microsegundos indicada por el usuario, espere por una cantidad pseudoaleatoria de microsegundos cuyo límite es el número indicado por el usuario en el segundo argumento de línea de comandos. Sugerencia: puede usar compilación condicional para implementar esta variación. La espera varía en cada iteración del ciclo de espera activa, algo como:

<p>// Random delayed busy waiting: wait until it is my turn<br>
<p>while (next_thread < my_thread_id) { <br>
  const unsigned my_delay = rand_r(&my_seed) % max_delay;<br>
  usleep(my_delay);
<p>}

Ejecute al menos tres veces su solución pseudoaleatoria con la cantidad máxima de hilos y un máximo de espera de 50µs. Tome la mayor de las duraciones. ¿Hubo una mejora de la espera pseudoaleatoria respecto a la espera constante?

Al correr el programa en la version de espera activa con retraso pseudoaleatorio
1. Primera Duracion: 5.430848497s
2. Segunda Duracion: 4.523553412s
3. Tercera Duracion: 5.702301743s

Si hubo una mejora comparado con la espera activa con retraso constante



## Comparación de las esperas. 
¿Mejora el tiempo de ejecución de los dos tipos de esperas (constante y pseudoaleatoria) si disminuye o incrementa la espera máxima de los hilos? Haga al menos un par de ejecuciones con al menos los siguientes tiempos de espera:

1µs
- constante = 3.801403693s, 3.905168192s, 3.714170893s
- pseudoaleatoria = 3.706187944s, 3.894736560s, 3.689082649s

5µs
- constante = 4.220764891s, 4.231430854s, 4.158045610s
- pseudoaleatorio = 4.104121731s, 4.351450986s, 3.934841513s

25µs
- constante = 4.384902108s, 4.227527407s, 4.318295948s
- pseudoaleatorio = 4.110097659s, 4.053431938s, 4.343752765s

50µs
- constante = 5.903071652s, 4.957264470s, 5.370373616s
- pseudoaleatorio = 4.305738647s, 4.212525994s, 4.003300141s

75µs
- constante = 5.686171000s, 5.713861060s, 5.915777785s
- pseudoaleatorio = 4.660847533s, 4.729892074s, 4.344195805s

100µs
- constante = 7.518732852s, 6.667682677s, 6.696610536s
- pseudoaleatorio = 5.246977934s, 4.811853580s, 5.308031990s



![Grafico](/desing/grafico.png)


El mejor tiempo de espera fue el de la espera pseudoaleatorio con un segundo como el maximo delay, al incrementar este numero el rango de los numeros random a crear aumenta y por lo tanto cabe el tiempo suele aumentar ya que hay mas probabilidad de que los numeros sean mayores y por lo tanto tenga que pasar mas nanosegundos "durmiendo". De todas formas, la espera activa no es una solucion optima para ejercicios en los que haya evitar condiciones de carrera, lo mejor para hacer en estos casos es hacer uso de la espera pasiva mediante mutex.

