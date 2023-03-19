#Trasncripcion del enunciado

Copie su ejemplos/pthreads/hello a ejercicios/pthreads/recursive_hello. Puede renombrar ejercicios/pthreads/recursive_hello/src/hello.c a recursive_hello.c o si lo prefiere, main.c.

Modifique a greet() para recibir un número de tipo size_t. Si este número es 0, greet() imprime una despedida y el número recibido. Luego termina su ejecución.

Si el número que greet() recibe es mayor que cero, imprime un saludo y el número recibido. Luego crea un hilo para que invoque a greet() pero con un número menor que el recibido por parámetro.

Modifique el procedimiento main() para crear una variable local de tipo size_t inicializada en 2. Envíe este número como parámetro de la subrutina greet() que correrá en el hilo secundario.

#Predicccion

Mi prediccion es que el programa imprimira el primer hello desde el primer hilo, y luego imprimira varias veces el hello desde el segundo hilo, hasta llegar a 0, el numero de veces que haga la subrutina va a depender de la variable iniciada en el main que luego es pasada como parametro para la subrutina greet, ya que es lo que define cuantas veces se llama a esta recursivamente.
Es decir: Hello from main thread y luego Hello from secondary thread, varias veces con el numero respectivo

#Resultado

Hello from main thread

Hello from secondary thread, number received:2
Hello from secondary thread, number received:1
Bye from secondary thread, number received:0
