# Enunciado del ejercicio

Modifique el programa del rango del Ejercicio 61 [hybrid_distr_stdin] para que cada proceso reporte en la salida estándar la cantidad de segundos en que tarda su ejecución. Mida la duración desde que inicializa el ambiente de ejecución de MPI (MPI_Init) hasta inmediatamente antes de finalizarlo (MPI_Finalize). Un ejemplo de salida podría ser el siguiente.

$ mpiexec -n 2 -f hosts.txt ./hybrid_distr_wtime 3 20 <br>
hostname1:0: range [3, 12[ size 9 in 0.000123s <br>
	hostname1:0.0: range [3,6[ size 3 <br>
	hostname1:0.1: range [6,9[ size 3 <br>
	hostname1:0.2: range [9,12[ size 3 <br>
hostname2:1: range [12, 20[ size 8 in 0.000342s <br>
	hostname2:1.0: range [12,15[ size 3 <br>
	hostname2:1.1: range [15,18[ size 3 <br>
	hostname2:1.2: range [18,20[ size 2 <br>

¿Cuál de las dos formas de especificar el rango consigue menores tiempos de ejecución: en argumento de línea de comandos o la entrada estándar?

R/ Se suele tener menor tiempo de ejecución en la línea de comandos ya que no hay que esperar a que el usuario inserte numeros en la entrada estándar, ademas de que se ahorra tiempo al no tener que comunicar estos valores a otros procesos. Sin embargo, los tiempos suelen ser muy similares.

## Nota

Para poder medir todo el tiempo de ejecuccion desde el inicio hasta la finalizacion, como lo indican las instrucciones, incluyendo el tiempo que ocupa la parte paralela de openmp se tuvo que modificar la impresion. De manera que, primero sale las particiones paralelas de la muestra de numeros que openmp reparte entre multiples hilos (la cantidad de hilos es la cantidad de CPU's disponibles en el sistema), y luego el reporte general del proceso.

Ejemplo:

	hostname1:0.0: range [3,6[ size 3 <br>
	hostname1:0.1: range [6,9[ size 3 <br>
	hostname1:0.2: range [9,12[ size 3 <br>
hostname1:0: range [3, 12[ size 9 in 0.000123s <br>

