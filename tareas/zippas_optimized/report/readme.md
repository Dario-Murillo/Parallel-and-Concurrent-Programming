Dario Murillo Chaverri C15406 <<dario.murillo@ucr.ac.cr>> 2023a

# Optimización 1

La primera parte de este reporte consiste en presentar la optmizacion propuesta libremente por nosotros sobre la primer tarea, es decir, la primer version serial que se creo. Para esto se seguira paso a paso el ciclo de optimizacion propuesto.

## Rendimiento

El primer paso de este ciclo, consiste en tomar la duracion del programa a optimizar, para esto, tomamos el tiempo de ejecuccion de nuestro programa mediante la herramienta perf. La ejecuccion del programa se llevo a cabo en un sistema operativo linux, en una computadora con 16 nucleos, con un caso de prueba grande. El caso de prueba utilizado fue el input008, el cual fue un caso de pruebas especial, creado por mi, es decir no es uno de los casos brindados provicionalmente. La duracion de la version serial, es decir, de la tarea01 con este caso de pruebas fue de 847,562123859 segundo, osea, aproximadamente 14 minutos.

## Profiling y Análisis

En el segundo paso, llevamos a cabo un análisis dinámico de rendimiento (profiling) mediante la herraminta callgrind. Luego visualisamos los resultados, para analizar cuales partes de codigo son las que requieren mas procesamiento, y por lo tanto, cuales son las parte que se deben optimizar.

[Profiling](img/callgrind_profiling.png)

Como podemos observar mediante los graficos y el apoyo visual de la herramienta de profiling, podemos observar que las partes del programa que mas requieren procesamiento y son las que toman mas tiempo, son las funciones encargadas de la generacion de claves y la apertura de los archivos, las cuales trabajan de manera conjunta. Debido a que el algoritmo es un algoritmo de fuerza bruta, que debe generar y probar todas las claves posibles hasta que encuentre la que descifra el archivo o haya probado todas las claves posibles. Mi diseño de optimizacion va a optar por aliviar el trabajo a la hora de abrir los archivos.

## Diseño

Como se menciono anteriorment el diseño de mi solucion va a consistir no tanto en cambiar la generacion de claves, si no en alivianar la carga de trabajo al tener que abrir archivos. Esto se logra, al cambiar el funcionamiento de la version en actual, ya que ahora, lo que sucede en el programa, es que, por cada clave que genera la funcion datos_generate_passw, es pasada como argumento al metodo datos_abrir_archivo que se encarga de abrir el zip que tambien se especifica como argumento, luego de reservar la memoria necesaria intenta abrir el archivo dentro del zip con la clave, en caso de ser exitosa devuelte true, de lo contrario false. Si analisamos, estamos abriendo y cerrando los archivos zips y reservando y liberando memoria, cada vez que generamos una nueva clave, este trabajo se puede simplificar al hacer que abra el archivo y reserve la memoria una unica vez antes de empezar a probar todas las claves para abrir el archivo dentro, y liberar esta memoria y cerrar el archivo una vez que se haya encontrado la clave o se hayan agotado las claves a probar.

## Resultados

Nuestra optimizacion si funciono ya que disminuyo el tiempo de ejecuccion del programa en un tiempo corrobarable. El tiempo de ejecuccion, en el caso de prueva, input008 fue de 820,841228273 segundos mientras que la version serial duro 847,562123859 segundos.

1. Speedup: 1,032553062
2. Eficiencia: 1,032553062 

La eficiencia es la misma al speed up ya que al ser una optimizacion serial solo hay un trabajador.

# Optimzacion 2

La optimización 2 consiste en hacer mejorar el rendimiento del programa al paralelizar tareas, es decir, hacer el programa concurrente repartiendo la generacion de claves lo mas equitativamente posible, es decir, haciendo un mapeo dinamico por bloque.

1. Speedup: 4,066938922
2. Eficiencia: 0,254183683

# Optimizacion 3

La tercera optimización consiste en paralelizar el programa, pero esta vez mediante un mapeo dinámico por medio del patrón productor-consumidor.

