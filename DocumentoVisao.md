## Projeto ##
Desenvolver um compilador de um subconjunto de Java para código de máquina do processador de Mano&Kime, usado na disciplina de Laboratório e Arquiteturas de Computadores (LOAC). Como entrada esse compilador deve aceitar um arquivo de código fonte C que não usa classe e só permite um único método chamado "main". O compilador deve gerar em arquivo de saída compatível com o assembly usado no compilador desenvolvido na disciplina LOAC, que segue as especificações do livro do Mano&Kime.


---

## Perfil do Usuário ##
Usuários: Alunos da Disciplina de LOAC. O nosso publico alvo são os alunos da disciplina de LOAC. <br />
Um dos objetivos da disciplina é aprender a manipular um processador. Os alunos aprendem na disciplina a construir um processador com operações básicas, no intuíto de conhecer como é um processador e como o mesmo manipula os dados. Alocando dados em registradores, passando os dados pela ULA, e vendo na prática como o processo de computação ocorre, como os processadores se comunicam com memoria e alocam e desalocam dados da mesma. <br />
Será beneficiado com esse projeto toda a comunidade do curso de computação, já que a disciplina de LOAC é obrigatória.
Esse projeto será utilizado como uma ferramenta de auxílio ao ensino do conteúdo da disciplina. O mesmo também será usado para fazer demonstrações de como o código gerado por uma linguagem de programação é transformado em código de máquina. <br />
O uso desse processador tem o objetivo de melhorar o entendimento das operações básicas de um processador, e como o mesmo processa as instruções.


---

## Necessidades do Cliente ##

Necessidades do cliente: Esse compilador, em termos de aprendizagem, cria um elo entre as
disciplinas de linguagens de programação e LOAC, evidenciando que uma
linguagem de programação é convertido numa linguagem assembly que eles
já vêm usando em LOAC.


---


## Requisitos Funcionais ##

  * Desenvolver o compilador baseado na sintaxe da linguagem C.
  * Ser correto.
  * Gerar codigo para a linguagem assembly de acordo com o processador desenvolvido em LOAC, seguindo as especificações do livro do Mano&Kime.
  * Não ter interface gráfica.
  * Utilizar mesmas funçoes e operaçoes do processador Mano&Kime, utilizando apenas 4 registradores, ([R0](https://code.google.com/p/projeto-es/source/detail?r=0),[R1](https://code.google.com/p/projeto-es/source/detail?r=1),[R2](https://code.google.com/p/projeto-es/source/detail?r=2),[R3](https://code.google.com/p/projeto-es/source/detail?r=3)).
  * O único tipo de dado suportado é inteiro com sinal de 16 bits.
  * Aceitar operação de Atribuição, Condicional(if, while).
  * Para as operação condicionais, utilizar operações relacionais utilizando operadores(< (menor que), > (maior que), <= (menor que), >= (maior que), !=(diferente), == (igualdade))
  * Para as operações de atribuição, sendo possível atribuir a uma variável o resultado de uma expressão aritmética ou lógica, não podendo misturar os tipos de expressão.
  * Para expressões aritméticas podem ser usados os operadores: +(soma), -(subtração).
  * Para expressões lógicas podem ser usados os operadores: &(e), |(ou), !(negação).
  * Suportar apenas um método main().
  * Aceitar comandos de input/output através da sintaxe "io", o valor será alocado no endereço 0xffff.



---


## Requisitos Não-Funcionais ##

  * Rodar e poder ser desenvolvido (recompilado) nos computadores do Labarc.
  * Ser escrito sem usar Java.
  * Usar linguagem imperativa C.
  * Usar ferramentas (analisador léxico e sintático) amplamente conhecidos e usados em ambiente Linux.
  * Deve ser possível que monitores futuros da disciplina LOAC possam
fazer manutenção do código fonte do projeto (qualidade de codificação
e documentação).


---


## Escopo Negativo ##

Existem algumas atividades relacionadas com o projeto que estão fora do escopo desse projeto.
  * Operações de multiplicação e divisão.
  * Uso de parênteses aninhados.
  * Uma expressão não pode ser dos 2 tipos, sendo aritmética ou lógica.


---