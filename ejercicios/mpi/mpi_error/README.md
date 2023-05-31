# Enunciado del ejercicio

Implemente una clase para transportar potenciales errores que puedan surgir al usar la tecnología MPI y que necesitará en ejercicios siguientes.

Cree un archivo MpiError.hpp dentro de la carpeta mpi_wrapper/. Cree una clase MpiError que herede de std::runtime_error. Declare los tres constructores que están en el siguiente fragmento de código. Necesitará hacer un forward declaration de la clase Mpi. <br>

explicit MpiError(const std::string& message); <br>
MpiError(const std::string& message, const Mpi& mpi); <br>
MpiError(const std::string& message, const Mpi& mpi, const int threadNumber); <br>

En un archivo MpiError.cpp implemente los tres constructores. Cada uno debe invocar al constructor de std::runtime_error y proveerle un texto cuyo formato es el siguiente para cada constructor (las líneas corresponden corresponden a los constructores del código previo): <br>

message <br>
hostname:process_number: mesasge <br>
hostname:process_number.thread_number: mesasge <br>

Modifique la implementación de la clase Mpi para que cuando una función de la biblioteca MPI retorne un código de error, su clase lance una instancia de MpiError. Pruebe su código con varios procesos. Modifique temporalmente su código para provocar o lanzar una excepción y verificar que el texto con formato de la excepción se imprima en el error estándar.

