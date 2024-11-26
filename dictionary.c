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
//Eliminar una entidad (Eliminacion lógica)
ENTITY removeEntity(FILE* dataDictionary, long currentEntityPointer, const char* entityName){
    long currentEntityDirection = -1; 

    fseek(dataDictionary, currentEntityDirection, SEEK_SET); 
    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary); 

    if(currentEntityDirection == -1){
        ENTITY emptyEntity = {0};
        return emptyEntity; 
    }
    else{
        ENTITY resultEntity; 
        long nextEntityDirection; 
        long nextHeaderPointer; 

        fseek(dataDictionary, currentEntityDirection, SEEK_SET); 
        fread(resultEntity.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2); 

        if(strcmp(resultEntity.name, entityName) == 0){
            fread(&resultEntity.dataPointer, sizeof(long), 1, dataDictionary); 
            fread(&resultEntity.attributesPointer, sizeof(long), 1, dataDictionary); 
            fread(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary); 

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

ATTRIBUTE removeAttribute(FILE* dataDictionary, long currentAttributePointer, const char* attributeName) {
    long currentAttributeDirection = -1;

    fseek(dataDictionary, currentAttributePointer, SEEK_SET);
    fread(&currentAttributeDirection, sizeof(long), 1, dataDictionary);

    if (currentAttributeDirection == -1) {
        ATTRIBUTE emptyAttribute = {0};
        return emptyAttribute;
    } else {
        ATTRIBUTE resultAttribute;
        long nextAttributeDirection;
        long nextHeaderPointer; 

        fseek(dataDictionary, currentAttributeDirection, SEEK_SET);
        fread(resultAttribute.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + sizeof(bool) + (sizeof(long) * 2); 

        if (strcmp(resultAttribute.name, attributeName) == 0) {
            fread(&resultAttribute.isPrimary, sizeof(bool), 1, dataDictionary); 
            fread(&resultAttribute.type, sizeof(long), 1, dataDictionary);
            fread(&resultAttribute.size, sizeof(long), 1, dataDictionary);  
            fread(&resultAttribute.nextAttribute, sizeof(long), 1, dataDictionary);         

            fseek(dataDictionary, currentAttributePointer, SEEK_SET);
            fwrite(&resultAttribute.nextAttribute, sizeof(long), 1, dataDictionary);

            return resultAttribute;
        } else {
            return removeAttribute(dataDictionary, nextHeaderPointer, attributeName);
        }
    }
}

void captureEntities(FILE* dataDictionary) {
    int keepCapturing = 1;

    while (keepCapturing) {
        printf("\n--- Capturing Entity ---\n");
        createEntity(dataDictionary);

        printf("\nDo you want to add another entity? 1)Yes 0)No: ");
        scanf("%d", &keepCapturing);
        getchar();
    }
}

ENTITY searchEntityByName(FILE* dataDictionary, const char* entityName) {
    long currentEntityDirection = -1;

    fseek(dataDictionary, MAIN_ENTITY_POINTER, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    while (currentEntityDirection != EMPTY_POINTER) {
        ENTITY currentEntity;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(currentEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
        fread(&currentEntity.dataPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.attributesPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.nextEntity, sizeof(long), 1, dataDictionary);

        if (strcmp(currentEntity.name, entityName) == 0) {
            printf("\nEntity '%s' found.\n", currentEntity.name);
            return currentEntity; 
        }

        currentEntityDirection = currentEntity.nextEntity;
    }

    printf("\nEntity '%s' not found.\n", entityName);
    ENTITY emptyEntity = {0};
    return emptyEntity;
}

void captureAttributes(FILE* dataDictionary, ENTITY currentEntity) {
    int keepCapturing = 1;

    while (keepCapturing) {
        printf("\n--- Capturing Attribute for Entity: %s ---\n", currentEntity.name);
        createAttribute(dataDictionary, currentEntity);

        printf("\nDo you want to add another attribute? 1)Yes 0)No: ");
        scanf("%d", &keepCapturing);
        getchar();
    }
}

void captureAttributesForEntity(FILE* dataDictionary) {
    char entityName[DATA_BLOCK_SIZE];

    showEntities(dataDictionary);

    printf("\nEnter the name of the entity to add attributes: ");
    fgets(entityName, sizeof(entityName), stdin);
    entityName[strcspn(entityName, "\n")] = '\0'; 

    ENTITY entity = searchEntityByName(dataDictionary, entityName);

    if (strlen(entity.name) > 0) {
        captureAttributes(dataDictionary, entity);
    } else {
        printf("\nCannot add attributes. Entity not found.\n");
    }
}

void showEntitiesWithAttributes(FILE* dataDictionary) {
    long currentEntityDirection = -1;

    fseek(dataDictionary, MAIN_ENTITY_POINTER, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if (currentEntityDirection == EMPTY_POINTER) {
        printf("\nNo entities found.\n");
        return;
    }

    printf("\n--- Entities List ---\n");
    while (currentEntityDirection != EMPTY_POINTER) {
        ENTITY currentEntity;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(currentEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
        fread(&currentEntity.dataPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.attributesPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntityDirection, sizeof(long), 1, dataDictionary);

        printf("\nEntity Name: %s\n", currentEntity.name);
        showAttributes(dataDictionary, currentEntity.attributesPointer);
    }
}

void showAttributes(FILE* dataDictionary, long attributesPointer) {
    long currentAttributeDirection = attributesPointer;

    if (currentAttributeDirection == EMPTY_POINTER) {
        printf("\nNo attributes found for this entity.\n");
        return;
    }

    printf("\n--- Attributes List ---\n");
    while (currentAttributeDirection != EMPTY_POINTER) {
        ATTRIBUTE currentAttribute;

        fseek(dataDictionary, currentAttributeDirection, SEEK_SET);
        fread(currentAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
        fread(&currentAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
        fread(&currentAttribute.type, sizeof(long), 1, dataDictionary);
        fread(&currentAttribute.size, sizeof(long), 1, dataDictionary);
        fread(&currentAttributeDirection, sizeof(long), 1, dataDictionary);

        printf("Name: %s\n", currentAttribute.name);
        printf("Primary Key: %s\n", currentAttribute.isPrimary ? "Yes" : "No");
        printf("Type: %ld\n", currentAttribute.type);
        printf("Size: %ld bytes\n", currentAttribute.size);
    }
}

void showEntities(FILE* dataDictionary) {
  long currentEntityDirection = -1;

  fseek(dataDictionary, MAIN_ENTITY_POINTER, SEEK_SET);
  fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

  if (currentEntityDirection == EMPTY_POINTER) {
    printf("\nNo entities found.\n");
    return;
  }

  printf("\n--- Entities List ---\n");
  while (currentEntityDirection != EMPTY_POINTER) {
    ENTITY currentEntity;

    fseek(dataDictionary, currentEntityDirection, SEEK_SET);
    fread(currentEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);

    fseek(dataDictionary, sizeof(long) * 2, SEEK_CUR); 

    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary);

    printf("\nEntity Name: %s\n", currentEntity.name);
  }
}