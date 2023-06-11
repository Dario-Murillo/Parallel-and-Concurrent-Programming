# concurrente23a-dario-murillo

Dario Murillo -C15406

Glosario

1. Programacion Serial: Secuencia estandar, las tareas en los programas se ejecutan una tras otra hasta que la otra se haya realizado por completo.

2. Programacion Concurrente: Lo que no es serial, permite la ejecucion de tareas de manera intercalada, no hay que esperar para terminar las tareas.

3. Programacion Paralela: A diferencia de la concurrenciua, las tareas e instrucciones son ejecutadas al mismo tiempo exactamente, es simultaneo.

4. Concurrencia de tareas: Separacion de asuntos, crear distintos hilos que trabajen en subrutinas distintas, por ejemplo, un hilo trabajando en calculos, otro en impresion de datos y otro en una lectura de datos. A diferencia del paralelismo de datos en lugar de crear una gran cantidad de hilos para una tarea, creamos un hilo.

5. Paralelismo de datos: Los hilos pueden ser asignados a una misma tarea, para incrementar el rendimiento, de manera que cada uno de estos trabaja con una parte de los datos para reducir el tiempo de ejecuccion. Un ciclo for con una cantidad definidad de hilos llamando a la misma subrutina, es decir haciendo una misma tarea, es ejemplo de esto.

6. Hilo de ejecucion: Los hilos de ejecuccion son los "ejecutantes" responsables de llamar invocar el codigo, por ejemplo, un metodo main, estos son creados por el sistema operativo, hay hilos prinicipales y secundarios. "Un arreglo de valores que el sistema operativo carga en los registros de una CPU y permiten ejecutar codigo en ella".

7. Indertermismo: No podemos predecir el orden en que se van a llegar a cabo las tareas y las instrucciones concurrentes, siempre que haya concurrencia va a existir indeterminismo.

8. Memoria privada y compartida: Hacemos uso de la memoria privada al reservar memoria dinamica por metodos como malloc o alloc, o por ejemplo, al hacer uso de ciertas estructuras de datos como los structs, los hilos tienen memoria privada. La memoria compartida permite la comunicacion entre hilos, un ejemplo de esto es un caso en el que el sistema se quede sin memoria para hacer mas hilos y por lo tanto su numero es reducido y esto es comunicado a todos los hilos, igualmente hay datos compartidos entre todos estos.

9. Espera Activa: La accion de poner hilos en cola, de manera que los hilos ejecutan las instrucciones en orden y para que un hilo empieza a hacer una tarea tiene que tener confirmacion de que el hilo anterior termino su tarea. Un ejemplo de como se puede ejecutar esto es mediante un ciclo, enbuclar un hilo sin que haga nada hasta su correspondiente turno, sin embargo, esto toma recursos de la cpu, sin embargo, la espera es la clave para el control de la concurrencia con espera pasiva.

10. Condicion de Carrera: Cuando multiples hilos acceden y tratan de modificar algo al mismo tiempo, se da una pelea por los recursos. La definicion formal: modificacion concurrente de memoria compartida.

11. Control de concurrencia: El control de concurrencia se basa en controlar el tiempo, los datos o en general las acciones que pueden efectuar los hilos. Esto se puede implementar por distintas maneras, por ejemplo con mutex o semafaros, ademas de ser util para evitar resultados indeseados como la condicion de carrera, nos permite crear un orden para ciertas acciones de los hilos, como su nombre lo indica controlamos la concurrencia, y por lo tanto el tiempo de ejecuccion de un hilo, asi como la espera de estos.

12. Seguridad condicional: Es un metodo alternativo sobretodo usado en problemas que involucran paralelismo de datos y que evita el control de concurrencia, permite que los hilos se esten ejecutando sin tener que esperar unos a otros esto haciendo que los hilos trabajen todos en recursos distintos, evitando de igual manera problemas de condicion de carrera.

13. Exclusion mutua: Solo un hilo va a estar ejecutando la o las instrucciones que decidamos proteger con un mutex/candado, cada mutex tendra su propia cola de espera.

14. Semaforo: Metodo de control de concurrencia, que de manera similar al mutex, crea un sistema el cual bloquea o libera recursos de manera que indica a cada hilo cuando puede realizar ciertas acciones, sin embargo, el mutex tiene indeterminismo mientras que los semaforos imponen orden. Los semaforos en particular se implementan de manera que haya un semafaro para cada hilo, la implementacion de este se da al asignarle un numero entero positivo, el cual es incrementado o decrementado y en base a su valor indica cuando un hilo puede actuar. Los numeros negativos indican los hilos esperando al semaforo, si un numero decrementa y es negativo, bloquea el semaforo y cuando se incrementa se desbloquea y por lo tanto uno de los hilos procede a hacer la tarea.

15. Barrera: Un punto de encuentro, donde todos los hilos tienen que llegar, y todos los hilos quedaran en espera, es decir no podran seguir su ejecuccion, hasta que el ultimo hilo llegue. Se puede implementar con herramientas de control de concurrencia, como los semaforos.

16. Variable de condicion

17. Candado de lectura y escritura

18. Descomposicion: Identificar las tareas que se pueden realizar independientemente para asi, poder realizarlas de forma paralela, se dividen unidades de trabaje independientes e indivisibles. Existen multiples tecnicas para descomponer, como la descomposicion recursiva, de datos, exploratoria y especulativa.

19. Mapeo: Es un concepto directamente asociado a la descomposicion, ya que trata de asignar a hilos las tareas anteriormente identificadas en el metodo de descomposicion. Los mapeos pueden clasificarse en mapeos estaticos y dinamicos.

20. Incremento de velocidad: Una comparacion entre cuando dura un proceso antes y cuanto dura despues de llevar a cabo una optimizacion.

21. Comunicacion punto a punto entre procesos: Permite la transferencia de datos, es decir comunicacion entre multiples procesos, esta comunicacion se da desde un proceso hasta otro, un proceso puede enviar multiples mensajes a otros procesos repetidas veces, pero siempre se da una comunicacion unica entre el proceso que manda los datos y el que los recibe. La comunicacion es orientada arreglos lo que permite pasar multiples datos de un proceso a otro. Las instrucciones suelen enviar un arreglo o un dato y deben especificar el tamano de los datos que se quieren enviar, asi como el tipo de dato y a cual proceso se desea enviar, los procesos que reciben estos datos deben tener el mismo tipo de datos y suelen almacenar la informacion recibida en un buffer, estos pueden especificar de que procesos pueden o no recibir mensajes.

22. Comunicacion colectiva entre procesos: Al igual que la comunicacion punto a punto entre procesos, la comunicacion colectiva permite la transferencia de datos, la diferencia es que en vez de ser una comunicacion exclusiva entre un proceso a otro, la comunicacion colectiva como su nombre lo indica, hace que un proceso designado transmite mensajes o envie datos hacia todos los demas procesos. La comunicacion colectiva hace que el codigo sea mas compacto ya que evita tener que hacer ciclos para hacer que un proceso se comunique con todos los demas.

23. Reduccion: Operaciones que pueden ser aplicadas en distintas tecnologias como openmp o mpi. Las reducciones consisten de operaciones simples como sumas, restas, que son aplicadas a todos los procesos, en el caso de MPI, o a todos los hilos, en regiones paralelas en el caso de openmp. Estas operaciones, reducen los datos que cada uno de estos procesos o hilos tiene independientemente en un solo resultado.
