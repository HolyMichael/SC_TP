#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helper.h"


//Estruturas
typedef struct EVENTO{

int tipo; // ChegadaCliente(0), FimZonaVendedor(1), FimZonaPagamento(2), FimZonaLevantamento(3)
int tempoOcorrencia;
int posto;
struct EVENTO *nseg;
struct EVENTO *nant;
}Evento;



typedef struct Cliente{

int prioridade; //Prioridade: geral(0),retomado(1), prioritario(2)
int tempoComeco;//Tempo em que o cliente chega ao sistema 

}Cliente;


//Funções
void setup();
void makeEvent(int tipo,int tempo,int posto);
void inserirEventoInicio( Evento*nv);
void inserirEventoFim( Evento *nv);
void inserirEventoOrdem(Evento*nv);
void imprimeLista();
void processarEventoChegada();
void temp();
void processarEventoSaidaVendedores();
void makeEventVendedoresSaida(int posto);
void makeEventPagamentoSaida(int posto);


//Variaveis Globais

//alterar número de clientes aqui
 #define minClients 2850
 #define maxClients 2851
int cli;
 int probPrioritario = 5;

//percentagem de clients por hora (Entrada de Clientes)
int percentagem1 = 99; //10-13h
int percentagem2 = 1; //13-16h
int percentagem3 = 15; //16-19h
//  percentagem 4 é o restante para 100

int relogio=0;
Evento*evento=NULL;

//Postos Vendedores
struct Cliente *vendedoresPostos [10] = {NULL};
struct Cliente *vendedoresFila[maxClients][10]= {NULL};

//Postos Pagamento 
struct Cliente *pagamentoPostos[4]={NULL};
struct Cliente *pagamentoFila[maxClients];

int main() {
	int tipoEvento;
	int eventosprocessados=0;
	srand(time(NULL));
	setup();
    imprimeLista();
	printf("\n\n\n");
	while(evento!=NULL){
		tipoEvento=evento->tipo;
		relogio=evento->tempoOcorrencia;
		eventosprocessados++;
		switch(tipoEvento){
			//Chegada Cliente
			case 0: processarEventoChegada();
					//temp();
					break; 
			//Fim Zona Vendedores
			case 1: 
				processarEventoSaidaVendedores();
				break;
			//Fim Zona Pagamento
			case 2:
				;
				break;
			//Zona de Levantamento
			case 3:
				;
				break;
	   }
		printf("\nEVENTO: %d\n", tipoEvento);
		printf("\nNUM_EVENTOS: %d\n", eventosprocessados);
		printf("\nNUM_CLIENTES: %d\n", cli);
		//temp();
	 evento=evento->nseg;
	}

	printf("\nCHEGAMOS AO FIM BOIISS  %d\n", eventosprocessados);
	imprimeLista();
	temp();
}

void makeEventPagamentoSaida(int posto){
	int tempe,tempo; 
	tempe= rand() % 101;

	if( tempe <11)
		tempo= rand () % 60;
	if(tempe > 11 && tempe < 36)
		tempo= rand() % 60+60;
	if(tempe > 35 && tempe < 76)
		tempo= rand() % 60+120;
	if (tempe > 75)
		tempo = rand() % 60+180;
	

	tempo +=relogio;
    
    makeEvent(2,tempo,posto);
	printf("\n\n %d clock:%d\n",tempo,relogio);	
	//imprimeLista();

}

void processarEventoSaidaVendedores(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
  	
    // Clientes que compram
	if(prob<2){
		for(i=0;i<10;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				makeEventPagamentoSaida(i);
				pagamentoPostos[i] = vendedoresPostos[posto];
				break;
			}
    	}
	}

	// Limpeza de filas e postos. Quem nao compra vai embora.
    printf("%p",vendedoresFila[0][posto]);
	if(vendedoresFila[0][posto]==NULL){
		vendedoresPostos[posto]=NULL;
		printf("VAMO RETORNAR GALERA");
		return;
	}
	vendedoresPostos[posto]=vendedoresFila[0][posto];
	
    for(i=1; i< maxClients/10; i++){
		vendedoresFila[i-1][posto]=vendedoresFila[i][posto];
		if(vendedoresFila[i][posto]==NULL){
			printf("\ncaso especail\n");
			vendedoresFila[i-1][posto]=NULL;
			break;
		}	
	}

	makeEventVendedoresSaida(posto);
}


void makeEventVendedoresSaida(int posto){
	int tempe,tempo; 
	tempe= rand() % 101;

	if( tempe < 11)
		tempo= rand () % 301;
	if(tempe > 10 && tempe < 36)
		tempo= rand() % 301 +300;
	if(tempe > 35 && tempe < 76)
		tempo= rand() % 301+ 600;
	if (tempe > 75 && tempe < 96)
		tempo = rand() % 301 +900;
	if(tempe > 95)
		tempo = rand () % 2400 +1200;

	tempo +=relogio;
    
    makeEvent(1,tempo,posto);
	printf("\n\n %d clock:%d\n",tempo,relogio);	
	//imprimeLista();

}



void processarEventoChegada(){
	int i=0,j=0;
	Cliente cliente;
    //Gerar numero clientes prioritarios

	if(rand() % + 101 < (probPrioritario+1)){
		cliente.prioridade=2;
	}
	else
		cliente.prioridade=0;

	//2 filas prioritarias:8 e 9. Clientes gerais: Escolher onde ha menos gente. Se iguais, nao prioritaria.
	for(i=0;i<10;i++){
		//Ocupar posto, se existir
		if(vendedoresPostos[i]==NULL){
			makeEventVendedoresSaida(i);
			vendedoresPostos[i] = &cliente;
			return;
		}
    }

	//Qdo os postos estao cheios, colocar o cliente na fila otima (Escolher onde ha menos gente. Se iguais, nao prioritaria.)		
	int postos[10][2]= {0};
	int minimoTemp=maxClients, count = 0;
	//Quantas pessoas estão em cada fila
	for(i=0;i<10;i++){
		for(j=0;j<maxClients;j++){
			if(vendedoresFila[i][j]!=NULL)
				count++;
			else
				break;
		}
		//printf("\n%d pessoas\n",j);
		//Escolher o cjto de filas com menos pessoas,mas com # igual
		if(count<minimoTemp){
			//printf("\nsim");
			memset(postos,0,sizeof postos);
			//printf(" %d o i",i);
			postos[i][0]=1; // o i[][0] representa se a fila é escolhida(1) ou não (0)
			postos[i][1]=j; // o i[][1] representa a qtd de pessoas na fila
			minimoTemp=count;
		}if(count==minimoTemp){
			//printf("\nmais ou menos");
			postos[i][0]=1;
		}
		//printf("%d o i",i);
		count=0;
	} 
	
	
	//Escolher fila nao prioritaria
	for(i = 0;i<10;i++){
		//printf("\n%d  %d\n", postos[0][1], postos[0][0]);
		if(postos[i][0]==1){
			//printf("\n I=%d ",i);
			vendedoresFila[i][(postos[i][1])]=&cliente;
			//printf("\n \n \n %d %d\n \n \n ", i, (postos[i][1]));
			break;
		}
	}
	
	
}

void setup(){
	//geração do número de clientes de minClients-maxClients
	int numberOfClients=minClients + rand() % (maxClients-minClients);
	int clientsLeft = numberOfClients; //numero de clientes ainda não atribuidos numa timezone
	printf("%d # de clientes", numberOfClients);
	cli=numberOfClients;
	//clientes por período de tempo
	int clientesTempo1 = numberOfClients*((float)percentagem1/(float)100.0); //10-13h
	clientsLeft-= clientesTempo1;
	int clientesTempo2 = numberOfClients*((float)percentagem2/(float)100.0); //13-16h
	clientsLeft-= clientesTempo2;
	int clientesTempo3 = numberOfClients*((float)percentagem3/(float)100.0); //16-19h
	clientsLeft-= clientesTempo3;
 	int clientesTempo4 = clientsLeft;                  						 //19-22h
 	
 	printf("\n%d , %d , %d , %d total: %d\n", clientesTempo1, clientesTempo2, clientesTempo3, clientesTempo4,
	  clientesTempo1+clientesTempo2+clientesTempo3+clientesTempo4);
	
	//gerar os eventos de chegada de clientes
	int temp;
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 10800 + 0;
		makeEvent( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo2; temp++){
		int tempo = rand() % 10800 + 10800;
		makeEvent( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo3; temp++){
		int tempo = rand() % 10800 + 21600;
		makeEvent( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo4; temp++){
		int tempo = rand() % 10800 + 43200;
		makeEvent( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	
	printf("\n\n\n");
}

//Criar Evento 
void makeEvent(int tipo,int tempo,int posto){

	Evento *nv=(Evento*) malloc(1*sizeof(Evento));
	nv->tempoOcorrencia=tempo; 
	nv->tipo=tipo;
	nv->posto=posto;
	nv->nseg=NULL;
	
	
	inserirEventoOrdem(nv);
}

//Inserir Evento no Inicio da Lista
void inserirEventoInicio( Evento*nv){
		nv->nseg=evento;
		nv->nant=NULL;
		if(evento!=NULL)
			evento->nant=nv;
		evento= nv;
}

//Inserir Evento no Fim da Lista
void inserirEventoFim(Evento *nv){

	Evento *aux=evento;

	if(aux==NULL){
		inserirEventoInicio(nv);
		return;
    }

	while(aux->nseg!=NULL)
		aux=aux->nseg;

	aux->nseg=nv;
	nv->nant=aux;

	

}

//Inserir Evento na Lista de Eventos tendo em conta o tempo de Ocorrencia(key)
void  inserirEventoOrdem( Evento *nv){

	Evento *aux= evento;

    if(evento==NULL){
		inserirEventoInicio(nv);
		return;
    }
	while(aux!=NULL){
		if(aux->tempoOcorrencia > nv->tempoOcorrencia){
			break;
			
		}
		aux=aux->nseg;
     }
	
	if(aux==NULL){
		inserirEventoFim(nv);
		return;
	}


	if(aux->nant==NULL){
		inserirEventoInicio(nv);
		return;
	}

	nv->nseg=aux;
	nv->nant=aux->nant;
	aux->nant=nv;
	nv->nant->nseg=nv;

}


void imprimeLista(){


	Evento *aux=evento;

	if(aux==NULL)
		printf("Vazio");
		while(aux!=NULL){
			printf("%d,%d,%d-> ",aux->tipo,aux->tempoOcorrencia,aux->posto);
			aux=aux->nseg;
		}
	printf("\n");
}


void temp(){
int i,j;
	for(i=0;i<10;i++){
		if(vendedoresPostos[i]==NULL)
				printf("|0|");
		else
			printf("|1|");
	}
	printf("\n\n");
	for(i=0;i<10;i++){
		for(j=0;j<maxClients;j++){
			if(vendedoresFila[i][j]==NULL){
				printf("|0|");
				break;
				}
			else
				printf("|1|");
		}
	printf("\n");
	}	
	printf("\n\naaaaaaaaaaaaaaaaaaaaaaaa");
}
	
	

	


