# Enunciado del ejemplo

1. Generalice su solución a la actividad rendezvous (Ejemplo 15) para asegurar que una cantidad arbitraria de hilos no continúen su ejecución hasta que todos hayan alcanzado (ejecutado) el punto de encuentro. Es decir, si se crean w hilos, los primeros w - 1 hilos que lleguen al punto de encuentro deberían esperar hasta que el hilo en la posición w llegue al punto de encuentro. En tal momento todos los w threads podrán continuar ejecutándose concurrentemente. A este patrón se le conoce como barrera (barrier). Implemente una barrera con semáforos en el siguiente pseudocódigo para que la instrucción Statement B se ejecute sólo hasta que todos los hilos hayan ejecutado la instrucción Statement A.

**procedure main:** <br>
  // How many threads have arrived to the barrier <br>
  shared count := 0 <br>
  // Protects the increment of the count <br>
  shared can_access_count := create_semaphore(1) <br>
  // Locked (0) until all threads arrive, then it is unlocked (1) <br>
  shared barrier := create_semaphore(0) <br>
  // Read thread count from standard input <br>
  shared constant thread_count = read_integer() <br>
  // Create a thread team running secondary <br>
  create_threads(thread_count, secondary) <br>
**end procedure** <br>

**procedure secondary:** <br>
  Statement A <br>
  // Adapt rendezvous solution here <br>
  // Statement B can be only executed until all threads have run Statement A <br>
  Statement B <br>
**end procedure** <br>

2. Agregue una barrera a la red de Petri para que los tres hilos ejecuten las instrucciones Statement B sólo hasta que los tres hayan ejecutado las instrucciones Statement A.

3. Generalice la red de Petri para que w hilos ejecuten las instrucciones Statement B sólo hasta que todos hayan ejecutado las instrucciones Statement A.
