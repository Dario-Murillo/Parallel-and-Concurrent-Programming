# concurrente23a-dario-murillo

Dario Murillo -C15406

Glosario

1.Programacion Serial: Secuencia estandar, las tareas en los programas se ejecutan una tras otra hasta que la otra se haya realizado por completo

2.Programacion Concurrente: Lo que no es serial, permite la ejecucion de tareas de manera intercalada, no hay que esperar para terminar las tareas

3.Programacion Paralela: A diferencia de la concurrenciua, las tareas e instrucciones son ejecutadas al mismo tiempo exactamente, es simultaneo

4.Concurrencia de tareas: Separacion de asuntos, crear distintos hilos que trabajen en subrutinas distintas, por ejemplo, un hilo trabajando en calculos, otro en impresion de datos y otro en una lectura de datos. A diferencia del paralelismo de datos en lugar de crear una gran cantidad de hilos para una tarea, creamos un hilo

5.Paralelismo de datos: Los hilos pueden ser asignados a una misma tarea, para incrementar el rendimiento, de manera que cada uno de estos trabaja con una parte de los datos para reducir el tiempo de ejecuccion. Un ciclo for con una cantidad definidad de hilos llamando a la misma subrutina, es decir haciendo una misma tarea, es ejemplo de esto

6.Hilo de ejecucion: Los hilos de ejecuccion son los "ejecutantes" responsables de llamar invocar el codigo, por ejemplo, un metodo main, estos son creados por el sistema operativo, hay hilos prinicipales y secundarios. "Un arreglo de valores que el sistema operativo carga en los registros de una CPU y permiten ejecutar codigo en ella"

7.Indertermismo: No podemos predecir el orden en que se van a llegar a cabo las tareas y las instrucciones concurrentes, siempre que haya concurrencia va a existir indeterminismo

8.Memoria privada y compartida: Hacemos uso de la memoria privada al reservar memoria dinamica por metodos como malloc o alloc, o por ejemplo, al hacer uso de ciertas estructuras de datos como los structs, los hilos tienen memoria privada. La memoria compartida permite la comunicacion entre hilos, un ejemplo de esto es un caso en el que el sistema se quede sin memoria para hacer mas hilos y por lo tanto su numero es reducido y esto es comunicado a todos los hilos, igualmente hay datos compartidos entre todos estos

9.Espera Activa

10.Condicion de Carrera

11.Control de concurrencia

12.Seguridad condicional

13.Exclusion mutua

14.Semaforo

15.Barrera

16.Variable de condicion

17.Candado de lectura y escritura

18.Descompisicion

19.Mapeo

20.Incremento de velocidad

21.Comunicacion punto a punto entre procesos

22.Comunicacion colectiva entre procesos

23.Reduccion
