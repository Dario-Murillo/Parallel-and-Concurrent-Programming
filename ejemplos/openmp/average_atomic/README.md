# Enunciado del ejemplo 

En el ejemplo anterior, la región crítica consta de una operación muy elemental, que los procesadores pueden ejecutar de forma excluyente entre sus núcleos, gracias a las operaciones atómicas en su conjunto de instrucciones. Lenguajes de programación como C11/C++11 disponen de decoraciones para instruir al compilador a usar operaciones atómicas. La tecnología OpenMP también dispone de este constructo. Use operaciones atómicas de OpenMP para reemplazar el control de concurrencia en el cálculo de la suma total.