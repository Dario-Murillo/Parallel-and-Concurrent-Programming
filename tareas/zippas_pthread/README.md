# Descripcion del Problema

Este codigo es un programa concurrente en C que encuentre mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP.

 **Este programa no extrae los archivos** simplemente encuentra la contraseña para el o los archivos, en caso de que se soliciten multiples, y imprime en la salida estandar la clave que decencripta cada archivo, en caso de no encontar la contraseña no presenta nada. 

El programa obtiene por medio de la entrada estandar de un archivo el alfabeto de caracteres que componen 
las contraseña asi como el maximo de digitos por el que esta compuesta y los direcciones de los archivos a 
desencriptar. Ademas, se puede proveer como parametro el numero de hilos con el que el usuario desea correr el programa.

## Manual de uso 

El programa es compilado en una terminal con el comando `"make"`. Esto generara un ejecutable, para correr la solucion basta con escribir
en la misma terminal el comando `bin/zippas_pthread` el cual hace referencia al ejecutable y luego redirigirlo a algun
archivo de la carpeta test/input00X.txt por medio del simbolo `<`. 


Ejemplo: <br>

Emitir el comando make generara en la consola una respuesta como esta: <br>

mkdir -p build/ <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/arr_dinamico.c -o build/arr_dinamico.o <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/main.c -o build/main.o <br>
gcc -c -Wall -Wextra -pthread -g -std=c17 -Isrc -MMD src/simulacion.c -o build/simulacion.o <br>
mkdir -p bin/ <br>
gcc -Wall -Wextra -pthread -g -Isrc build/arr_dinamico.o build/main.o build/simulacion.o -o bin/zippas_pthread -lzip -lm <br>

Luego de esto el ejecutable se puede correr con un comando como este: <br>

bin/zippas-serial < test/input004.txt

En un caso como el anterior en el que no se provee la cantidad de hilos como argumentos el programa tomara la cantidad de CPUs del sistema
como el numero de hilos a usar, en caso de que si se quiera una cantidad de hilos, se puede proveer como argumento, de esta manera: <br>

bin/zippas-serial x < test/input004.txt

Donde x es la cantidad de hilos que se quieren usar

## Creditos

Dario Murillo Chaverri @dario.murillo@ucr.ac.cr

[Link al archivo README de la carpeta desing](desing/README.md)