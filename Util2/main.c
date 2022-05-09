#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "[-] Argumentos incorrectos\n");
        fprintf(stderr, "[+] Uso: ./util2 directorio\n");
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;
    char path[1025];
    struct stat file_data;
    mode_t mode;
    int success;

    // Abrir directorio
    if ((dir = opendir(argv[1])) == NULL)
    {
        fprintf(stderr, "No se puede abrir el directorio '%s'\n", argv[1]);
        exit(2);
    }

    // Iterar elementos
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            sprintf(path, "%s/%s", argv[1], entry->d_name);
            if (lstat(path, &file_data) == -1)
            {
                fprintf(stderr, "Error al hacer stat al fichero %s\n", entry->d_name);
                exit(3);
            }

            // Directorios
            if (S_ISDIR(file_data.st_mode))
            {
                mode = file_data.st_mode & 07777;
                mode |= (S_IXUSR);
                mode &= ~(0050);
                if ((success = chmod(path, mode)) == -1)
                {
                    fprintf(stderr, "Error al cambiar permisos del directorio %s\n", entry->d_name);
                    exit(4);
                }
                else if (S_ISLNK(file_data.st_mode)) // Symbolik link
                {
                    mode = file_data.st_mode & 07777;
                    mode |= (0700);
                    mode &= ~(0077);
                    if ((success = chmod(path, mode)) == -1)
                    {
                        fprintf(stderr, "Error al cambiar permisos del fichero %s\n", entry->d_name);
                        exit(5);
                    }
                }
                else
                {
                    mode = file_data.st_mode & 07777;
                    mode &= ~(0077);
                    if ((success = chmod(path, mode)) == -1)
                    {
                        fprintf(stderr, "Error al cambiar permisos del fichero %s\n", entry->d_name);
                        exit(6);
                    }
                }
            }
        }
    }
    closedir(dir);
    return 0;
}
