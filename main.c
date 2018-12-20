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

int prioridade; //Prioridade: geral(0),retomado(1), prioritÃ¡rio(2)
int tempoComeco;//Tempo em que o cliente chega ao sistema 

}Cliente;


//Funções
void setup();
void makeEventChegada(int tipo,int tempo,int posto);
void inserirEventoInicio( Evento*nv);
void inserirEventoFim( Evento *nv);
void inserirEventoOrdem(Evento*nv);
void imprimeLista();




//alterar número de clientes aqui
int minClients = 300;
int maxClients = 320;

//percentagem de clients por hora
int percentagem1 = 10; //10-13h
int percentagem2 = 30; //13-16h
int percentagem3 = 15; //16-19h
//  percentagem 4 é o restante para 100

Evento*evento=NULL;

int main() {
	srand(time(NULL));
	setup();
	
}

void setup(){
	//geração do número de clientes de minClients-maxClients
	int numberOfClients=minClients + rand() % (maxClients-minClients);
	int clientsLeft = numberOfClients; //numero de clientes ainda não atribuidos numa timezone
	printf("%d # de clientes", numberOfClients);
	
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
		makeEventChegada( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo2; temp++){
		int tempo = rand() % 10800 + 10800;
		makeEventChegada( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo3; temp++){
		int tempo = rand() % 10800 + 21600;
		makeEventChegada( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	for(temp=0 ; temp < clientesTempo4; temp++){
		int tempo = rand() % 10800 + 43200;
		makeEventChegada( 0, tempo, -1);
		printf("%d, ",tempo);
	}
	
	printf("\n\n\n");
}

//Criar Evento 
void makeEventChegada(int tipo,int tempo,int posto){

	Evento *nv=(Evento*) malloc(1*sizeof(Evento));
	nv->tempoOcorrencia=tempo; 
	nv->tipo=tipo;
	nv->posto=posto;
	nv->nseg=NULL;
	
	
	inserirEventoOrdem(nv);
	imprimeLista();
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
			printf("%d-> ",aux->tempoOcorrencia);
			aux=aux->nseg;
		}
	printf("\n");
}




