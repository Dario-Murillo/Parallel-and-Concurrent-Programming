# Enunciado del ejemplo

Agregue semáforos al pseudocódigo siguiente para forzar a que los incrementos en los hilos se hagan con exclusión mutua.	

**procedure main:** <br>
  shared count := 0 <br>
  create_thread(thread_a) <br>
  create_thread(thread_b) <br>
**end procedure** <br>

**procedure thread_a:** <br>
  count := count + 1 <br>
**end procedure** <br>

**procedure thread_b:** <br>
  count := count + 1 <br>
**end procedure** <br>
