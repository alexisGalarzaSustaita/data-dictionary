#include "dictionary.h"

int initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;

    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen(dictionaryName, "w+");

    if (dictionary == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fwrite(&mainHeader, sizeof(mainHeader), 1, dictionary);
    
    fclose(dictionary);  

    return EXIT_SUCCESS;
}
