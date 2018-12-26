#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helper.h"


//Estruturas
typedef struct EVENTO{

int tipo; // ChegadaCliente(0), FimZonaVendedor(1), FimZonaPagamento(2), FimZonaLevantamento(3), FimPeriodoContagem(4)
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
void processarEventoChegada();
void processarEventoSaidaVendedores();
void processarEventoSaidaPagamento();
void processarEventoSaidaLevantamento();
void makeEventVendedoresSaida(int posto);
void makeEventPagamentoSaida(int posto);
void makeEventLevantamentoSaida(int posto);
void imprimeLista();
void imprimeFilaVendedores();
void imprimeFilaPagamento();
void imprimeFilaLevantamento();


//Variaveis Globais

//alterar número de clientes aqui
 #define minClients 300
 #define maxClients 320
int cli;
int probPrioritario = 50;
int retornados=0;
int saidavendedores=0;
//percentagem de clients por hora (Entrada de Clientes)
int percentagem1 = 100; //10-13h
int percentagem2 = 0; //13-16h
int percentagem3 = 0; //16-19h
//  percentagem 4 é o restante para 100

//Variavel estatisticas
int tempos[3][4] = {0};
int zonaTempoAtual= 0;
int clientesZonaTempo[4];
int clientescolocadosprioridade=0;

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
	int eventospassagem=0;
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
				printf("EVENTO_TIPO: 1 SAIDA VENDEDORES");
				printf(" %d \n",evento->posto);
				break;
			case 2:
				printf("EVENTO_TIPO: 2 SAIDA PAGAMENTO\n");
				break;
			case 3:
				printf("EVENTO_TIPO: 3 SAIDA LEVANTAMENTO\n");
				break;
			case 4:
				printf("EVENTO_TIPO: 4 PASSAGEM ZONA TEMPORAL\n");
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
				//imprimeFilaLevantamento();
				eventospagamento++;
				break;
			//Zona de Levantamento
			case 3:
				processarEventoSaidaLevantamento();
				//imprimeFilaLevantamento();
				eventoslevantamento++;
				break;
			case 4:
				eventospassagem++;
				zonaTempoAtual++;
				printf("ESTAMOS AGORA NA ZONA %d", zonaTempoAtual);
				break;
			default:
				return;
				break;
	   }
	 evento=evento->nseg;
	}
	printf("-------   FIM   -------");
	imprimeFilaVendedores();
	imprimeFilaPagamento();
	imprimeFilaLevantamento();
	printf("\n\n %d clientes, e %d eventos processados\n", cli, eventosprocessados);
	printf("\nEventos de chegada            : %d",eventoschegada);
	printf("\nEventos de saida vendedores   : %d",eventosvendedor);
	printf("\nEventos de saida pagamento    : %d",eventospagamento);
	printf("\nEventos de saida levantamento : %d\n",eventoslevantamento);
	printf("\nRETORNADOS                    : %d", retornados);
	printf("\nSAIDAS NOS VENDEDORES         : %d", saidavendedores);
	printf("\nEVENTOS PASSAGEM              : %d\n", eventospassagem);
	int i;
	for(i=0;i<4;i++){
		printf("CLIENTES ZONA TEMPORAL %d    : %d\n",i+1,clientesZonaTempo[i]);
	}
	
	printf("\nCLIENTES COLOCADOS POR PRIORIDADE: %d",clientescolocadosprioridade);
	printf("\n ESPERAS                       :");
	printf("\n10h-13h:");
	printf("\nFASE DE VENDEDORES             : %d",tempos[0][0]/clientesZonaTempo[0]);
	printf("\nFASE DE PAGAMENTO              : %d",tempos[1][0]/clientesZonaTempo[0]);
	printf("\nFASE DE LEVANTAMENTO           : %d",tempos[2][0]/clientesZonaTempo[0]);
	printf("\n13h-16h:");
	printf("\nFASE DE VENDEDORES             : %d",tempos[0][1]/clientesZonaTempo[1]);
	printf("\nFASE DE PAGAMENTO              : %d",tempos[1][1]/clientesZonaTempo[1]);
	printf("\nFASE DE LEVANTAMENTO           : %d",tempos[2][1]/clientesZonaTempo[1]);
	printf("\n16h-19h:");
	printf("\nFASE DE VENDEDORES             : %d",tempos[0][2]/clientesZonaTempo[2]);
	printf("\nFASE DE PAGAMENTO              : %d",tempos[1][2]/clientesZonaTempo[2]);
	printf("\nFASE DE LEVANTAMENTO           : %d",tempos[2][2]/clientesZonaTempo[2]);
	printf("\n19h-22h:");
	printf("\nFASE DE VENDEDORES             : %d",tempos[0][3]/clientesZonaTempo[3]);
	printf("\nFASE DE PAGAMENTO              : %d",tempos[1][3]/clientesZonaTempo[3]);
	printf("\nFASE DE LEVANTAMENTO           : %d",tempos[2][3]/clientesZonaTempo[3]);
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
	tempo=10000; //AQUI

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
    	//caso não exista posto livre ocupar fila
    	int i;
    	for(i=0;i<maxClients;i++){
    		if(pagamentoFila[i]==NULL && flag == 0){
    			pagamentoFila[i] = levantamentoPostos[posto];
    			pagamentoFila[i]->tempoComeco = relogio;
    			break;
    		}
    	}
	}

	// Limpeza de filas e postos. Quem nao compra vai embora.
	int flag=0;
	if(levantamentoFila[0]==NULL){
		levantamentoPostos[posto]=NULL;
		flag=1;
	}
	
	if(flag==0){
		//mover o cliente da fila para o atendimento e fazer o seu evento
		levantamentoPostos[posto]=levantamentoFila[0];
		makeEventLevantamentoSaida(posto);
		//atualizar variaveis estatisticas
		if(levantamentoPostos[posto]->tempoComeco!=0){
	  		tempos[2][zonaTempoAtual]+=(relogio - levantamentoPostos[posto]->tempoComeco);
	  		levantamentoPostos[posto]->tempoComeco=0;	
	  	}
	
		
	    for(i=1; i<maxClients; i++){
			levantamentoFila[i-1]=levantamentoFila[i];
			if(levantamentoFila[i]==NULL){
				levantamentoFila[i-1]=NULL;
				break;
			}	
		}
	}
}

void processarEventoSaidaVendedores(){
	int i,j;
	int posto=evento->posto;
	int prob= rand() % 101;
	
    // Clientes que compram
	if(prob<81){
		saidavendedores--;
		int flag=0;
		for(i=0;i<4;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				printf("\nCLIENTE COLOCADO EM POSTO");
				makeEventPagamentoSaida(i);
				pagamentoPostos[i] = vendedoresPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso não exista posto livre ocupar fila
    	if(flag==0){
	    	printf("\nCLIENTE COLOCADO EM FILA");
	    	//caso prioritario
	    	if(vendedoresPostos[posto]->prioridade==2){
	    		//selecionar o lugar correcto
	    		int lugar;
	    		for(i=0;i<maxClients;i++){
	    			if(pagamentoFila[i]==NULL || pagamentoFila[i]->prioridade==0){
	    				lugar=i;
	    				break;
	    			}
	    		}
	    		printf(" POR PRIORIDADE NO LUGAR %d",i+1);
	    		//mover os clientes na fila para trás
				for(j=maxClients;j>lugar;j--){
					if(pagamentoFila[j]==NULL){
						continue;
					}
					pagamentoFila[j+1]=pagamentoFila[j];
				}
				//colocar o cliente no seu lugar
				pagamentoFila[lugar]=vendedoresPostos[posto];
	    	}
	    	else{
	    	//caso nao prioritario
		    	int i;
		    	printf(" SEM PRIORIDADE");
		    	for(i=0;i<maxClients;i++){
		    		if(pagamentoFila[i]==NULL && flag == 0){
		    			//inicializar o novo tempo
		    			vendedoresPostos[posto]->tempoComeco=relogio;
		    			pagamentoFila[i] = vendedoresPostos[posto];
		    			break;
		    		}
		    	}
		    }
		}
	}
	
	int flag=0;
	if(vendedoresFila[0][posto]==NULL){
		printf("\nnao ha ninguem na fila\n");
		vendedoresPostos[posto]=NULL;
		flag=1;
	}
	
	//atualizar variaveis estatisticas
	if(flag==0){
	    //ir buscar o próximo cliente em fila
	    vendedoresPostos[posto]=vendedoresFila[0][posto];
		if(vendedoresPostos[posto]->tempoComeco!=0){
	  		tempos[0][zonaTempoAtual]+=(relogio - vendedoresPostos[posto]->tempoComeco);
	  		vendedoresPostos[posto]->tempoComeco=0;
	  	}
	  		
	    for(i=1; i< maxClients; i++){
			vendedoresFila[i-1][posto]=vendedoresFila[i][posto];
			if(vendedoresFila[i][posto]==NULL){
				vendedoresFila[i-1][posto]=NULL;
				break;
			}
		}
		//criamos o evento do cliente agora a ser atendido no posto livre.
		makeEventVendedoresSaida(posto);
	}
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
	Cliente *cliente=(Cliente*) malloc(1*sizeof(Cliente));
    //Gerar numero clientes prioritarios
	cliente->tempoComeco=0;
	if(rand() % + 101 < (probPrioritario+1)){
		cliente->prioridade=2;
		printf("\n CLIENTE PRIORITARIO");
	}
	else{
		cliente->prioridade=0;
		printf("\n CLIENTE NAO PRIORITARIO");
	}
	//2 filas prioritarias:8 e 9. Clientes gerais: Escolher onde ha menos gente. Se iguais, nao prioritaria.
	for(i=0;i<10;i++){
		//Ocupar posto, se existir
		if(vendedoresPostos[i]==NULL){
			printf("\n CLIENTE COLOCADO NO POSTO %d",i);
			makeEventVendedoresSaida(i);
			vendedoresPostos[i] = cliente;
			return;
		}
    }

	printf("\nA COLOCAR CLIENTE EM FILA");
	//Qdo os postos estao cheios, colocar o cliente na fila otima	
	//caso cliente prioritario
	if(cliente->prioridade==2){ //Escolher a fila prioritaria com menos pessoas prioritarias em fila
		printf("\nCliente colocado por prioridade");
		int filasprioridade[2]={0}; //# de clientes prioritarios nas filas prioritarias
		for(i=0;i<2;i++){
			for(j=0;j<maxClients;j++){
				if(vendedoresFila[j][i+8]==NULL || vendedoresFila[j][i+8]->prioridade==0){
					filasprioridade[i]=j;
					printf("|%d",filasprioridade[i]);
					break;
				}
			}
		}
		int filaMin;
		int fila;
		if(filasprioridade[0]<filasprioridade[1]){
			filaMin=filasprioridade[0];
			fila=8;
		}
		else{
			filaMin=filasprioridade[1];
			fila=9;
		}
		//mover os clientes na fila para trás
		for(j=maxClients;j>filaMin;j--){
			if(vendedoresFila[j][i]==NULL){
				continue;
			}
			vendedoresFila[j+1][fila]=vendedoresFila[j][fila];
		}
		cliente->tempoComeco=relogio;
		vendedoresFila[filaMin][fila]=cliente;
		clientescolocadosprioridade++;
	}
	//caso cliente nao prioritario
	else{ //escolher a fila com menos pessoas em fila e entre as que teem menos pessoas escolher fila nao prioritaria
		printf("\nCLIENTE NAO PRIORITARIO");
		int filas[10]={0};
		for(i=0;i<10;i++){
			for(j=0;j<maxClients;j++){
				if(vendedoresFila[j][i]==NULL){
					filas[i]=j;
					printf("|%d",filas[i]);
					break;
				}
			}
		}
		
		int filaMin=maxClients;
		for(i=0;i<10;i++){
			if(filas[i]<filaMin)
				filaMin=filas[i];
		}
		printf("\n FILA MINIMA=%d",filaMin);
		
		//Escolher fila nao prioritaria preferencialmente
		for(i = 0;i<10;i++){
			if(filas[i]==filaMin){
				cliente->tempoComeco=relogio;
				vendedoresFila[filaMin][i]=cliente;
				printf("\n FILA ESCOLHIDA = %d",i);
				printf("\nCLIENTE GERADO PRIORIDADE: %d     TEMPOCOMECO: %d",vendedoresFila[filaMin][i]->prioridade,vendedoresFila[filaMin][i]->tempoComeco);
				break;
			}
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
    	//caso não exista posto livre ocupar fila
    	int i;
    	for(i=0;i<maxClients;i++){
    		if(levantamentoFila[i]==NULL && flag == 0){
    			levantamentoFila[i] = pagamentoPostos[posto];
    			levantamentoFila[i]->tempoComeco = relogio;
    			break;
    		}
    	}
	}
	
	
	// Limpeza de filas e postos. Quem nao compra vai embora.
	//Fila de pagamento vazia
	int flag=0;
	if(pagamentoFila[0]==NULL){
		pagamentoPostos[posto]=NULL;
		flag=1;
	}
	if(flag==0){
		//mover o primeiro cliente na fila para o posto
		pagamentoPostos[posto]=pagamentoFila[0];
		//atualizar variaveis estatisticas
		if(pagamentoPostos[posto]->tempoComeco!=0){
	  		tempos[1][zonaTempoAtual]+=(relogio - pagamentoPostos[posto]->tempoComeco);
	  		pagamentoPostos[posto]->tempoComeco=0;	
	  	}
		
		//criamos o evento do cliente agora a ser atendido no posto livre.
		makeEventPagamentoSaida(posto);
		
		//mover toda a fila 1 para a frente
	    for(i=1; i< maxClients; i++){
			pagamentoFila[i-1]= pagamentoFila[i];
			if(pagamentoFila[i]==NULL){
				pagamentoFila[i-1]=NULL; //fazemos null a ultima posição pois trata-se de um cliente duplicado
				break;
			}	
		}
	}
}

void setup(){
	//geração do número de clientes de minClients-maxClients
	int numberOfClients=minClients + rand() % (maxClients-minClients);
	int clientsLeft = numberOfClients;
	printf("%d # de clientes", numberOfClients);
	cli=numberOfClients;
	if(percentagem1+percentagem2+percentagem3 >100){
		printf("\nPercentagens invalidas");
		return;
	}
	//clientes por período de tempo
	int clientesTempo1 = numberOfClients*((float)percentagem1/(float)100.0); //10-13h
	clientesZonaTempo[0]= clientesTempo1;
	clientsLeft-= clientesTempo1;
	int clientesTempo2 = numberOfClients*((float)percentagem2/(float)100.0); //13-16h
	clientsLeft-= clientesTempo2;
	clientesZonaTempo[1]= clientesTempo2;
	int clientesTempo3 = numberOfClients*((float)percentagem3/(float)100.0); //16-19h
	clientsLeft-= clientesTempo3;
	clientesZonaTempo[2]= clientesTempo3;
 	int clientesTempo4 = clientsLeft;                  						 //19-22h
 	clientesZonaTempo[3]= clientesTempo4;
 	
 	printf("\n%d , %d , %d , %d total: %d\n", clientesTempo1, clientesTempo2, clientesTempo3, clientesTempo4,
	  clientesTempo1+clientesTempo2+clientesTempo3+clientesTempo4);
	
	//gerar os eventos de chegada de clientes
	int temp;
	for(temp=1 ; temp < 4; temp++){
		int tempo = 10800*temp;
		makeEvent( 4, tempo, -1);
		printf("%d", temp);
	}
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 10800 + 0;
		makeEvent( 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo2; temp++){
		int tempo = rand() % 10800 + 10800;
		makeEvent( 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo3; temp++){
		int tempo = rand() % 10800 + 21600;
		makeEvent( 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo4; temp++){
		int tempo = rand() % 10800 + 43200;
		makeEvent( 0, tempo, -1);
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
	printf("Postos: ");
	for(i=0;i<4;i++){
		if((pagamentoPostos[i]!=NULL)){
			if(pagamentoPostos[i]->prioridade==2)
				printf(" P |");
			if(pagamentoPostos[i]->prioridade==1)
				printf(" R |");
			if(pagamentoPostos[i]->prioridade==0)
				printf(" G |");
		}
		else
			printf(" 0 |");
	}
	printf("\nFilas :");
	for(i = 0; i< maxClients ;i++){
		if(pagamentoFila[i]!=NULL){
			if(pagamentoFila[i]->prioridade==0)
				printf("|G|");
			if(pagamentoFila[i]->prioridade==2)
				printf("|P|");
			if(pagamentoFila[i]->prioridade==1)
				printf("|R|");
		}
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
		else{
			if(vendedoresPostos[i]->prioridade==0)
				printf("|G|");
			if(vendedoresPostos[i]->prioridade==2)
				printf("|P|");
		}
	}
	printf("\nFILAS |");
	for(j=0;j<maxClients;j++){
		for(i=0;i<10;i++){
			if(vendedoresFila[j][i]==NULL){
				printf("|0|");
				finish++;
			}
			else{
				if(vendedoresFila[j][i]->prioridade==0){
					finish=0;
					printf("|G|");
					
				}
				if(vendedoresFila[j][i]->prioridade==2){
					finish=0;
					printf("|P|");
					
				}
			}
		}
		if(finish<20)
		printf("\n      |");
		else return;
	}
	printf("\n");
}
	
	

	


