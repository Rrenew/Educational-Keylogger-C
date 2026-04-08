# Keylogger Educacional em C (Linux)

Este projeto é um **keylogger simples desenvolvido em C para sistemas Linux**, com o objetivo **exclusivamente educacional**. Ele demonstra como capturar eventos de teclado diretamente do dispositivo de entrada (`/dev/input/eventX`).

**Aviso importante:** Este projeto foi criado apenas para fins de estudo sobre sistemas operacionais, dispositivos de entrada e segurança. **Não utilize este código para monitorar usuários sem consentimento.**

---

## Objetivos do Projeto

- Entender como o Linux gerencia dispositivos de entrada
- Trabalhar com leitura de arquivos de dispositivo (`/dev/input`)
- Manipular eventos de teclado (`struct input_event`)
- Praticar programação em C de baixo nível

---

##Como Funciona

O programa:

1. Abre o dispositivo de teclado (`/dev/input/event2`)
2. Lê continuamente eventos de entrada
3. Identifica teclas pressionadas
4. Converte códigos de tecla para caracteres
5. Exibe no terminal e salva em um arquivo (`log.txt`)

---

##Requisitos

- Linux
- Permissão de superusuário (root)
- Compilador GCC

---

## Compilação

```bash
gcc keylogger.c -o keylogger
