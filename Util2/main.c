#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("[+] Uso: ./util2 directorio\n");
        return 1;
    }

    // Abrir directorio
    DIR *dir = opendir(argv[1]);
    struct dirent *entry;
    char path[1025];

    if (dir == NULL)
    {
        printf("No se puede abrir el directorio '%s'\n", argv[1]);
        return 1;
    }

    // Iterar elementos
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {

            strcpy(path, argv[1]);
            strcat(path, "/");
            strcat(path, entry->d_name);

            mode_t mode;

            switch (entry->d_type)
            {
            case DT_REG:
                mode = "0600";

                break;
            case DT_DIR:
                break;
            case DT_FIFO:
                break;
            default:
                printf("[-] [%s] Tipo de archivo no soportad\n", entry->d_name);
                continue;
            }
            chmod(path, mode);
        }
    }

    return 0;
}
