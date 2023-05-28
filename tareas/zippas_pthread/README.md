# Descripcion del Problema

Este codigo consiste de un programa concurrente en C que encuentre mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP.

 **Este programa no extrae los archivos** simplemente encuentra la contraseña para el o los archivos, en caso de que se soliciten multiples, y imprime en la salida estandar la clave que descencripta cada archivo, en caso de no encontar la contraseña no presenta nada. 
 
El programa obtiene por medio de la entrada estandar de un archivo el alfabeto de posibles caracteres que componen 
las contraseñas, asi como el numero maximo de caracteres por el que esta compuesta la clave y las direcciones de los archivos a 
desencriptar. Ademas, se puede proveer como parametro el numero de hilos con el que el usuario desea correr el programa.

## Manual de uso 

El programa es compilado en una terminal  en la misma carpeta donde se encuentras los archivos, con el comando `"make"`. Esto generara un ejecutable, para correr la solucion basta con escribir
en la misma terminal el comando `bin/zippas_pthread` el cual hace referencia al ejecutable y luego redirigirlo a algun
archivo de la carpeta tests/inputXXX.txt por medio del simbolo `<`. 


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

En un caso como el anterior en el que no se provee la cantidad de hilos como argumento en la linea de comandos, el programa tomara la cantidad de CPUs del sistema
como el numero de hilos a usar, en caso de que si se quiera una cantidad de hilos especifica, se puede proveer como argumento, de esta manera: <br>

bin/zippas-serial x < test/input004.txt

Donde x es la cantidad de hilos que se quieren usar

## Creditos

Dario Murillo Chaverri @dario.murillo@ucr.ac.cr

Codigo reutilizado  para borrar una carpeta temporal https://stackoverflow.com/questions/2256945/removing-a-non-empty-directory-programmatically-in-c-or-c  respuesta de asveikau y pevik

Codigo reutilizado para obtener una serie de caracteres random https://stackoverflow.com/questions/15767691/whats-the-c-library-function-to-generate-random-string

[Link al archivo README de la carpeta desing](desing/README.md)

## Incremento de desempeño en casos de pruebas

La siguiente seccion muestra capturas de los resultados y tiempos de ejecuccion de algunos casos de prueba del programa concurrente, con el fin de demostrar el aumento de desempeño conforme a la version serial del programa. Las pruebas fueron hechas en una maquina con 4 nucleos, y los programas fueron ejecutados con 4 hilos, es decir con el numero por defecto de hilos.

### Input000

[input000_serial](desing/img/input000_serial.png)
[input000_concurrente](desing/img/input000_concurrente.png)

### Input001

[input001_serial](desing/img/input001_serial.png)
[input001_concurrente](desing/img/input001_concurrente.png)

### Input002

[input002_serial](desing/img/input002_serial.png)
[input002_concurrente](desing/img/input002_concurrente.png)

### Input003

[input003_serial](desing/img/input003_serial.png)
[input003_concurrente](desing/img/input003_concurrente.png)

### Input004

[input004_serial](desing/img/input004_serial.png)
[input004_concurrente](desing/img/input004_concurrente.png)


### Input005

[input005_serial](desing/img/input005_serial.png)
[input005_concurrente](desing/img/input005_concurrente.png)


