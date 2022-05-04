#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 256
#define BLOCK_SIZE 256

struct s_Reg
{
    char PathName[MAX_NAME];
    unsigned int Index;
    unsigned int Pos;
};

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *entry;
    FILE *result, *table, *temp;
    char path[1025];
    int file_length, index = 0, last_reg_index = 0;

    // Comprobar si se han pasado todos los parametros
    if (argc != 4)
    {
        printf("[+] Uso: ./util1 directorio Resultado.dat Tabla.dat\n");
        return 1;
    }

    // Abrir directorio
    dir = opendir(argv[1]);
    if (dir == NULL)
    {
        printf("No se puede abrir el directorio '%s'\n", argv[1]);
        return 1;
    }

    // Abrir ficheros
    result = fopen(argv[2], "w+");
    if (result == NULL)
    {
        printf("No se puede abrir el fichero '%s'\n", argv[2]);
        return 1;
    }
    table = fopen(argv[3], "w+");
    if (table == NULL)
    {
        printf("No se puede abrir el fichero '%s'\n", argv[3]);
        return 1;
    }

    // Leer directorio
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            strcpy(path, argv[1]);
            strcat(path, "/");
            strcat(path, entry->d_name);
            printf("Path: %s\n", path);
            temp = fopen(path, "rb"); // Abrir fichero en modo lectura de bytes
            if (temp == NULL)
            {
                printf("No se puede abrir el fichero '%s'\n", entry->d_name);
                continue;
            }

            // Leer longitud del archivo
            fseek(temp, 0, SEEK_END);
            file_length = ftell(temp);
            rewind(temp);

            // Copiar contenido a result
            for (int i = 0; i < ((BLOCK_SIZE / file_length) + 1); i++)
            {
                char *buffer = (char *)calloc(BLOCK_SIZE, sizeof(char)); // Crear buffer de elementos a 0
                fread(buffer, sizeof(char), BLOCK_SIZE, temp);
                fwrite(buffer, sizeof(char), BLOCK_SIZE, result);
                free(buffer);
            }

            // Escribir en Table
            struct s_Reg reg;
            strcpy(reg.PathName, path);
            reg.Index = index;
            reg.Pos = last_reg_index * BLOCK_SIZE; // set pos
            // fwrite("{\n\tPath: ;\n\tIndex: asdasda;\n\tPos: asdasda\n}\n",);

            fclose(temp); // Cerrar fichero
            index++;
        }
    }

    fclose(result);
    fclose(table);
    closedir(dir);

    return 0;
}
