# Enunciado del ejemplo 

Dos threads ejecutan las siguientes tareas. Liste todas las rutas de ejecución indicando el valor final de la variable x y la salida estándar. Una ruta de ejecución (execution path) es una combinación de instrucciones que depende del orden en que sean ejecutadas.

procedure main: <br>
  >shared x := 0 <br>
  >create_thread(thread_a) <br>
  >create_thread(thread_b) <br>

end procedure

procedure thread_a: <br>
  >x := 5 <br> 
  >print(x) <br>

end procedure

procedure thread_b: <br>
  >x := 7 <br>

end procedure