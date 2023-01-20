# Jewels

Implementação do Jogo Jewels em C com a biblioteca Allegro

# Duvidas: 
Criar o jogo em tabela e animar apos a chamada de função é uma ideia boa mesmow
Mascara de Bits(Usar Bits para Bool)
Para utilizar o display em outras bibliotecas eu tenho que enviar o ponteiro para o display


# A fazer
Otimizar verificação de trios para meio e para lados, Testes são repetidos 
Otimizar verificação de fim de jogo
Reescrever estoura usando check_vertical e check_horizontal

# Problemas 
Cai não funciona quando um movimento gera duas destruiçoes distintas que estão empilhadas
É possivel mover sem que ocorra a destruição 
Pedras em queda não calculam estouro
Tirar funçao cai de dentro da funçao destroi