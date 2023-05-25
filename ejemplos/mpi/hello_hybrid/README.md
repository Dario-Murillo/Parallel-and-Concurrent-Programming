# Enunciado del ejemplo

El ejemplo de "Hola mundo" lanza un único hilo principal (main thread) en cada proceso ejecutado. Modifique el programa para que cada proceso lance tantos hilos secundarios (secundary thread) como CPUs hay disponibles en la máquina donde se ejecuta. Cada hilo secundario debe imprimir en la salida estándar una línea antecedida por un tabulador en la que salude al usuario. Por ejemplo si se lanzan dos procesos y cada uno crea tres hilos secundarios, se podría obtener una salida como la siguiente: <br>

Hello from main thread of process 0 of 2 on hostname1 <br>
	Hello from thread 0 of 3 of process 0 on hostname1 <br>
	Hello from thread 2 of 3 of process 0 on hostname1 <br>
	Hello from thread 1 of 3 of process 0 on hostname1 <br>
Hello from main thread of process 1 of 2 on hostname2 <br>
	Hello from thread 1 of 3 of process 1 on hostname2 <br>
	Hello from thread 0 of 3 of process 1 on hostname2 <br>
	Hello from thread 2 of 3 of process 1 on hostname2 <br>

Dado que existe indeterminismo por la salida estándar, la salida que obtenga puede ser muy distinta a la anterior. Sugerencia: Puede usar OpenMP junto con MPI para lograr el modelo de concurrencia híbrida solicitada en esta actividad.
