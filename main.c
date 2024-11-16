#include "main.h"

int main(int argc, char** argv) {
    char name[50];

    printf("Enter a Data Dictionary Name: ");
    
    fgets(name, sizeof(name), stdin);

    FILE* dataDictionary = initializeDataDictionary(name); 
    
    ENTITY newEntity; 

    printf("\nEnter the Entity name: "); 

    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.dataPointer = EMPTY_POINTER; 
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER; 
    
    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection); 


    printf("\nEnter the Entity name: "); 

    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.dataPointer = EMPTY_POINTER; 
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER;

    entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);


    printf("\nEnter the Entity name: "); 

    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.dataPointer = EMPTY_POINTER; 
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER;

    entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);



    printf("\nEnter the Entity name: "); 

    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.dataPointer = EMPTY_POINTER; 
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER;

    entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);
    
    
    fclose(dataDictionary); 

    return 0; 
}