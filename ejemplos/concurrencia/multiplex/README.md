# Enunciado del ejemplo



Modifique el pseudocódigo dado para que imponga una cota o límite superior de n hilos ejecutando una sección de concurrencia acotada. A este patrón se le llama multiplex y es útil para problemas donde se tienen distintos entes trabajando al mismo tiempo pero con máximo de capacidad.

Por ejemplo, la cantidad de cajeros atendiendo en las ventanillas de un banco o de clientes patinando en la pista del salón de patines. En este último caso, si la capacidad de la pista se agota, algunos clientes tendrán que esperar afuera, y apenas un cliente sale de la pista, otro podrá ingresar de inmediato. Use el código siguiente para reflejar este comportamiento.

**procedure main:** <br>
  shared constant skater_count = read_integer() <br>
  shared constant room_capacity = read_integer() <br>
  create_threads(skater_count, skater) <br>
**end procedure** <br>

**procedure skater:** <br>
  // Concurrency-bounded region <br>
  skate() <br>
**end procedure** <br>

Modifique la red de Petri para reflejar la llegada de 10 patinadores a un salón de patines que tiene una capacidad para 3 patinadores concurrentes.
