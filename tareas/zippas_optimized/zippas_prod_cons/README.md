# Descripcion del Problema

Este codigo consiste de un programa concurrente en C que encuentre mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP. A diferencia de la primer version concurrente, la cual hacia uso de un mapeo estatico por bloque para repartir su trabajo, esta hace uso de un mapeo dinamico, mediante el patron productor-consumidor.

 **Este programa no extrae los archivos** simplemente encuentra la contraseña para el o los archivos, en caso de que se soliciten multiples, y imprime en la salida estandar la clave que descencripta cada archivo, en caso de no encontar la contraseña no presenta nada. 
 
El programa obtiene por medio de la entrada estandar de un archivo el alfabeto de posibles caracteres que componen 
las contraseñas, asi como el numero maximo de caracteres por el que esta compuesta la clave y las direcciones de los archivos a 
desencriptar. Ademas, se puede proveer como parametro el numero de hilos con el que el usuario desea correr el programa.

## Manual de uso 

El programa es compilado en una terminal  en la misma carpeta donde se encuentras los archivos, con el comando `"make"`. Esto generara un ejecutable, para correr la solucion basta con escribir
en la misma terminal el comando `bin/zippas_prod_cons` el cual hace referencia al ejecutable y luego redirigirlo a algun
archivo de la carpeta tests/inputXXX.txt por medio del simbolo `<`. 


Ejemplo: <br>

Emitir el comando make generara en la consola una respuesta como esta: <br>

mkdir -p build/ <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/arr_dinamico.c -o build/arr_dinamico.o  <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/main.c -o build/main.o  <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/queue.c -o build/queue.o  <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/simulacion.c -o build/simulacion.o  <br>
mkdir -p bin/  <br>
gcc -Wall -Wextra -pthread -g -Isrc build/arr_dinamico.o build/main.o build/queue.o build/simulacion.o -o bin/  <br> zippas_prod_cons -lzip -lm  <br>

        
Luego de esto el ejecutable se puede correr con un comando como este: <br>

bin/zippas_prod_cons < test/input004.txt

En un caso como el anterior en el que no se provee la cantidad de hilos como argumento en la linea de comandos, el programa tomara la cantidad de CPUs del sistema
como el numero de hilos a usar, en caso de que si se quiera una cantidad de hilos especifica, se puede proveer como argumento, de esta manera: <br>

bin/zippas_prod_cons x < test/input004.txt

Donde x es la cantidad de hilos que se quieren usar

## Creditos

Dario Murillo Chaverri @dario.murillo@ucr.ac.cr

Codigo reutilizado  para borrar una carpeta temporal https://stackoverflow.com/questions/2256945/removing-a-non-empty-directory-programmatically-in-c-or-c  respuesta de asveikau y pevik

Codigo reutilizado para obtener una serie de caracteres random https://stackoverflow.com/questions/15767691/whats-the-c-library-function-to-generate-random-string

[Link al archivo README de la carpeta desing](desing/README.md)



