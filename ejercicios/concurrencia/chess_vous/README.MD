# Enunciado del ejercicio

En un torneo de ajedrez los participantes efectúan el siguiente itinerario tradicional. Los jugadores y árbitros se identifican para ingresar al centro deportivo (enter_room). Cada jugador ubica la mesa con su tablero y se sienta en ella. Una vez que ambos jugadores están presentes, avisan al árbitro. El árbitro establece el tiempo en el reloj (set_clock) del tablero y con ello ambos jugadores inician la partida (play_chess).

El problema se considera resuelto si el árbitro establece el reloj sólo hasta que los dos jugadores hayan ingresado al centro deportivo (enter_room) y los jugadores juegan (play_chess) hasta que el árbitro haya establecido el reloj (set_clock). Considere el siguiente código inicial que trata de representar el escenario descrito.

**procedure main()** <br>
  shared player1_ready := semaphore(0) <br>
  shared player2_ready := semaphore(0) <br>
  shared players_ready := semaphore(0) <br>
  shared clock_ready := semaphore(0) <br>

  create_thread(player1) <br>
  create_thread(player2) <br>
  create_thread(referee) <br>
**end procedure** <br>

**procedure player1()** <br>
  enter_room() <br>
  play_chess() <br>
**end procedure** <br>

**procedure player2()** <br>
  enter_room() <br>
  play_chess() <br>
**end procedure**

**procedure referee()** <br>
  enter_room() <br>
  set_clock() <br>
**end procedure** <br>

Agregue control de concurrencia al código inicial para considerando todas las posibles rutas de ejecución, el problema siempre esté resuelto.
