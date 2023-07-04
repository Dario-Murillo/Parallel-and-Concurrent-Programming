# Descripcion del Problema

Este codigo consiste de un programa concurrente en C que encuentre mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP. La concurrencia y paralelizacion del programa se llevo a cabo con la tecnologia de openmp.

 **Este programa no extrae los archivos** simplemente encuentra la contraseña para el archivo y la imprime en la salida estandar, en caso de no encontra la contraseña no presenta nada. 

El programa obtiene por medio de la entrada estandar de un archivo el alfabeto de caracteres que componen 
las contraseña asi como el maximo de digitos por el que esta compuesta y los direcciones de los archivos a 
desencriptar.

## Manual de uso 

El programa es compilado en una terminal con el comando `make`. Para correr la solucion basta con escribir
en la misma terminal el comando `bin/zippas_omp` el cual hace referencia al ejecutable y luego se redirige la entrada estandar a algun archivo de la carpeta que contiene los casos de prueba, `test/input00X.txt` por medio del simbolo <. Emitir el comando make genera una respuesta como la siguiente.

Ejemplo: <br>

gcc -c -Wall -Wextra -fopenmp -g -std=c17 -Isrc -MMD src/arr_dinamico.c -o build/arr_dinamico.o
gcc -c -Wall -Wextra -fopenmp -g -std=c17 -Isrc -MMD src/datos.c -o build/datos.o
gcc -c -Wall -Wextra -fopenmp -g -std=c17 -Isrc -MMD src/main.c -o build/main.o
mkdir -p bin/
gcc -Wall -Wextra -fopenmp -g -Isrc build/arr_dinamico.o build/datos.o build/main.o -o bin/zippas_omp -lzip -lm

Una vez compilado se puede correr con un comando como este: <br>

`bin/zippas_omp < test/inputXXX.txt`

Donde `XXX ` hace referencia a el numero del caso de prueba que se desea correr. En un caso como el anterior en el que no se provee la cantidad de hilos como argumento en la linea de comandos, el programa tomara la cantidad de CPUs del sistema como el numero de hilos a usar, en caso de que si se quiera una cantidad de hilos especifica, se puede proveer como argumento, de esta manera: <br>

`bin/zippas_omp x < test/inputXXX.txt`

Donde x es la cantidad de hilos que se quieren usar, por ejemplo, si se quiere usar 8 hilos:

`bin/zippas_omp 8 < test/inputXXX.txt`

## Creditos

Dario Murillo Chaverri @dario.murillo@ucr.ac.cr

[Link al archivo README de la carpeta desing](desing/README.md)
