#include <stdio.h>
#include "../include/support.h"
#include "../include/cthread.h"

/*
  Programa teste para ccreate e cjoin

  Espera-se que as funções 1, 2, 4 e 5 sejam executadas do inicio ao fim normalmente.
  A funcao teste 3, no ententanto, executará a função 4 dentro de si através do cjoin. Portanto, só terminará após a execução do teste 4

  SAIDA ESPERADA:
    Sou o teste 1!

    Sou o teste 2!

    Sou o teste 3! E ainda nao terminei.
    Sou o teste 4!

    Sou o teste 3 e agora eu terminei.

    Sou o teste 5!
*/

int teste();
int teste2();
int teste3();
int teste4();
int teste5();

int main(){
  int thread = ccreate((void*)&teste, 0);
  cjoin(thread);
  thread = ccreate((void*)&teste2, 0);
  cjoin(thread);
  thread = ccreate((void*)&teste3, 0);
  cjoin(thread);
  thread = ccreate((void*)&teste5, 0);
  cjoin(thread);
}

int teste(){
  printf("Sou o teste 1!\n\n");
  return 0;
}

int teste2(){
  printf("Sou o teste 2!\n\n");
  return 0;
}

int teste3(){
  printf("Sou o teste 3! E ainda não terminei.\n");
  int thread = ccreate((void*)&teste4, 0);
  cjoin(thread);
  printf("Sou o teste 3 e agora eu terminei\n\n");
  return 0;
}
int teste4(){
  printf("Sou o teste 4!\n\n");
  return 0;
}

int teste5(){
  printf("Sou o teste 5!\n\n");
  return 0;
}
