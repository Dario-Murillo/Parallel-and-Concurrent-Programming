# Enunciado del ejercicio

Modifique su solución de Ejemplo 7 para inicializar todos los semáforos en 0. Luego haga que el hilo principal incremente el semáforo del hilo 0. ¿Produce el mismo resultado que su solución anterior?

¿Qué pasa si el hilo principal incrementa el semáforo del hilo 0 después de hacer su saludo Hello from main thread?

Luego de hacer las prubas nos damos cuenta de que al iniciar todos los semaforos en 0 y luego aumentar el primero en el hilo principal, conseguimos el mismo resultado de como si se implementara normalmente. No se puede decir lo mismo del segundo caso ya que al incrementar el semaforo luego del saludo Hello from main thread, nos aseguramos de que siempre se imprima primero este saludo y luego el resto de los saludos de los hilos secundario
