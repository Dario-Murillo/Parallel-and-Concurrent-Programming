# Enunciado del ejemplo

Simule una carrera de relevos, donde los competidores son equipos de threads. Cada equipo consta de dos corredores. Un corredor tomará la estafeta (en inglés, baton) y se posicionará en la línea de salida. El otro corredor esperará en la mitad de la pista a que el primer corredor llegue y le transfiera la estafeta. En tal momento, el segundo corredor seguirá a toda velocidad hasta alcanzar la meta. Ganan únicamente los tres primeros equipos en llegar con la estafeta a la meta, quienes son premiados con oro, plata, y bronce.

Su simulación recibirá tres parámetros: la cantidad de equipos, la duración en milisegundos que tarda el primer corredor atravesando la etapa 1 de la carrera (entre la salida y la mitdad de la pista), y la duración en milisegundos del segundo corredor atravesando la etapa 2 de la carrera (de la mitad de la pista hasta la meta). Suponga que los corredores de una etapa tardan exactamente esta duración y no un valor generado al azar. Ejemplo de ejecución:

./relay_race 10 300 700 <br>
Place 1: team 2 <br>
Place 2: team 4 <br>
Place 3: team 1 <br>
Simulation time: 1.006079000s <br>

