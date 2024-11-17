#include "dictionary.h"

FILE* initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;

    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen(dictionaryName, "w+");

    fwrite(&mainHeader, sizeof(mainHeader), 1, dictionary);
        
    return dictionary;
}


int appendEntity(FILE* dataDictionary, ENTITY newEntity) {
 
    fseek(dataDictionary, 0, SEEK_END);
    
    long entityDirection = ftell(dataDictionary);

    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary); 
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary); 
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary); 

    return entityDirection; 
}

void reorderEntities(FILE* dataDictionary, long currentEntityPointer, const char* newEntityName, long newEntityDirection){
    long currentEntityDirection = -1; 

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary); 

    if (currentEntityDirection == -1) {
        
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);
    }
    else {
        char currentEntityName[DATA_BLOCK_SIZE]; 
        long nextEtityDirection; 
        long nextHeaderPointer; 


        fseek(dataDictionary, currentEntityDirection, SEEK_SET);

        fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);

        if (strcmp(currentEntityName, newEntityName) < 0) {
            
            reorderEntities(dataDictionary, nextHeaderPointer, newEntityName, newEntityDirection);
        }
        else {

            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);

            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + (sizeof(long) * 2), SEEK_SET);
            fwrite(&currentEntityDirection, sizeof(long), 1, dataDictionary);
        }
    }
}

void createEntity(FILE* dataDictionary) { 
    ENTITY newEntity; 

    printf("\nEnter the Entity name: "); 

    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.dataPointer = EMPTY_POINTER;
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER; 

    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);
}

void createAttribute(FILE* dataDictionary, ENTITY currentEntity) {
    ATTRIBUTE newAttribute; 

    printf("\nEnter the Attribute name: "); 

    fgets(newAttribute.name, sizeof(newAttribute.name), stdin); 
    newAttribute.isPrimary = false; 
    newAttribute.type = EMPTY_POINTER;
    newAttribute.size = EMPTY_POINTER; 
    newAttribute.nextAttribute = EMPTY_POINTER; 

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAttributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}

int appendAttribute(FILE* dataDictionary, ATTRIBUTE newAttribute) {
 
    fseek(dataDictionary, 0, SEEK_END);
    
    long attributeDirection = ftell(dataDictionary);

    fwrite(newAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary); 
    fwrite(&newAttribute.isPrimary, sizeof(bool), 1, dataDictionary); 
    fwrite(&newAttribute.type, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.nextAttribute, sizeof(long), 1, dataDictionary);  

    return attributeDirection; 
}

void reorderAttributes(FILE* dataDictionary, long currentAttributePointer, const char* newAttributeName, long newAttributeDirection){
    long currentAttributeDirection = -1; 

    fseek(dataDictionary, currentAttributePointer, SEEK_SET);
    fread(&currentAttributeDirection, sizeof(currentAttributeDirection), 1, dataDictionary); 

    if (currentAttributeDirection == -1) {
        
        fseek(dataDictionary, currentAttributePointer, SEEK_SET);
        fwrite(&newAttributeDirection, sizeof(long), 1, dataDictionary);
    }
    else {
        char currentAttributeName[DATA_BLOCK_SIZE]; 
        long nextAttributeDirection; 
        long nextHeaderPointer; 


        fseek(dataDictionary, currentAttributeDirection, SEEK_SET);

        fread(&currentAttributeName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + sizeof(bool) + (sizeof(long) * 2);

        if (strcmp(currentAttributeName, newAttributeName) < 0) {
            
            reorderAttributes(dataDictionary, nextHeaderPointer, newAttributeName, newAttributeDirection);
        }
        else {

            fseek(dataDictionary, currentAttributePointer, SEEK_SET);
            fwrite(&newAttributeDirection, sizeof(long), 1, dataDictionary);

            fseek(dataDictionary, newAttributeDirection + DATA_BLOCK_SIZE + (sizeof(long) * 2), SEEK_SET);
            fwrite(&currentAttributeDirection, sizeof(long), 1, dataDictionary);
        }
    }
}