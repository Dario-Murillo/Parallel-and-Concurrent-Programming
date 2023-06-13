![Diagrama UML](/desing/zippas_prod_cons_diagram.png)

La imagen anterior hace referencia a un diagrama, el cual trata de ilustrar las estructuras de datos con sus respectivas subrutinas, atributos asi como las interacciones entre ellas a lo largo de la ejecuccion del programa. 

Los diagramas UML, representan los miembros y subrutinas de las estructuras de datos usadas a lo largo del programa. Ademas, se provee un ejemplo de ejecuccion.

El diagrama representa de una forma grafica y simplificada los llamados de subrutinas y en general el flujo del programa. Los circulos tratan de representar los distintos metodos que son llamados y durante las lineas de transicion se representa como cambian y se procesan los datos.

En este ejemplo representado se recibe de la entrada estandar, un alfabeto de claves que consiste de 2 numeros: 1 y 2 donde ademas el maximo largo de la clave es de 2, y el archivo a descifrar tiene el nombre f99.zip. Luego de ser recibidos en la entrada estandar, son analizados y almacenados en la estructura de datos "datos_t".

Seguidamente se crea un equipo de 2 hilos, los hilos son representados por medio de circulos dobles, ademas de esto, se representa en un diagrama UML los miembros de la estructura de datos que conforman estos hilos. Estos trabajan en memoria privada en paralelo, consumiendo las claves son producidas y puestas en la cola, la accion de "consumir" consiste en sacar esta clave de la cola e intentar abrir todos las copias de archivos con esta. Siguiendo asi un modelo de tipo "productor-consumidor" efectuando un mapeo dinamico.

Finalmente los hilos se unen al hilo prinicipal y la memoria privada es liberada, la clave del archivo es imprimada en pantalla, y el resto de datos utilizados en la ejecuccion son liberados.
