#include "main.h"

int main(int argc, char** argv) {
    char name[50];

    printf("Enter a Data Dictionary Name: ");
    
    // Usamos gets (no recomendado por razones de seguridad)
    gets(name);

    if (initializeDataDictionary(name) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to initialize data dictionary.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}