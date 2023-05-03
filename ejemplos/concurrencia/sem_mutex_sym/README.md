# Enunciado del ejemplo

Note que en la actividad anterior ambos threads ejecutaban el código en subrutinas distintas, a lo que se le llama una solución asimétrica (separación de asuntos o concurrencia de tareas). Sin embargo, el código de las subrutinas era el mismo y por tanto podría convertirse en una solución simétrica. En una solución simétrica los hilos ejecutan el mismo código, es decir, la misma subrutina.

Las soluciones simétricas son a menudo más fáciles de generalizar. Agregue semáforos al pseudocódigo siguiente para forzar a que los incrementos hechos por una cantidad arbitraria de hilos sean con exclusión mutua.

**procedure main:** <br>
  shared count := 0 <br>
  shared constant thread_count = read_integer() <br>
  create_threads(thread_count, secondary) <br>
**end procedure** <br>

**procedure secondary:** <br>
  // Critical section <br>
  count := count + 1 <br>
**end procedure** <br>

Corrija la red de Petri para que una cantidad arbitraria de hilos (cinco en la red dada) hagan el incremento con exclusión mutua.
