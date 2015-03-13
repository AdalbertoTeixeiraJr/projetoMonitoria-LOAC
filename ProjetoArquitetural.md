# Visão Geral #
## Projeto Arquitetural ##
![http://projeto-es.googlecode.com/files/Projeto%20Arquitetural3.png](http://projeto-es.googlecode.com/files/Projeto%20Arquitetural3.png)


---

## Descrição ##

A arquitetura do sistema é dividida em módulos.
Existe o módulo responsável por fazer a análise léxica, outro módulo responsável por análise sintática e um módulo responsável por fazer a análise semântica.
Através do uso do terminal o usuário escreverá um código fonte em uma linguagem de programação (linguagem C).<br />
Após a escrita do código então o usuário fará uso de um script com o objetivo de traduzir esse código escrito para uma linguagem de máquina.<br />

# Explicação Funcionamento #

O analisador léxico irá ler o programa em linguagem fonte, enviar o que foi lido para a tabela de símbolos em forma de tokens da linguagem. Armazenando tudo que foi lido em uma estrutura de dados. O analisador sintático se comunica com a tabela de símbolos para verificar se a sintaxe da linguagem está correta, após a leitura pelo sintático temos o analisador semântico que vai verificar coerção de tipo, declaração de variáveis já existentes (por exemplo declaração de duas variáveis com mesmo nome) e verificar o escopo de cada variável dentro do programa, além de verificar se os valores atribuidos a cada variável estão dentro da faixa de valores aceitáveis.<br />

Após essas camadas de processamento o código será gerado por outra classe gerador de código esse módulo está acoplado com a análise semântica. <br />
Executando assim todo o processo necessário para a transformação de uma linguagem para outra linguagem.<br />

O código só pode ser gerado caso não haja nenhum erro de compilação em alguma das fases do processo de geração de código. Ou seja, não deve haver nenhum erro sintático, léxico ou semântico para o código poder ser gerado.<br />

Resolvemos manter esse modelo de arquitetura pela sua simplicidade, e divisão de tarefas em módulos a fim de melhor tratar erros e simplificar a implementação do compilador, visto que o processo de compilação se divide naturalmente nessas cinco fases.<br />

O código gerado é um ultimo passo dessa arquitetura. É mostrado diretamente na tela.<br />
Essa arquitetura foi definida de acordo com os requisitos do cliente.<br />

Considerando que o usuário irá apenas com um comando e o local do arquivo de entrada realizar todos esses passos automaticamente.

---


## Linhas Gerais ##
![http://projeto-es.googlecode.com/files/Projeto%20Arquitetural1.png](http://projeto-es.googlecode.com/files/Projeto%20Arquitetural1.png)