# Enunciado del ejercicio

¿Afecta el orden de creación de los equipos el resultado de la carrera de relevos? Modifique su solución de la Section 3.2.6, es decir, el ejemplo relay_race, para crear los equipos en orden inverso y compare los resultados de las ejecuciones. Sugerencia: no duplique el código, sino que utilice metaprogramación (#ifdef…​) para escoger en tiempo de compilación el orden de creación de los equipos.

### Respuesta

Despues de probar con varias ejecucciones, podemos ver que el orden de ejecuccion de los hilos si se puede ver afectado por el orden de la creacion de estos, sobretodo en casos en los que no hay muchos hilos, si se puede notar una ligera ventaja de los hilos que son creados primero. Por ejemplo, en este caso se crean 10 equipos en orden inverso, es decir 10,9,...,0 y podemos notar que los primeros equipos en llegar se componen principalmente de los primeros equipos creados, en este caso los equipos 9, 7 y 8 <br>

bin/relay_race_order_effect 10 300 300 <br>
Place 1: team 9 <br> 
Place 2: team 7 <br>
Place 3: team 8 <br>
Place 4: team 2 <br>
Place 5: team 5 <br>
Place 6: team 3 <br>
Place 7: team 6 <br>
Place 8: team 1 <br>
Place 9: team 0 <br>
Place 10: team 4 <br>

Por otro lado, al correr el programa con los mismos parametros pero con un orden de creacion normal, podemos ver que al contrario del ejemplo anterior los primeros equipos se componen de equipos menores al 5, es decir los primeros equipos en crearse  <br>

bin/relay_race 10 300 300  <br>
Place 1: team 2  <br>
Place 2: team 6  <br>
Place 3: team 5  <br>
Place 4: team 0  <br>
Place 5: team 3  <br>
Place 6: team 8  <br>
Place 7: team 4  <br>
Place 8: team 9  <br>
Place 9: team 7  <br>
Place 10: team 1  <br>

Sin embargo, es importante notar que conforme aumenta el numero de equipos aumenta tambien el indeterminismo del programa de manera que si se ejecuta con una gran cantidad de equipos, por ejemplo 100 o mas, los hilos llegan de manera muy aleatoria e impredecible. De manera opuesta al comportamiento cuando hay pocos hilos, es dificil encontrar casos en los que los primeros hilos en ser creados sean tambien los primers hilos en llegar