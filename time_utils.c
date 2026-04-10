#include <time.h>
#include <string.h>

void pegar_horario(char *buffer, int tamanho) {
    time_t agora;
    struct tm *info;
    
    time(&agora);
    info = localtime(&agora);
    strftime(buffer, tamanho, "%H:%M:%S", info);
}
