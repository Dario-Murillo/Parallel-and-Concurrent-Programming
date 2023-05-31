# Enunciado del ejemplo

Modifique su clase Mpi en la carpeta mpi_wrapper/ (no cree una nueva) para que pueda enviar y recibir mensajes punto a punto con métodos send() y receive(). La interfaz de estos y futuros métodos de esta clase serán similares al pseudocódigo propuesto para distribución (Pseudocódigo de paso de mensajes).

Naturalmente es deseable que su clase Mpi sea capaz de enviar y recibir valores de todos los tipos de datos primitivos soportados por MPI. Sin embargo no debe introducir redundancia de código, es decir, evitar algo como:


Para evitar esta redundancia de código, utilice el paradigma de programación genérica. Cree plantillas de subrutinas. La plantilla recibe por parámetro el tipo de datos genérico a enviar o recibir por MPI. Sin embargo, la implementación de estos métodos tendrá que invocar funciones de la biblioteca de MPI que no son genéricas, sino que requieren constantes enteras acordes al tipo de datos. Puede usar las siguientes subrutinas que mapean los tipos de datos primitivos de C++ a constantes enteras que MPI requiere:

 public:
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; } <br>
  static inline MPI_Datatype map(char) { return MPI_CHAR; } <br>
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; } <br>
  static inline MPI_Datatype map(short) { return MPI_SHORT; } <br>
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; } <br>
  static inline MPI_Datatype map(int) { return MPI_INT; } <br>
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; } <br>
  static inline MPI_Datatype map(long) { return MPI_LONG; } <br>
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; } <br>
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; } <br>
  static inline MPI_Datatype map(unsigned long long) { return MPI_UNSIGNED_LONG_LONG; } <br>
  static inline MPI_Datatype map(float) { return MPI_FLOAT; } <br>
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; } <br>
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; } <br>

A modo de ejemplo, con los métodos anteriores, la expresión map(0.0) se evaluará por MPI_DOUBLE, y la expresión map(double()) también se evaluará por MPI_DOUBLE. Es decir, con un valor value cualquiera del tipo de datos typename DataType se puede obtener su constante de MPI correspondiente con la expresión map(value) ó map(DataType()). Provea entonces dos plantillas en su clase de acuerdo al siguiente interfaz en pseudocódigo para envío y recepción de valores escalares:

send(value, toProcess, tag=0)
receive(&value, fromProcess, tag=any_tag)

La primera plantilla servirá para generar métodos que reciben un valor (referencia constante) del tipo de datos genérico, y lo envía hacia el proceso destino usando MPI_Send(). La segunda plantilla recibe una referencia no constante de una variable y en ella escribe un valor recibido por la red proveniente del proceso fromProcess con MPI_Recv(). Con estos métodos el ejemplo Ejemplo 46 podrá reducirse al siguiente código que es mucho más claro y fácil de mantener:


Copie el código anterior a un archivo main.cpp en una carpeta send_recv_ord_sem_wrap/, asegúrese de que compila y produce los resultados esperados.


