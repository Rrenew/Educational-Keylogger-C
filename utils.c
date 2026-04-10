#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

#include "utils.h"

// funcao pra pegar o horario 
void pegar_horario(char *buffer, int tamanho) {
    time_t agora;
    struct tm *info;
    
    time(&agora);
    info = localtime(&agora);
    strftime(buffer, tamanho, "%H:%M:%S", info);
}