# Descripcion del Problema

Este codigo es un programa procedimental en C que encuentre mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP. Este programa es una version optimizada de la primer version serial.

 **Este programa no extrae los archivos** simplemente encuentra la contraseña para el archivo y la imprime en la salida estandar, en caso de no encontra la contraseña no presenta nada. 

El programa obtiene por medio de la entrada estandar de un archivo el alfabeto de caracteres que componen 
las contraseña asi como el maximo de digitos por el que esta compuesta y los direcciones de los archivos a 
desencriptar.

## Manual de uso 

El programa es compilado en una terminal con el comando **"make"**. Para correr la solucion basta con escribir
en la misma terminal el comando bin/zippas_serial_optimized el cual hace referencia al ejecutable y luego redirigirlo a algun
archivo de la carpeta tests/input00X.txt por medio del simbolo <. 

Ejemplo: <br>

Emitir el comando make generara en la consola una respuesta como esta: <br>

mkdir -p build/ <br>
gcc -c -Wall -Wextra -g -std=c17 -Isrc -MMD src/arr_dinamico.c -o build/arr_dinamico.o <br>
gcc -c -Wall -Wextra -g -std=c17 -Isrc -MMD src/datos.c -o build/datos.o <br>
gcc -c -Wall -Wextra -g -std=c17 -Isrc -MMD src/main.c -o build/main.o <br>
mkdir -p bin/ <br>
gcc -Wall -Wextra -g -Isrc build/arr_dinamico.o build/datos.o build/main.o -o bin/zippas_serial_optimized -lzip -lm <br>

Una vez compilado se puede correr con un comando como este: <br>

bin/zippas_serial_optimized < test/input004.txt

## Creditos

Dario Murillo Chaverri @dario.murillo@ucr.ac.cr

[Link al archivo README de la carpeta desing](desing/README.md)
