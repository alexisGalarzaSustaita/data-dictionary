#include "main.h"

int main(int argc, char** argv) {
    char name[50];

    printf("Enter a Data Dictionary Name: ");
    
    fgets(name, sizeof(name), stdin);

    FILE* dataDictionary = initializeDataDictionary(name); 
    
    //createEntity(dataDictionary);
    //createEntity(dataDictionary);
    //createEntity(dataDictionary); 

    captureEntities(dataDictionary);
    captureAttributesForEntity(dataDictionary);
    showEntitiesWithAttributes(dataDictionary);

    fclose(dataDictionary); 

    return 0; 
}