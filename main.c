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


//Fun��es
void setup();
void makeEvent(int tipo,int tempo,int posto);
void inserirEventoInicio( Evento*nv);
void inserirEventoFim( Evento *nv);
void inserirEventoOrdem(Evento*nv);
void processarEventoChegada();
void processarEventoSaidaVendedores();
void processarEventoSaidaPagamento();
void processarEventoLevantamentoPagamento();
void makeEventVendedoresSaida(int posto);
void makeEventPagamentoSaida(int posto);
void makeEventLevantamentoSaida(int posto);
void imprimeLista();
void imprimeFilaVendedores();
void imprimeFilaPagamento();
void imprimeFilaLevantamento();


//Variaveis Globais

//alterar n�mero de clientes aqui
 #define minClients 300
 #define maxClients 320
int cli;
int probPrioritario = 5;
int retornados=0;
int saidavendedores=0;
//percentagem de clients por hora (Entrada de Clientes)
int percentagem1 = 10; //10-13h
int percentagem2 = 30; //13-16h
int percentagem3 = 15; //16-19h
//  percentagem 4 � o restante para 100

int relogio=0;
Evento*evento=NULL;

//Postos Vendedores
struct Cliente *vendedoresPostos [10] = {NULL};
struct Cliente *vendedoresFila[maxClients][10]= {NULL};

//Postos Pagamento 
struct Cliente *pagamentoPostos[4]={NULL};
struct Cliente *pagamentoFila[maxClients] = {NULL};

//Postos Levantamento 
struct Cliente *levantamentoPostos[2]={NULL};
struct Cliente *levantamentoFila[maxClients] = {NULL};

int main() {
	int tipoEvento;
	int eventosprocessados=0;
	int eventoschegada=0;
	int eventospagamento=0;
	int eventosvendedor=0;
	int eventoslevantamento=0;
	srand(time(NULL));
	setup();
	saidavendedores=cli;
    imprimeLista();
    printf("\nNUM_CLIENTES: %d", cli);
	printf("\n\n\n");
	while(evento!=NULL){
		tipoEvento=evento->tipo;
		relogio=evento->tempoOcorrencia;
		eventosprocessados++;
		printf("\n------ UM NOVO EVENTO ------\n");
		switch(tipoEvento){
			case 0:
				printf("EVENTO_TIPO: 0 CHEGADA CLIENTE\n");
				break;
			case 1:
				printf("EVENTO_TIPO: 1 SAIDA VENDEDORES\n");
				break;
			case 2:
				printf("EVENTO_TIPO: 2 SAIDA PAGAMENTO\n");
				break;
			case 3:
				printf("EVENTO_TIPO: 3 SAIDA LEVANTAMENTO\n");
				break;
		}
		printf("NUM_EVENTOS: %d\n", eventosprocessados);
		printf("CLOCK_ATUAL: %d\n",relogio);
		switch(tipoEvento){
			//Chegada Cliente
			case 0:
				processarEventoChegada();
				imprimeFilaVendedores(); //flush it to file
				eventoschegada++;
				break; 
			//Fim Zona Vendedores
			case 1:
				processarEventoSaidaVendedores();
				imprimeFilaVendedores();
				imprimeFilaPagamento();
				eventosvendedor++;
				break;
			//Fim Zona Pagamento
			case 2:
				processarEventoSaidaPagamento();
				imprimeFilaPagamento();
				imprimeFilaLevantamento();
				eventospagamento++;
				break;
			//Zona de Levantamento
			case 3:
				processarEventoSaidaLevantamento();
				imprimeFilaLevantamento();
				eventoslevantamento++;
				break;
	   }
	 evento=evento->nseg;
	}
	printf("-------   FIM   -------");
	imprimeFilaVendedores();
	imprimeFilaPagamento();
	imprimeFilaLevantamento();
	printf("\n\n %d clientes, e %d eventos processados\n", cli, eventosprocessados);
	printf("\n Eventos de chegada            : %d",eventoschegada);
	printf("\n Eventos de saida vendedores   : %d",eventosvendedor);
	printf("\n Eventos de saida pagamento    : %d",eventospagamento);
	printf("\n Eventos de saida levantamento : %d\n",eventoslevantamento);
	printf("\n RETORNADOS                    : %d", retornados);
	printf("\n SAIDAS NOS VENDEDORES         : %d", saidavendedores);
	imprimeLista();
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
	printf("\n Novo Evento gerado:\n ETIPO:2\n TEMPO:%d\n",tempo);	
	//imprimeLista();

}

void processarEventoSaidaLevantamento(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
  	
    // Clientes que regressam ao pagamento
	if(prob<6){
		retornados++;
		int flag=0;
		for(i=0;i<4;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				makeEventPagamentoSaida(i);
				pagamentoPostos[i] = levantamentoPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso n�o exista posto livre ocupar fila
    	int i;
    	for(i=0;i<maxClients;i++){
    		if(pagamentoFila[i]==NULL && flag == 0){
    			pagamentoFila[i] = levantamentoPostos[posto];
    			break;
    		}
    	}
	}

	// Limpeza de filas e postos. Quem nao compra vai embora.
	if(levantamentoFila[0]==NULL){
		levantamentoPostos[posto]=NULL;
		return;
	}
	
	//mover o cliente da fila para o atendimento e fazer o seu evento
	levantamentoPostos[posto]=levantamentoFila[0];
	makeEventLevantamentoSaida(posto);
	
    for(i=1; i<maxClients/10; i++){
		levantamentoFila[i-1]=levantamentoFila[i];
		if(levantamentoFila[i]==NULL){
			levantamentoFila[i-1]=NULL;
			break;
		}	
	}
}

void processarEventoSaidaVendedores(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
  	
    // Clientes que compram
	if(prob<81){
		saidavendedores--;
		int flag=0;
		for(i=0;i<4;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				makeEventPagamentoSaida(i);
				pagamentoPostos[i] = vendedoresPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso n�o exista posto livre ocupar fila
    	int i;
    	for(i=0;i<maxClients;i++){
    		if(pagamentoFila[i]==NULL && flag == 0){
    			pagamentoFila[i] = vendedoresPostos[posto];
    			break;
    		}
    	}
	}

	// Limpeza de filas e postos. Quem nao compra vai embora.
	if(vendedoresFila[0][posto]==NULL){
		vendedoresPostos[posto]=NULL;
		return;
	}
	vendedoresPostos[posto]=vendedoresFila[0][posto];
	
    for(i=1; i< maxClients/10; i++){
		vendedoresFila[i-1][posto]=vendedoresFila[i][posto];
		if(vendedoresFila[i][posto]==NULL){
			vendedoresFila[i-1][posto]=NULL;
			break;
		}	
	}
	//criamos o evento do cliente agora a ser atendido no posto livre.
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
	printf("\n Novo Evento gerado:\n ETIPO:1\n TEMPO:%d\n",tempo);

}

void makeEventLevantamentoSaida(int posto){
	int tempe,tempo; 
	tempe= rand() % 101;

	if( tempe < 31)
		tempo= rand () % 301;
	if(tempe > 30 && tempe < 71)
		tempo= rand() % 301 +300;
	if(tempe > 70 && tempe < 96)
		tempo= rand() % 301+ 600;
	if(tempe > 95)
		tempo = rand () % 301 + 900;

	tempo +=relogio;
    
    makeEvent(3,tempo,posto);
	printf("\n Novo Evento gerado:\n ETIPO:3\n TEMPO:%d\n",tempo);

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
	//Quantas pessoas est�o em cada fila
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
			postos[i][0]=1; // o i[][0] representa se a fila � escolhida(1) ou n�o (0)
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

void processarEventoSaidaPagamento(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
	
	// Clientes que levantam
	if(prob<61){
		int flag=0;
		for(i=0;i<2;i++){
		//Ocupar posto, se existir
			if(levantamentoPostos[i]==NULL){
				makeEventLevantamentoSaida(i); //LEVANTAMENTO!
				levantamentoPostos[i] = pagamentoPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso n�o exista posto livre ocupar fila
    	int i;
    	for(i=0;i<maxClients;i++){
    		if(levantamentoFila[i]==NULL && flag == 0){
    			levantamentoFila[i] = pagamentoPostos[posto];
    			break;
    		}
    	}
	}
	
	
	// Limpeza de filas e postos. Quem nao compra vai embora.
	//Fila de pagamento vazia
	if(pagamentoFila[0]==NULL){
		pagamentoPostos[posto]=NULL;
		return;
	}
	//mover o primeiro cliente na fila para o posto
	pagamentoPostos[posto]=pagamentoFila[0];
	//criamos o evento do cliente agora a ser atendido no posto livre.
	makeEventPagamentoSaida(posto);
	
	//mover toda a fila 1 para a frente
    for(i=1; i< maxClients; i++){
		pagamentoFila[i-1]= pagamentoFila[i];
		if(pagamentoFila[i]==NULL){
			pagamentoFila[i-1]=NULL; //fazemos null a ultima posi��o pois trata-se de um cliente duplicado
			break;
		}	
	}
}

void setup(){
	//gera��o do n�mero de clientes de minClients-maxClients
	int numberOfClients=minClients + rand() % (maxClients-minClients);
	int clientsLeft = numberOfClients;
	printf("%d # de clientes", numberOfClients);
	cli=numberOfClients;
	//clientes por per�odo de tempo
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
	printf("\nLista de eventos:");
	if(aux==NULL)
		printf("Vazio");
		while(aux!=NULL){
			printf("%d,%d,%d-> ",aux->tipo,aux->tempoOcorrencia,aux->posto);
			aux=aux->nseg;
		}
	printf("\n");
}

void imprimeFilaLevantamento(){
	int i;
	printf("\n\nFila de Levantamento\n");
	printf("Postos: %i | %i\n", (levantamentoPostos[0]!=NULL),(levantamentoPostos[1]!=NULL));
	printf("Filas :");
	for(i = 0; i< maxClients ;i++){
		if(levantamentoFila[i]!=NULL)
			printf("| 1");
		else break;
			
	}
	printf("\n");
}

void imprimeFilaPagamento(){
	int i;
	printf("\n\nFila de pagamento\n");
	printf("Postos: %i | %i | %i | %i\n", (pagamentoPostos[0]!=NULL),(pagamentoPostos[1]!=NULL),
		(pagamentoPostos[2]!=NULL),(pagamentoPostos[3]!=NULL));
	printf("Filas :");
	for(i = 0; i< maxClients ;i++){
		if(pagamentoFila[i]!=NULL)
			printf("| 1");
		else break;
			
	}
	printf("\n");
}
void imprimeFilaVendedores(){
	int i,j;
	int finish=0;
	printf("\n\nFila de vendedores\n");
	printf("Postos|");
	for(i=0;i<10;i++){
		if(vendedoresPostos[i]==NULL)
				printf("|0|");
		else
			printf("|1|");
	}
	printf("\nFILAS |");
	for(j=0;j<maxClients;j++){
		for(i=0;i<10;i++){
			if(vendedoresFila[i][j]==NULL){
				printf("|0|");
				finish++;
			}
			else
				printf("|1|");
		}
	if(finish<20)
	printf("\n      |");
	else return;
	}	
	printf("\n");
}
	
	

	


