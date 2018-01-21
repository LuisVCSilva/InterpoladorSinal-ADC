# InterpoladorSinal-ADC

Projeto de Arquitetura de Computadores.

Implementação de um algoritmo de interpolação linear para aprimoramento da resolução do sensor ADC de um microcontrolador ATMega 8.

O que é: Lê varios valores de um sensor ADC, interpola intervalo de valores com o algoritmo de Newton-Gregory, retorna um f(x) correspondendo a um polinômio que melhor encaixa no conjunto de pontos de entrada.

Utilidade: 
 - Possibilidade de interpolar sinais, melhorando a resolução do sensor ADC, computando medidções intermediárias entre as leituras do sensor.

 - Armazenar um conjunto de valores de forma compacta na memória, representando um conjunto de valores, utilizando apenas um polinômio f(x) que gera tais valores dado seu indíce.
