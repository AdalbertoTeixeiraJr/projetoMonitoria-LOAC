//Gera o assembly do arquivo de entrada

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *sint_out;
int indice = 0;
char ids[7][16]; // EDA onde guardamos as ids das variaveis, a posicao que esta no array indica a posicao que ficara na memoria do processador
char valor[16];
char variavel[16] = "";
char numero[16] = "";
char operacao[16] = "";
char operador[16] = "";
char operadorAninhado[16] = "";
char tipo[16] = "";
char ptovirg[1] = "";
char igual[2] = "";
char var[3] = "";
char parentese[1] = "";
int sinal = 0;
int sinalAninhado = 0;
char operadorRelacional[16] = "";
int pc = -1; //	Guarda o valor de PC
int condicional = 0; //Flag que indica se esta dentro de uma condicional, guardando assim o valor do PC
char instrucoes[2048] = "";	// Variavel que guarda as instrucoes antes de imprimir
char instrucao[16] = "";
char instrucao1[16] = "";
char instrucao2[16] = "";
int ifElse = 0;
int lerOperacao = 1;
  

void declaracao();
int retornaID(char*);
void carregaNumero(char* reg, int sinal);
void carregaVariavel(char* reg, int sinal);
void carregaNumeroLog(char* reg, int sinal);
void carregaVariavelLog(char* reg, int sinal);
void expressao(int tipoExpressao);
void atribuicao_arit(char* operador);
void atribuicao_arit_aninhada(int complemento);
void atribuicao_log(char* operador);
void atribuicao_log_aninhada(int sinal);
int relacional(char* operador);
int subrotina(int flag);
void imprime(char* string, int cond); 
void carregaIO(char* reg, int sinal);


int main()
{
  char inst1[16] = "";
  char inst2[16] = "";

  int k;
  for(k = 0; k < 7; k++) {
    int l;
    for(l = 0; l < 16; l++){
 	ids[k][l] = 0;
    }
  }
  
  sint_out = fopen("sint_out.c", "r");

  if (!sint_out) {
    printf("ERRO: Arquivo nao pode ser aberto ou nao existe.");
  }
  
  condicional = 0;

  while(!feof(sint_out)) {
       
    if(lerOperacao==1){
	fscanf(sint_out," %s\n", operacao);	
    }
    lerOperacao = 1;

    //VRI = verifica identificador
    if(strcmp(operacao,"VRI")==0){
    	declaracao();
    }

    //ATR = atribuicao 
    else if(strcmp(operacao,"ATR")==0){
	fscanf(sint_out," %s\n", var);
	if(strcmp(var,"VAR")==0){
		fscanf(sint_out," %s\n", variavel);
		if(retornaID( variavel)!=-1){
			int a = retornaID(variavel);
			sprintf(instrucao, "LDI R0, %d\n",a);
			imprime(instrucao,condicional);
			pc++;
		}else{
			printf("ERRO: Variavel %s nao declarada!\n",variavel);
			exit(1);
		}
		fscanf(sint_out," %s\n", igual);
		if(strcmp(igual,"=")==0){
			expressao(0);	
		}else{
			printf("ERRO: Faltando '='.\n");
			exit(1);
		}
	}
	else{
		printf("ERRO: Codigo com erros, nao pode ser gerado.\n");
		exit(1);
	}
    }

    else if(strcmp(operacao,"IO")==0){
	carregaIO("R0", 0);
	fscanf(sint_out," %s\n", igual);
	if(strcmp(igual,"=")==0){
		expressao(0);	
	}else{
		printf("ERRO: Faltando '='.\n");
		exit(1);
	}
    }
    
    else if(strcmp(operacao,"IF")==0){
	fscanf(sint_out," %s\n", parentese);
	if(strcmp("(",parentese)!= 0){
		printf("Erro: Faltando '('.\n");
		exit(1);
	}
	expressao(1);	//Carregando resultado da 1ª expressao no R0	
	expressao(2);	//Carregando resultado da 2ª expressao no R1

	fscanf(sint_out," %s\n", operador);
	if(strcmp("{",operador)!= 0){
		printf("Erro: Faltando '{'.\n");
		exit(1);
	}
	
	int numeroInst = 0;
	pc++;	
	
	if(strcmp(operadorRelacional,"==")==0){
		imprime("NOT R1, R1\n",condicional);
		pc++;
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRZ R0, ");
		sprintf(inst2,"");
		
	}else if(strcmp(operadorRelacional,"!=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRZ R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,">=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);		
		
		sprintf(inst1,"BRN R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,"<=")==0){
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRN R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,">")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		sprintf(inst1,"BRZ R0, ");
		pc++;
		sprintf(inst2,"BRN R0, ");
	}else{
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		sprintf(inst1,"BRZ R0, ");
		pc++;
		sprintf(inst2,"BRN R0, ");
	}
	fscanf(sint_out," %s\n", operacao);
	if(strcmp(operacao,"ELSE")==0){
		lerOperacao = 1;
		if(strcmp(inst2,"")!=0){
			sprintf(instrucao,"%s%d\n%s%d\n",inst1, numeroInst+4, inst2, numeroInst+3);
		}else{
			sprintf(instrucao,"%s%d\n",inst1, numeroInst+3);	
		}		
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
		sprintf(instrucoes,"");
		fscanf(sint_out," %s\n", parentese);
		if(strcmp("{",parentese)!= 0){
			printf("Erro: Faltando '{'.\n");
			exit(1);
		}
		numeroInst = subrotina(1);
		condicional = 0;
		imprime("LDI R0, 0\n",condicional);
		sprintf(instrucao, "BRZ R0, %d\n", numeroInst+1);
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
	}else{	
		lerOperacao = 0;
		if(strcmp(inst2,"")!=0){
			sprintf(instrucao,"%s%d\n%s%d\n",inst1, numeroInst+2, inst2, numeroInst+1);
		}else{
			sprintf(instrucao,"%s%d\n",inst1, numeroInst);	
		}
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
		sprintf(instrucoes,"");
	}
	
    }

    else if(strcmp(operacao,"WHILE")==0){
	fscanf(sint_out," %s\n", parentese);
	if(strcmp("(",parentese)!= 0){
		printf("Erro: Faltando '('.\n");
		exit(1);
	}
	int pcAtual = pc;
	expressao(1);	//Carregando resultado da 1ª expressao no R0	
	expressao(2);	//Carregando resultado da 2ª expressao no R1
	fscanf(sint_out," %s\n", operador);	
	if(strcmp("{",operador)!= 0){
		printf("Erro: Faltando '{'.\n");
		exit(1);
	}
	int numeroInstrucoes = relacional(operadorRelacional);
	if(strcmp(instrucao2,"")!=0){
		sprintf(instrucao,"%s%d\n%s%d\n",instrucao1, numeroInstrucoes+4, instrucao2, numeroInstrucoes+3);
	}else{
		sprintf(instrucao,"%s%d\n",instrucao1, numeroInstrucoes+3);	
	}		
	imprime(instrucao,condicional);
	imprime(instrucoes, condicional);
	imprime("LDI R0, 0\n", condicional);
	pc++;
	pc++;
	sprintf(instrucao, "BRZ R0, -%d\n", pc-pcAtual);
	imprime(instrucao, condicional);
	
    } 
    
  }
  
  exit(1);
	
}

int subrotina(int flag){
  int pcAtual =  pc;
  condicional = 1;
  int lerOperacao = 1;
  char inst1[16] = "";
  char inst2[16] = "";
  char instrucao[20] = "";
  char instrucoes[2048] = "";

  while(!feof(sint_out)) {
    if(flag == 1 && lerOperacao == 1){
	fscanf(sint_out," %s\n", operacao);	
    }   
    flag = 1; 
    lerOperacao = 1;

    if(strcmp(operacao,"}")==0){
	condicional = 0;
	return pc-pcAtual;	
    }

    //VRI = verifica identificador
    if(strcmp(operacao,"VRI")==0){
    	declaracao();
    }

    //ATR = atribuicao 
    else if(strcmp(operacao,"ATR")==0){
	fscanf(sint_out," %s\n", var);
	if(strcmp(var,"VAR")==0){
		fscanf(sint_out," %s\n", variavel);
		if(retornaID( variavel)!=-1){
			int a = retornaID(variavel);
			sprintf(instrucao, "LDI R0, %d\n",a);
			imprime(instrucao,condicional);
			pc++;
		}else{
			printf("ERRO: Variavel %s nao declarada!\n",variavel);
			exit(1);
		}
		fscanf(sint_out," %s\n", igual);
		if(strcmp(igual,"=")==0){
			expressao(0);	
		}else{
			printf("ERRO: Faltando '='.\n");
			exit(1);
		}
	}
	else{
		printf("ERRO: Codigo com erros, nao pode ser gerado.\n");
		exit(1);
	}
    }
    // IO: Atribuicao a variavel de Output
    else if(strcmp(operacao,"IO")==0){
	carregaIO("R0", 0);
	fscanf(sint_out," %s\n", igual);
	if(strcmp(igual,"=")==0){
		expressao(0);	
	}else{
		printf("ERRO: Faltando '='.\n");
		exit(1);
	}
    }
    // IF: Operacao condicional
    else if(strcmp(operacao,"IF")==0){
	fscanf(sint_out," %s\n", parentese);
	if(strcmp("(",parentese)!= 0){
		printf("Erro: Faltando '('.\n");
		exit(1);
	}
	expressao(1);	//Carregando resultado da 1ª expressao no R0	
	expressao(2);	//Carregando resultado da 2ª expressao no R1
	fscanf(sint_out," %s\n", operador);
	if(strcmp("{",operador)!= 0){
		printf("Erro: Faltando '{'.\n");
		exit(1);
	}
	
	int numeroInst = 0;
	pc++;	
	
	if(strcmp(operadorRelacional,"==")==0){
		imprime("NOT R1, R1\n",condicional);
		pc++;
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRZ R0, ");
		sprintf(inst2,"");
		
	}else if(strcmp(operadorRelacional,"!=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRZ R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,">=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);		
		
		sprintf(inst1,"BRN R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,"<=")==0){
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		
		sprintf(inst1,"BRN R0, ");
		sprintf(inst2,"");

	}else if(strcmp(operadorRelacional,">")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		sprintf(inst1,"BRZ R0, ");
		pc++;
		sprintf(inst2,"BRN R0, ");

	}else{
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		numeroInst = subrotina(1);
		sprintf(inst1,"BRZ R0, ");
		pc++;
		sprintf(inst2,"BRN R0, ");

	}
	fscanf(sint_out," %s\n", operacao);
	// ELSE
	if(strcmp(operacao,"ELSE")==0){
		lerOperacao = 1;
		if(strcmp(inst2,"")!=0){
			sprintf(instrucao,"%s%d\n%s%d\n",inst1, numeroInst+4, inst2, numeroInst+3);
		}else{
			sprintf(instrucao,"%s%d\n",inst1, numeroInst+3);	
		}		
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
		sprintf(instrucoes,"");
		fscanf(sint_out," %s\n", parentese);
		if(strcmp("{",parentese)!= 0){
			printf("Erro: Faltando '{'.\n");
			exit(1);
		}
		numeroInst = subrotina(1);
		imprime("LDI R0, 0\n",condicional);
		sprintf(instrucao, "BRZ R0, %d\n", numeroInst+1);
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
		fscanf(sint_out," %s\n", operacao);
	}else{	
		lerOperacao = 0;
		if(strcmp(inst2,"")!=0){
			sprintf(instrucao,"%s%d\n%s%d\n",inst1, numeroInst+1, inst2, numeroInst);
		}else{
			sprintf(instrucao,"%s%d\n",inst1, numeroInst);	
		}
		imprime(instrucao,condicional);
		imprime(instrucoes,condicional);
		sprintf(instrucoes,"");
	}
	
    }
    
    //Operacao: WHILE
    else if(strcmp(operacao,"WHILE")==0){
	fscanf(sint_out," %s\n", parentese);
	if(strcmp("(",parentese)!= 0){
		printf("Erro: Faltando '('.\n");
		exit(1);
	}
	int pcAtual = pc;
	expressao(1);	//Carregando resultado da 1ª expressao no R0	
	expressao(2);	//Carregando resultado da 2ª expressao no R1
	fscanf(sint_out," %s\n", operador);	
	if(strcmp("{",operador)!= 0){
		printf("Erro: Faltando '{'.\n");
		exit(1);
	}
	int numeroInstrucoes = relacional(operadorRelacional);
	if(strcmp(instrucao2,"")!=0){
		sprintf(instrucao,"%s%d\n%s%d\n",instrucao1, numeroInstrucoes+4, instrucao2, numeroInstrucoes+3);
	}else{
		sprintf(instrucao,"%s%d\n",instrucao1, numeroInstrucoes+3);	
	}		
	imprime(instrucao,condicional);
	imprime(instrucoes, condicional);
	imprime("LDI R0, 0\n", condicional);
	pc++;
	pc++;
	sprintf(instrucao, "BRZ R0, -%d\n", pc-pcAtual);
	imprime(instrucao, condicional);
	
    }
	
    else {
	printf("ERRO: Operacao %s nao reconhecida.\n", operacao);
	exit(1);
    }
    
    
  }
return 0;

}

int relacional(char* operadorRelacional){

	if(strcmp(operadorRelacional,"==")==0){
		imprime("NOT R1, R1\n",condicional);
		pc++;
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);
		
		sprintf(instrucao1,"BRZ R0, ");
		sprintf(instrucao2,"");
		return numeroInstrucoes;
		
	}else if(strcmp(operadorRelacional,"!=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);
		
		sprintf(instrucao1,"BRZ R0, ");
		sprintf(instrucao2,"");
		return numeroInstrucoes;

	}else if(strcmp(operadorRelacional,">=")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);		
		
		sprintf(instrucao1,"BRN R0, ");
		sprintf(instrucao2,"");
		return numeroInstrucoes;

	}else if(strcmp(operadorRelacional,"<=")==0){
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);
		
		sprintf(instrucao1,"BRN R0, ");
		sprintf(instrucao2,"");
		return numeroInstrucoes;

	}else if(strcmp(operadorRelacional,">")==0){
		imprime("SUB R0, R0, R1\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);
		sprintf(instrucao1,"BRZ R0, ");
		pc++;
		sprintf(instrucao2,"BRN R0, ");
		return numeroInstrucoes;
	}else{
		imprime("SUB R0, R1, R0\n",condicional);
		pc++;
		int numeroInstrucoes = subrotina(1);
		sprintf(instrucao1,"BRZ R0, ");
		pc++;
		sprintf(instrucao2,"BRN R0, ");
		return numeroInstrucoes;
	}
}


void expressao(int tipoExpressao){
		
	fscanf(sint_out," %s\n", tipo);
	sinal = 0;	
	if(strcmp(tipo,"-")==0 | strcmp(tipo,"!")==0){
		sinal = 1;
		fscanf(sint_out," %s\n", tipo);	
	}		
	if(strcmp(tipo,"NUM")==0){
		carregaNumero("R1", sinal);
	}else if(strcmp(tipo,"VAR")==0){
		carregaVariavel("R1",sinal);
	}else if(strcmp(tipo,"(")==0){
		atribuicao_arit_aninhada(sinal);
		imprime("MV R1, R2\n",condicional);
		pc++;
	}else if(strcmp(tipo,"IO")==0){
		carregaIO("R1", sinal);

	}else{
		printf("ERRO: Codigo com erros, nao pode ser gerado.");
		exit(1);
	}
	fscanf(sint_out," %s\n", operador);

	if((strcmp(operador,"&")==0) | (strcmp(operador,"|")==0)){
		atribuicao_log(operador);
	}else if((strcmp(operador,"+")==0) | (strcmp(operador,"-")==0)){
		atribuicao_arit(operador);
	}else if(strcmp(operador,";")!=0 && tipoExpressao == 0 ){
		printf("ERRO: Faltando ';'\n");
		exit(1);
	}else if(tipoExpressao == 1){
		if((strcmp(operador,"==")==0) || (strcmp(operador,"!=")==0) || (strcmp(operador,">=")==0)
		|| (strcmp(operador,"<=")==0) || (strcmp(operador,">")==0) || (strcmp(operador,"<")==0)){
			sprintf(operadorRelacional,"%s",operador);
			imprime("MV R0, R1\n",condicional);	
			pc++;
		}else{
			printf("ERRO: Faltando operador relacional.\n");
			exit(1);
			
		}
	}else if(strcmp(")",operador)!=0 && tipoExpressao == 2){
		printf("ERRO: Faltando ')'.\n");
		exit(1);
	}else{
		if(tipoExpressao == 0){				// Expressao Atribuicao
			imprime("ST R0, R1\n",condicional);
			pc++;
		}
	}
}

void declaracao(){
	fscanf(sint_out," %s\n", variavel);
	fscanf(sint_out," %s\n", ptovirg);
	int j;
	if(indice >= 7){
		printf("ERRO: So e permitido declarar 7 variaveis.\n");
		exit(1);
	}
	
	if(retornaID(variavel)!=-1){
		printf("ERRO: Variavel %s ja declarada!\n", variavel);
                exit(1);
	}
        strcpy(ids[indice],variavel);
        indice+=1;
	
	if(strcmp(ptovirg,";")!=0){
		printf("ERRO: Faltando ';'\n");
		exit(1);
	}
}

void atribuicao_arit(char* operador){

	while ((strcmp(operador,"+")==0) || (strcmp(operador,"-")==0) ){
		fscanf(sint_out," %s\n", tipo);
		if(strcmp(tipo,"NUM")==0){
			carregaNumero("R2",0);
		}else if(strcmp(tipo,"VAR")==0){
			carregaVariavel("R2",0);
		}else if(strcmp(tipo,"IO")==0){
			carregaIO("R2", 0);
		}else if(strcmp(tipo,"(")==0){
			if((strcmp(operador,"+")==0)){
				atribuicao_arit_aninhada(2);
			}else{
				atribuicao_arit_aninhada(2);
			}
		}else{
			printf("ERRO: Codigo com erros, nao pode ser gerado.");
			exit(1);
		}
		if(strcmp(operador,"+")==0){
			imprime("ADD R1, R1, R2\n",condicional);
			pc++;
		}else{
			printf("SUB R1, R1, R2\n",condicional);
			pc++;
		}
		if(!feof(sint_out)){
			fscanf(sint_out," %s\n", operador);
		}else{
			printf("ERRO: Faltando ';'.\n");
			exit(1);
		}
	}
	
	if(strcmp(operador,";")==0){
		imprime("ST R0, R1\n",condicional);
		pc++;
	}else{
		printf("ERRO: Operador '%s' nao e valido.\n",operador);
		exit(1);
	}
    
}

void atribuicao_arit_aninhada(int sinal){
	sinalAninhado = 0;
	fscanf(sint_out," %s\n", tipo);
	if(strcmp("-",tipo)==0){
		sinalAninhado = 1;
		fscanf(sint_out," %s\n", tipo);
	}
	if(strcmp(tipo,"NUM")==0){
		carregaNumero("R2",sinalAninhado);
	}else if(strcmp(tipo,"VAR")==0){
		carregaVariavel("R2",sinalAninhado);
	}else if(strcmp(tipo,"IO")==0){
		carregaIO("R2", 0);
	}else{
		printf("ERRO: Codigo com erros, nao pode ser gerado.");
		exit(1);
	}
	fscanf(sint_out," %s\n", operadorAninhado);
	while ((strcmp(operadorAninhado,"+")==0) || (strcmp(operadorAninhado,"-")==0) ){
		fscanf(sint_out," %s\n", tipo);
		if(strcmp(tipo,"NUM")==0){
			carregaNumero("R3",0);
		}else if(strcmp(tipo,"VAR")==0){
			carregaVariavel("R3",0);
		}else if(strcmp(tipo,"IO")==0){
			carregaIO("R3", 0);
		}else{
			printf("ERRO: Codigo com erros, nao pode ser gerado.");
			exit(1);
		}
		if(strcmp(operadorAninhado,"+")==0){
			imprime("ADD R2, R2, R3\n",condicional);
			pc++;
		}else{
			imprime("SUB R2, R2, R3\n",condicional);
			pc++;
		}
		if(!feof(sint_out)){
			fscanf(sint_out," %s\n", operadorAninhado);
		}else{
			printf("ERRO: Faltando ')'.\n");
			exit(1);
		}
	}
	if(sinal == 1){
		imprime("NOT R2, R2\n",condicional);
		pc++;
	}
}

void carregaIO(char* reg, int sinal){
	imprime("LDI R3, 0\n",condicional);
	pc++;
 	imprime("DEC R3, R3\n",condicional);
	pc++; 			
	sprintf(instrucao,"LD %s, R3\n",reg);
	imprime(instrucao, condicional);
	pc++;
	if(sinal == 1){
		sprintf(instrucao, "NOT %s, %s\n", reg, reg);
		imprime(instrucao,condicional);	//Numero Negativo	
		pc++;
	}

}

void carregaNumero(char* reg, int sinal){
	fscanf(sint_out," %s\n", numero);
	if(atoi(numero) >= 0 && atoi(numero) <= 7){ 
		sprintf(instrucao, "LDI %s, %s\n", reg, numero);
		imprime(instrucao,condicional);
		pc++;
		if(sinal == 1){
			sprintf(instrucao, "NOT %s, %s\n", reg, reg);
			imprime(instrucao,condicional);	//Numero Negativo	
			pc++;
		}
	}else{
		printf("ERRO: Variavel %s fora do intervalo [-7:7].\n", numero);
		exit(1);
	}
}

void carregaVariavel(char* reg, int sinal){
	fscanf(sint_out," %s\n", variavel);

	if(retornaID(variavel)!= -1){
		int a = retornaID(variavel);
		sprintf(instrucao, "LDI R3, %d\n", a);
		imprime(instrucao,condicional);
		pc++;
		sprintf(instrucao, "LD %s, R3\n", reg);
		imprime(instrucao,condicional);
		pc++;
		if(sinal == 1){
			sprintf(instrucao, "NOT %s, %s\n", reg, reg);
			imprime(instrucao,condicional);	//Numero Negativo	
			pc++;
		}
	}

	else{
		printf("ERRO: Variavel %s nao existe.\n",variavel);
		exit(1);
	}
	
}

void atribuicao_log(char* operador){
	while((strcmp(operador,"&")==0) || (strcmp(operador,"|")==0) ){
		fscanf(sint_out," %s\n", tipo);
		if(strcmp(tipo,"!")==0){
			sinal = 1;
			fscanf(sint_out," %s\n", tipo);
		}else{
			sinal = 0;
		}			
		if(strcmp(tipo,"NUM")==0){
			carregaNumeroLog("R2",sinal);
		}else if(strcmp(tipo,"VAR")==0){
			carregaVariavelLog("R2",sinal);
		}else if(strcmp(tipo,"(")==0){
			atribuicao_log_aninhada(sinal);
		}else if(strcmp(tipo,"IO")==0){
			carregaIO("R2", sinal);
		}else{
			printf("ERRO: Codigo com erros, nao pode ser gerado.");
			exit(1);
		}
		if(strcmp(operador,"&")==0){
			imprime("AND R1, R1, R2\n",condicional);
			pc++;
		}else{
			imprime("OR R1, R1, R2\n",condicional);
			pc++;
		}
		fscanf(sint_out," %s\n", operador);
	}
	if(strcmp(operador,";")!=0){
		if(strcmp(operador,"")==0){
			printf("ERRO: Esta faltando ';'\n");
			exit(1);
		}else{	
			printf("ERRO: Operador '%s' nao e valido.\n",operador);
			exit(1);
		}
	}

	imprime("ST R0, R1\n",condicional);
	pc++;

}

void atribuicao_log_aninhada(int sinal){
	sinalAninhado = 0;
	fscanf(sint_out," %s\n", tipo);
	if(strcmp("-",tipo)==0){
		sinalAninhado = 1;
		fscanf(sint_out," %s\n", tipo);
	}
	if(strcmp(tipo,"NUM")==0){
		carregaNumeroLog("R2",sinalAninhado);
	}else if(strcmp(tipo,"VAR")==0){
		carregaVariavelLog("R2",sinalAninhado);
	}else if(strcmp(tipo,"IO")==0){
		carregaIO("R2", sinalAninhado);
	}else{
		printf("ERRO: Codigo com erros, nao pode ser gerado.");
		exit(1);
	}
	fscanf(sint_out," %s\n", operadorAninhado);
	while ((strcmp(operadorAninhado,"&")==0) || (strcmp(operadorAninhado,"|")==0) ){
		fscanf(sint_out," %s\n", tipo);
		if(strcmp(tipo,"!")==0){
			sinalAninhado = 1;
			fscanf(sint_out," %s\n", tipo);
		}else{
			sinalAninhado = 0;
		}
		if(strcmp(tipo,"NUM")==0){
			carregaNumeroLog("R3",sinalAninhado);
		}else if(strcmp(tipo,"VAR")==0){
			carregaVariavelLog("R3",sinalAninhado);
		}else if(strcmp(tipo,"IO")==0){
			carregaIO("R3", sinalAninhado);
		}else{
			printf("ERRO: Codigo com erros, nao pode ser gerado.");
			exit(1);
		}
		if(strcmp(operadorAninhado,"&")==0){
			imprime("AND R2, R2, R3\n",condicional);
			pc++;
		}else{
			imprime("OR R2, R2, R3\n",condicional);
			pc++;
		}
		if(!feof(sint_out)){
			fscanf(sint_out," %s\n", operadorAninhado);
		}else{
			printf("ERRO: Faltando ')'.\n");
			exit(1);
		}
	}
	if(sinal == 1){
		imprime("NOT R2, R2\n",condicional);
		pc++;
	}
}

void carregaNumeroLog(char* reg, int sinal){
	fscanf(sint_out," %s\n", numero);
	if(atoi(numero) >= 0 && atoi(numero) <= 7){
		sprintf(instrucao, "LDI %s, %s\n", reg, numero);
		imprime(instrucao,condicional);
		pc++;
		if(sinal==1){ 		//Numero Negativo
			sprintf(instrucao, "NOT %s, %s\n", reg, reg);
			imprime(instrucao,condicional);	//Numero Negativo	
			pc++;
		}
	}else{
		printf("ERRO: Valor %s fora do intervalo [-7:7].\n", numero);
		exit(1);	
	}
}

void carregaVariavelLog(char* reg, int sinal){
	fscanf(sint_out," %s\n", variavel);
	if(retornaID(variavel)!= -1){
		int a = retornaID(variavel);
		sprintf(instrucao, "LDI R3, %d\n", a);
		imprime(instrucao,condicional);
		pc++;
		sprintf(instrucao,"LD %s, R3\n",reg);
		imprime(instrucao,condicional);
		pc++;
		if(sinal == 1){					
			sprintf(instrucao, "NOT %s, %s\n", reg, reg);
			imprime(instrucao,condicional);	//Numero Negativo	
			pc++;
		}
	}

	else{
		printf("ERRO: Variavel %s nao existe.\n",variavel);
		exit(1);
	}
	
}

int retornaID(char* variavel){
	int j;
	for(j=0;j<indice+1;j++) {
        	if(strcmp(ids[j], variavel) == 0) {
			return j;
		}
	}
	return -1;
}

void imprime(char* string, int cond){
	if(cond == 0){
		printf("%s",string);
	}else{
		sprintf(instrucoes,"%s%s",instrucoes,string);
	}
}
