#include <datos.h>
#include <string.h>

int main(void) {
    FILE* input = stdin;
    char value = 0;
    int linea = 0;
    datos_t *datos = (datos_t*) malloc(1 * sizeof(datos_t));
    
    datos_innit(datos);

    while (value != EOF) {
        fscanf(input,"%c",&value);
        if(linea == 0) {   
            if (value != '\n') {
                arreglo_agregar(&datos->alfabeto, value);
            }
        } 
        if(linea == 1) {   
                
        }
        if(linea == 3) {
        
        }
        if(linea == 4) {
            
        }
        if(linea == 5) {
            
        }
        if(linea == 6) {
            break;
        }
        if (value == '\n') {
            linea++;
        }
    }

    free(datos);
    return 0;
}