# Enunciado del ejemplo

Modifique los códigos de los hilos para que la instrucción a1 se ejecute siempre antes que la instrucción b1. Esto es, que un hilo envíe una señal (aviso) a otro (signaling).

**procedure main:** <br>
  create_thread(thread_a) <br>
  create_thread(thread_b) <br>
**end procedure** <br>

**procedure thread_a:** <br>
  statement a1 <br>
**end procedure** <br>

**procedure thread_b:** <br>
  statement b1 <br>
**end procedure** <br>

Modifique la red de Petri para que el hilo a haga el aviso al hilo b