# Enunciado del ejercicio

Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su readme.md las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.

### Ejecuciones del ejemplo 7

1. Primera ejecucion: 2.007955404s <br>
2. Segunda ejecucion: 1.894473435s <br>
3. Tercera ejecucion: 1.753399179s <br>

### Ejecuciones del ejemplo 8

1. Primera ejecucion: 1.295432595s <br>
2. Segunda ejecucion: 1.221549574s <br>
3. Tercera ejecucion: 1.229823930s <br>

### Conclusiones 

La menor duracion del ejemplo 7 fue 1.753399179 segundos mientras que la menor duracion del ejemplo 8 fue 
1.221549574s, la posible causa de este comportamiento es que en el ejemplo 7 se hace uso de semafaros una estructura concreta de control de concurrencia, esta al igual que otros metodos de control de concurrencia, como los mutex hacen uso de muchos procesos extra en la computadora, es decir, ocupan muchos mas recursos en el proceso de compilacion. En el ejemplo 8 no se hace uso de ninguna estructura formal de control de concurrencia, si no que en cambio se hace uso de la seguridad condicional, en la cual manejamos los recursos y tareas que pueden usar los hilos haciendo que cada uno de los hilos trabaje en espacios de memoria separados, evitando condiciones de carrera justo como lo hacen los semaforos pero evitando todos los procesos adicionales que el
compilador efectua cuando se agregan estructuras de control de concurrencia, haciendo asi, que tengo un
mucho menor tiempo de ejecucion.