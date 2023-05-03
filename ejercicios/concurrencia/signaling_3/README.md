# Enunciado del ejercicio

Modifique los códigos de los hilos para que la instrucción a1 se ejecute siempre antes que la instrucción b1 y ésta siempre se ejecute antes que la instrucción c1. Este orden de ejecución puede abreviarse como a1 < b1 < c1.

**procedure main:** <br>
  create_thread(thread_a) <br>
  create_thread(thread_b) <br>
  create_thread(thread_c) <br>
**end procedure** <br>

**procedure thread_a:** <br>
  statement a1 <br>
**end procedure** <br>

**procedure thread_b:** <br>
  statement b1 <br>
**end procedure** <br>

**procedure thread_c:** <br>
  statement c1 <br>
**end procedure** <br>

Actualice la red de Petri para que el aviso se dé entre los tres hilos.
