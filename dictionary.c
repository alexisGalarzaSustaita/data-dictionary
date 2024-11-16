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

    fseek(dataDictionary, entityDataPointer, SEEK_SET);
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
            
            reorderEntities(dataDictionary, nextHeaderPointer, newEntityName, nextEtityDirection);
        }
        else {

            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);

            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + (sizeof(long) * 2), SEEK_SET);
            fwrite(&currentEntityDirection, sizeof(long), 1, dataDictionary);
        }
    }
}