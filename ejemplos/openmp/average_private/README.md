# Enunciado del ejemplo 

La solución al ejemplo anterior tiene una serialización innecesaria provocada por el control de concurrencia para evitar una condición de carrera. Una alternativa es usar seguridad condicional (conditionally safety) ¿Qué diseño propondría usted para aplicar los beneficios de la seguridad condicional a la solución de este problema?

Una forma de crear seguridad condicional es con variables privadas declaradas en la pila de cada hilo de ejecución. Utilice este esquema para calcular sumas parciales. Aplique control de concurrencia para calcular la suma total.
