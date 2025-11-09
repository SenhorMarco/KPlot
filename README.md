# KPlot

Calculadora gráfica básica, escrita em C++, utilizando a biblioteca SDL para a parte gráfica e a biblioteca exprtk para leitura e manipulação das expressões matemáticas. Só colocar o diretório do executável no PATH, e rodar os comandos a partir de qualquer terminal.

A documentação da forma como se deve escrever as expressões matemáticas está disponível em https://github.com/ArashPartow/exprtk.

## Uso
```
 kplot expressao_1 expressao_2 expressao_3 ... expressao_n
```

As expressões matemáticas não devem conter espaços.

![Multiplas funções](/prints/1.png?raw=true "Funções x^2, x^3 e sqrt(x) + 3")
Exemplo 1:
```
kplot "x^2" "x^3" sqrt(x)+3
```

OBS: Alguns caracteres especiais, como o '^', precisam que a expressão esteja envolvida por aspas duplas. São válidos e equivalentes os comandos 
```
kplot x*x 
```
e 
```
kplot x^2
```

![Funções periódicas](/prints/2.png?raw=true "Função sen(x) + 1")
Exemplo 2: 
```
kplot sin(x)+1
```

## A ser adicionado
- [ ] Seleção de cores para os gráficos
- [ ] Gráficos de conjuntos de pontos
- [ ] Pop-up ao clicar em determinado ponto
- [ ] Valores numéricos explicitados no gráfico

---

Marco - 09/11/2025




