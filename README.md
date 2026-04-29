# Educational Keylogger in C (Linux)

This project is a **simple keylogger developed in C for Linux systems**, with an **exclusively educational purpose**. It demonstrates how to capture keyboard events directly from the input device (`/dev/input/eventX`).

**Important notice:** This project was created for study purposes only, focusing on operating systems, input devices, and security. **Do not use this code to monitor users without consent.**

---

## Project Objectives

- Understand how Linux manages input devices  
- Work with device file reading (`/dev/input`)  
- Handle keyboard events (`struct input_event`)  
- Practice low-level programming in C  

---

## How It Works

The program:

1. Opens the keyboard device (`/dev/input/event2`)  
2. Continuously reads input events  
3. Identifies pressed keys  
4. Converts key codes to characters  
5. Displays them in the terminal and saves them to a file (`log.txt`)  

---

## Requirements

- Linux  
- Superuser (root) permission  
- GCC compiler  

---

## Compilation

```bash
gcc keylogger.c -o keylogger
