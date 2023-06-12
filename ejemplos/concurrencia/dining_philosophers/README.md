# Enunciado del ejemplo 

Corrija el siguiente pseudocódigo de concurrencia de recursos compartidos para que resuelva el problema de los filósofos comensales, y por lo tanto, logre cumplir con los tres requerimientos.

procedure main(argc, argv[]): <br>
  shared chopsticks[] := create_semaphores(5, 1)<br>

  for id := 0 to 5 do <br>
    create_thread(philosopher, id) <br>
  end for <br>
end procedure <br>

procedure philosopher(id): <br>
  while true do <br>
    think() <br>
    get_left_chopstick(id) <br>
    get_right_chopstick(id) <br>
    eat() <br>
    put_left_chopstick(id) <br>
    put_right_chopstick(id) <br>
  end while <br>
end procedure <br>

procedure get_left_chopstick(id): <br>
  wait(chopsticks[(id + 1) mod 5]) <br>
end procedure <br>

procedure get_right_chopstick(id): <br>
  wait(chopsticks[id]) <br>
end procedure <br>

procedure put_left_chopstick(id): <br>
  signal(chopsticks[(id + 1) mod 5]) <br>
end procedure <br>

procedure put_right_chopstick(id): <br>
  signal(chopsticks[id]) <br>
end procedure <br>

Considere las condiciones que son necesarias para que el bloqueo mutuo o la inanición ocurran, y cambie una de ellas a la vez para generar distintas soluciones:

1. Considere la lateralidad de los filósofos.

2. Considere la cantidad de filósofos que pueden comer concurrentemente.

3. Haga que los filósofos decidan con variables protegidas por exclusión mutua.

