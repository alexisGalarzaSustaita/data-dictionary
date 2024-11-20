#include "dictionary.h"
//Inicializa el diccionario de datos
FILE* initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;

    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen(dictionaryName, "w+");

    fwrite(&mainHeader, sizeof(mainHeader), 1, dictionary);
        
    return dictionary;
}

//Agrega una nueva entidad
int appendEntity(FILE* dataDictionary, ENTITY newEntity) {
 
    fseek(dataDictionary, 0, SEEK_END);
    
    long entityDirection = ftell(dataDictionary);

    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary); 
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary); 
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary); 

    return entityDirection; 
}
//Ordena las entidades
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
//Crea una nueva entidad
void createEntity(FILE* dataDictionary) { 
    ENTITY newEntity; 

    printf("\nEnter the Entity name: "); 
    fgets(newEntity.name, sizeof(newEntity.name), stdin); 
    newEntity.name[strcspn(newEntity.name, "\n")] = '\0';
    newEntity.dataPointer = EMPTY_POINTER;
    newEntity.attributesPointer = EMPTY_POINTER; 
    newEntity.nextEntity = EMPTY_POINTER; 

    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);
}

/*void createAttribute(FILE* dataDictionary, ENTITY currentEntity) {
    ATTRIBUTE newAttribute;
    long type;  

    printf("\nEnter the Attribute name: "); 
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    fflush(stdin); 
    printf("\nIs primary key? (true/false)");
    fgets(newAttribute.isPrimary, sizeof(bool), stdin); 
    printf("\nAttribute type: 1)int 2)long 3)float 4)char 5)bool")
    scanf("%ld", &type); 
    newAttribute.type = type;
    attributeSize(newAttribute);
    newAttribute.nextAttribute =EMPTY_POINTER;  

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAttributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}*/
//Crea un nuevo atributo
void createAttribute(FILE* dataDictionary, ENTITY currentEntity) {
    ATTRIBUTE newAttribute;
    long type;
    char response[10];

    fflush(stdin);
    printf("\nEnter the Attribute name: "); 
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    newAttribute.name[strcspn(newAttribute.name, "\n")] = '\0';
    fflush(stdin);
    printf("\nIs primary key? 0)false 1)true: ");
    fgets(response, sizeof(response), stdin);

    if(strcmp(response, "true") == 0)
        newAttribute.isPrimary = 1; 
    else
        newAttribute.isPrimary = 0; 
    fflush(stdin);
    printf("\nAttribute type: 1)int 2)long 3)float 4)char 5)bool: ");
    scanf("%ld", &type);
    newAttribute.type = type; 
    attributeSize(newAttribute);
    newAttribute.nextAttribute = EMPTY_POINTER;

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAttributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}
//Aggrega el atributo a la entidad a la que pertenece
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
//Ordena los atributos de la entidad
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

            fseek(dataDictionary, newAttributeDirection + DATA_BLOCK_SIZE + sizeof(bool) + (sizeof(long) * 2), SEEK_SET);
            fwrite(&currentAttributeDirection, sizeof(long), 1, dataDictionary);
        }
    }
}

ENTITY removeEntity(FILE* dataDictionary, long currentEntityPointer, const char* entityName){
    long currentEntityDirection = -1; 

    fseek(dataDictionary, currentEntityDirection, SEEK_SET); 
    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary); 

    if(currentEntityDirection == -1){
        return; 
    }
    else{
        ENTITY resultEntity; 
        long nextEntityDirection; 
        long nextHeaderPointer; 

        fseek(dataDictionary, currentEntityDirection, SEEK_SET); 
        fread(resultEntity.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2); 

        if(strcmp(resultEntity.name, entityName) == 0){
            fread(resultEntity.dataPointer, sizeof(long), 1, dataDictionary); 
            fread(resultEntity.attributesPointer, sizeof(long), 1, dataDictionary); 
            fread(resultEntity.nextEntity, sizeof(long), 1, dataDictionary); 

            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary); 

            return resultEntity;
        }
        else{
            return removeEntity(dataDictionary, nextHeaderPointer, entityName);
        } 
    }
}
//Verificar la funcion con el maestro

//Asignar el tamaño del atributo
void attributeSize(ATTRIBUTE newAtribute){
    int number; 

    switch (newAtribute.type){
        case 1:
            newAtribute.size = sizeof(int); 
            break;

        case 2:
            newAtribute.size = sizeof(long); 
            break;

        case 3:
            newAtribute.size = sizeof(float); 
            break;

        case 4:
            printf("Enter string size: ");
            scanf("%d", &number); 
            fflush(stdin);
            newAtribute.size = sizeof(char) * number; 
            break; 

        case 5: 
            newAtribute.size = sizeof(bool); 
            break;

        default:
            printf("Invalid attribute type.\n");
            break;
    }
}