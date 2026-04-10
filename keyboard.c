#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/input.h>

// Tabela de teclas normais
char teclas[200] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

int procurar_teclado() {
    printf("Procurando teclado..\n");
    printf("------------------------\n");
    
    for(int i = 0; i <= 15; i++) {
        char caminho[50];
        sprintf(caminho, "/dev/input/event%d", i);  // testa event0 até event15
        
        int fd = open(caminho, O_RDONLY);
        if(fd != -1) {
            char nome[256] = {0};
            // tenta ler o nome do dispositivo
            if(ioctl(fd, EVIOCGNAME(sizeof(nome)), nome) != -1) {
                printf("event%d: %s\n", i, nome);
                
                // palavras chaves que possam indicar que é teclado para facilitar identificação
                if(strstr(nome, "keyboard") || strstr(nome, "Keyboard") ||
                   strstr(nome, "Key") || strstr(nome, "kbd") ||
                   strstr(nome, "AT") || strstr(nome, "Set")) {
                    printf("\n Encontrado, usando /dev/input/event%d (%s)\n", i, nome);
                    return fd;
                }
            }
            close(fd);
        }
    }
    
    // caso não achar pelo nome
    printf("Não achado pelo nome, tentando event1..\n");
    int fd = open("/dev/input/event2", O_RDONLY);
    if(fd != -1) {
        printf("Usado event2, talvez seja teclado..\n");
        return fd;
    }
    return -1;
}
