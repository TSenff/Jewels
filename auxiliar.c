#include "auxiliar.h"

#include <stdio.h>

void check_init(bool check, char *mensagem){
    if (check)
        return;
    printf("Erro ao inicializar: %s",mensagem);
}