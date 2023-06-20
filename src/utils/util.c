#define _DEFAULT_SOURCE
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "../database/database.h"
#include <sys/stat.h>
#include <unistd.h>
#include "lista.h"


void printError(const char* message) {
    printf("[ERROR] %s\n", message);
}

void printInfo(const char* message) {
    printf("[INFO] %s\n", message);
}

void createDirectory(const char* path) {
    int result = mkdir(path, 0777);
    if (result != 0) {
        printError("Failed to create directory.");
        exit(1);
    }
}
int contLines(char* path){
     // nome e local do arquivo que será aberto para
  // obtermos a quantidade de linhas
  FILE *arquivo = fopen(path, "r");
  
  
  int quant_linhas = 0;
  int caractere, existe_linhas = 0;
   
  while((caractere = fgetc(arquivo)) != EOF){
    existe_linhas = 1; // há conteúdo no arquivo
     
    if(caractere == '\n'){ // é uma quebra de linha?
      // vamos incrementar a quantidade de linhas
      quant_linhas++;             
    } 
  }
 
  // se não houver uma quebra de linha na última linha
  // a contagem será sempre um a menos. Assim, é melhor
  // incrementar quant_linhas mais uma vez
  if(existe_linhas)
    quant_linhas++;

    return quant_linhas;
}
char* concatenatePaths(const char* path1, const char* path2) {
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    char* combined = (char*)malloc(len1 + len2 + 2);
    strcpy(combined, path1);
    strcat(combined, "/");
    strcat(combined, path2);
    return combined;
}

char* readTextFile(const char* name) {
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (name[i] != '\0') {
            filename[j] = name[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';
    printInfo(filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printError("Failed to open file for reading.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(fileLength + 1);
    fread(content, 1, fileLength, file);
    content[fileLength] = '\0';

    fclose(file);
    return content;
    
}
void writeTextFileNextLine(const char* name, const char* text){
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (name[i] != '\0') {
            filename[j] = name[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';
    printInfo(filename);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printError("Failed to open file for reading.");
        return;
    }
    fputc('\n', file);
    fputs(text, file);
    fclose(file);
}

void writeTextFile(const char* filename, const char* content) {
    
    char fileName[100];
    strcpy(fileName, filename);

    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printError("Failed to open file for writing.");
        return;
    }

    fputs(content, file);
    fclose(file);
}


int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}


char* generateUniqueIdentifier() {
    char* identifier = (char*)malloc((MAX_IDENTIFIER_LENGTH + 1) * sizeof(char));

    // Gerar um identificador único baseado na data e hora atual
    time_t currentTime = time(NULL);
    struct tm* timeInfo = localtime(&currentTime);
    strftime(identifier, MAX_IDENTIFIER_LENGTH, "%Y%m%d%H%M%S", timeInfo);

    return identifier;
}


const char* getVersionIdentifier(int versionIndex) {
    // Verificar se o índice da versão é válido
    if (versionIndex < 0 || versionIndex >= numVersions) {
        return NULL;
    }
    
    // Obter o identificador da versão correspondente
    char* identifier = (char*)malloc(sizeof(char) * MAX_IDENTIFIER_LENGTH);
    if (identifier == NULL) {
        return NULL;
    }
    
    // Copiar o identificador da versão para a variável "identifier"
    strncpy(identifier, versions[versionIndex].identifier, MAX_IDENTIFIER_LENGTH);
    
    return identifier;
}

void removeFile(char* file) {

    char filE[100];
    int i = 0;
    int j = 0;
   
   while (file[i] != '\0') {
            filE[j] = file[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filE[j] = '\0';
    // Verificar se o arquivo existe
    if (access(filE, F_OK) != 0) {
        printf("O arquivo %s não existe.\n", filE);
        return;
    }

    // Remover o arquivo
    int result = remove(filE);

}


void searchDirectoryFiles(char* path, Lista* header){

        // Abrir a pasta de conteúdo
        DIR* contentDir = opendir(path);
        if (contentDir == NULL) {
            printError("Failed to open content directory.");
            return;
        }

        // Ler os arquivos na pasta de conteúdo
        struct dirent* entry;
        while ((entry = readdir(contentDir)) != NULL) {
            if (entry->d_type == DT_REG) {  
                 lst_insere(header, entry->d_name);
            }
        }

        closedir(contentDir);
}