# Enunciado del ejercicio

Generalice el patron de aviso (signaling) para dada una cantidad arbitraria w de hilos que ejecutan la instruccion a, lo hagan en el orden del numero de hilo. Por ejemplo, si ai es la ejecucion de la instruccion statement a por parte del hilo con numero i, entonces se asegure que siempre se ejecute la secuencia a0 < a1 < a2 < a3 < < aw.

**procedure main:** <br>
  declare thread_count := read_integer()  <br>
  create_threads(thread_count, secondary) <br>
**end procedure** <br>

**procedure secondary:** <br>
  statement a <br>
**end procedure**
