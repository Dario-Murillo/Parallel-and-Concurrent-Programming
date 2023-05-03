# Enunciado del ejemplo 



Suponga que el hilo de ejecución principal crea w=100 hilos secundarios con los siguientes códigos.

shared count := 0

**procedure main:** <br>
create_threads(100, secondary) <br>
**end procedure**

**procedure secondary:** <br>
for i := 1 to 100 do <br>
declare temp := count <br>
count := temp + 1 <br>
end for <br>
**end procedure** <br>


1. ¿Cuál es el valor más grande que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?

2. ¿Cuál es el menor valor que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?

