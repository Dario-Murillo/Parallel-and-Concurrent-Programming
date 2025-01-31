# Enunciado del ejemplo



Haga que su solución a la actividad Ejemplo 19 sea reusable. Es decir, que la barrera quede lista para volver a usarse, por ejemplo, dentro de un ciclo. Debe asegurarse de que el contador quede en 0, y los threads que salen de la barrera no se combinen con otros que aún están en ella.

Sugerencia: Puede reemplazar la barrera por dos torniquetes. Un torniquete (turnstile) o "trompo" es un dispositivo de control de acceso que impide o da paso a una secuencia de personas de acuerdo a ciertas condiciones. Se usa típicamente en instalaciones deportivas, transporte público, acceso a edificios, parques de diversiones, y otros.

Un torniquete es un semáforo que detiene a todos los hilos que lleguen a él. Pero apenas un hilo logre pasarlo, inmediatamente habilita el paso al siguiente. Puede pensarse como una manera más elegante de que el último hilo que llega a la barrera haga un ciclo de incrementos por todos los demás.

**procedure main:** <br>
  ... <br>
  // Stops all threads, but the first one that passess it passess the rest <br>
  shared turnstile := create_semaphore(0) <br>
  ... <br>
**end procedure** <br>

**procedure secondary:** <br>
  while true do <br>
    Statement A <br>
    ... <br>
    // Use turnstile <br>
    wait(turnstile) <br>
    signal(turnstile) <br>
    ... <br>
    // Statement B can be only executed until all threads have run Statement A <br>
    Statement B <br>
  end while <br>
**end procedure**
