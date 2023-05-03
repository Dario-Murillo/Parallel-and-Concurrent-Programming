# Enunciado del ejercicio 

Modifique la simulación de red para recibir en un argumento de línea de comandos la capacidad máxima de paquetes que cabe en cada cola de la simulación. Modifique la plantilla Queue para que represente un buffer acotado. Sugerencia, agregue un semárofo canProduce que se inicialice en esta capacidad indicada por el usuario en el constructor de la cola. Si este número no se provee, suponga el número más grande que permite un semáforo en POSIX, indicado por la constante SEM_VALUE_MAX declarada en climits.

Corra su simulación de red con diferentes límites en las colas ¿Se afecta el tiempo de ejecución?

-R/ Si el tiempo de ejecuccion se ve afectado ya que limita la cantidad de paquetes que entran en la cola. Sin embargo, para que este sea un efecto verdaderamente notable, hay que aumentar el delay a la hora de distribuir los paquetes, ya que de lo contrario, apenas se produce un paquete, este es consumido y deja espacio libre en la cola. Si el numero de paquetes que entra a la cola es limtado y encima se toma un tiempo para distribuir este paquete, si se notara que aumenta mucho el tiempo de ejecuccion.s