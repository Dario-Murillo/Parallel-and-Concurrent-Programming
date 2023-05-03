# Enunciado del ejemplo

Modifique los códigos de los hilos para que la instrucción a1 y b1 se ejecuten siempre antes que las instrucciones a2 y b2. Este problema tiene el nombre francés rendezvous que significa encuentro en un punto de ejecución, y ninguno de los dos hilos pueden continuar su ejecución hasta que el otro haya llegado.

**procedure main:** <br>
  create_thread(thread_a) <br>
  create_thread(thread_b) <br>
end procedure <br>

**procedure thread_a::** <br>
  statement a1 <br>
  statement a2 <br>
**end procedure** <br>

**procedure thread_b:** <br>
  statement b1 <br>
  statement b2 <br>
**end procedure** <br>

Modifique la red de Petri para que ocurra el encuentro entre los hilos de ejecución:
