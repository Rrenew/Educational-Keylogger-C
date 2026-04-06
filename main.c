#include <stdio.h>
#include <locale.h>
void main(){
  setlocale(LC_ALL, "Portuguese");
  float altura, peso, imc;
  char nome[50];
printf("Calculo do IMC\nDigite seu nome: ");
fflush(stdin);
gets(nome);

  printf("Digite sua altura: ");
  fflush(stdin);
  scanf("%f", &altura);

printf("Digite seu peso: ");
fflush(stdin);
scanf("%f", &peso);

 imc = (altura * altura) / peso;

printf("Olá %s, seu imc é: %f", imc, nome);

} 