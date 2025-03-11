#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // Para toupper()

// Función auxiliar para saber si una cadena es palíndromo
int esPalindromo(const char *cadena) {
    int i, j;
    i = 0;
    j = strlen(cadena) - 1;
    while (i < j) {
        if (cadena[i] != cadena[j])
            return 0; // No es palíndromo
        i++;
        j--;
    }
    return 1; // Sí es palíndromo
}

int main(int argc, char *argv[])
{
    char peticion[200];
    char respuesta[200];
    respuesta[0] = '\0';  // Dejamos la respuesta vacía al inicio

    
    strcpy(peticion, "1/Ana/2/Miguel/1/otto/2/Juan");

    // Vamos separando con strtok
    char *p = strtok(peticion, "/");
    while (p != NULL) {
        int codigo = atoi(p);  
        p = strtok(NULL, "/"); 
        if (p == NULL) {
            
            break;
        }
        char nombre[50];
        strcpy(nombre, p);

        
        if (codigo == 1) {
            // Palíndromo
            if (esPalindromo(nombre))
                sprintf(respuesta, "%s[%s: ES palíndromo]-", respuesta, nombre);
            else
                sprintf(respuesta, "%s[%s: NO es palíndromo]-", respuesta, nombre);
        }
        else if (codigo == 2) {
            // Pasar a mayúsculas
            char mayus[50];
            for (int i = 0; i < strlen(nombre); i++) {
                mayus[i] = toupper(nombre[i]);
            }
            mayus[strlen(nombre)] = '\0';
            sprintf(respuesta, "%s[%s]-", respuesta, mayus);
        }
        else {
            // Código desconocido; en un servidor real podríamos ignorar o avisar
            sprintf(respuesta, "%s[ERROR: código %d desconocido]-", respuesta, codigo);
        }

        // Avanzamos a la siguiente posible petición
        p = strtok(NULL, "/");
    }

    // Quitamos el último guión si la respuesta no está vacía
    int len = strlen(respuesta);
    if (len > 0 && respuesta[len - 1] == '-')
        respuesta[len - 1] = '\0';

    printf("Resultado final:\n%s\n", respuesta);

    return 0;
}
