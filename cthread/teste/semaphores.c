#include <stdio.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"


/*
  Programa teste de semaforo
  Foram criadas 5 funcoes.
  A primeira faz uso de um semaforo para a alocação de um recurso. As outras são apenas comandos prints

  Forçando que a função main espere pela execução de algum thread qualquer, faço com que a maioria das  funções
  sejam executadas e que mostrem quando tentam alocar, quando usam e quando liberam um recurso.

  De acordo com as regras de uso de semáforos, não podem existir mais do que N threads utilizando um recurso ao mesmo tempo.
  Neste caso definimos N = 1.
  Para comprovar que os semaforos funcionam, executamos o codigo e procuramos por mensagens do tipo "RECURSO EM USO".

  Se houver, em qualquer parte da execução, ocorrencias da mensagem "RECURSO EM USO" que não sejam intercaladas por uma "LIBERAÇÂO DE RECURSO",
  significa que duas threads conseguiram acessar o recurso ao mesmo tempo.

  Portanto, esperamos SEMPRE intercalações perfeitas de mensagens de "RECURSO EM USO" e "LIBERAÇÂO DE RECURSOS"

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

csem_t semaphore;

int main(){
  csem_init(&semaphore, 1);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste, 0);
  ccreate((void*)&teste2, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste3, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste4, 0);
  ccreate((void*)&teste5, 0);
  ccreate((void*)&teste5, 0);
  ccreate((void*)&teste5, 0);
  cjoin(33);
}

int teste(){
  printf("recurso SOLICITADO\n");
  cwait(&semaphore);
  printf("recurso EM USO\n");
  cyield();
  csignal(&semaphore);
  printf("recurso LIBERADO\n");
  return 0;
}

int teste2(){
  printf("Teste 2\n");
  return 0;
}

int teste3(){
  printf("Teste 3\n");

  return 0;
}
int teste4(){
  printf("Teste 4\n");
  return 0;
}

int teste5(){
  printf("Teste 5\n");
  return 0;
}
