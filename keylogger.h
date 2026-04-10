#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <linux/input.h>

// Tabela de teclas normais
extern char teclas[200];

// Funções de teclado
int procurar_teclado(void);

// Funções de tempo
void pegar_horario(char *buffer, int tamanho);

#endif // KEYLOGGER_H
