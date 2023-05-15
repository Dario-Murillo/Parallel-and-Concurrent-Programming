# Enunciado del ejercicio

Provea una solución concurrente con OpenMP del ordenamiento par-impar. Conjeture cuál o cuáles ciclos pueden ser paralelizados en este algoritmo de ordenamiento. Utilice la directiva parallel for para paralelizar el o los ciclos. Modifique el programa de pruebas para recibir como segundo argumento de línea de comandos la cantidad de hilos que deben emplearse. Asegúrese que su implementación produce resultados correctos y no genere fugas de memoria.

## Analisis

Debido a que en este ejercicio se debe usar la directiva parallel for, para paralelizar el algoritmo, y esta es una directiva que crea una nueva region paralela donde se crea un equipo de hilos que se reparte las iteraciones de un ciclo por contador, podemos optar por dos opciones.

1. Podemos crear una region paralela en el ciclo mas externo, lo cual va a hacer que los hilos se repartan sus iteraciones en las cuales luego haran los otros ciclos respectivamente

2. Podemos crear una region paralela en ambos ciclos internos, para repartir las iteraciones de estos

3. La tercera opcion la cual es intentar paralelizar los 3 ciclos con la directiva parallel for causa resultados incorrectos, ya que intenta crear equipos de hilos en una region ya paralelizada en vez de reutilizar estos recursos