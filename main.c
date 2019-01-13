/* Trabalho de Computação Científica 

Realizado por:
 Alexandre Fonseca (E10154) e Miguel Brandão (M9390) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>




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


//Funcoes
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
int  *bubble_sort(int *a, int n);
//Variaveis Globais

//alterar numero de clientes aqui
 #define minClients 300
 #define maxClients 320
int cli;
int probPrioritario = 5;
int retornados=0;
int saidavendedores=0;
//percentagem de clients por hora (Entrada de Clientes)
int percentagem1 = 10; //10-13h defeito:10
int percentagem2 = 30; //13-16h  defeito:30
int percentagem3 = 15; //16-19h  defeito:15
//  percentagem 4 é o restante para 100

//Variavel estatisticas
int espera00=9999999, espera01=9999999, espera02=9999999, espera03=9999999;
int espera10=9999999, espera11=9999999, espera12=9999999, espera13=9999999;
int espera20=9999999, espera21=9999999, espera22=9999999, espera23=9999999;
int esperam00=-1, esperam01=-1, esperam02=-1, esperam03=-1;
int esperam10=-1, esperam11=-1, esperam12=-1, esperam13=-1;
int esperam20=-1, esperam21=-1, esperam22=-1, esperam23=-1;
int tempos[3][4] = {0};
int zonaTempoAtual= 0;
int clientesZonaTempo[4];
int clientescolocadosprioridade=0;
int clientesprioritarios=0;
int  *minimo;
int  *maximo ;

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

//Ficheiro 
FILE *fp;



int main() {
	fp= fopen("iteracoes.txt","w");
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
    fprintf(fp,"\nNUM_CLIENTES: %d", cli);
	fprintf(fp,"\n\n\n");
	while(evento!=NULL){
		tipoEvento=evento->tipo;
		relogio=evento->tempoOcorrencia;
		eventosprocessados++;
		fprintf(fp,"\n------ UM NOVO EVENTO ------\n");
		switch(tipoEvento){
			case 0:
				fprintf(fp,"EVENTO_TIPO: 0 CHEGADA CLIENTE\n");
				break;
			case 1:
				fprintf(fp,"EVENTO_TIPO: 1 SAIDA VENDEDORES");
				fprintf(fp," %d \n",evento->posto);
				break;
			case 2:
				fprintf(fp,"EVENTO_TIPO: 2 SAIDA PAGAMENTO\n");
				break;
			case 3:
				fprintf(fp,"EVENTO_TIPO: 3 SAIDA LEVANTAMENTO\n");
				break;
			case 4:
				fprintf(fp,"EVENTO_TIPO: 4 PASSAGEM ZONA TEMPORAL\n");
				break;
		}
		fprintf(fp,"NUM_EVENTOS: %d\n", eventosprocessados);
		fprintf(fp,"CLOCK_ATUAL: %d\n",relogio);
		switch(tipoEvento){
			//Chegada Cliente
			case 0:
				processarEventoChegada();
			    imprimeFilaVendedores(); 
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
				imprimeFilaPagamento();
				eventoslevantamento++;
				break;
			case 4:
				eventospassagem++;
				zonaTempoAtual++;
				fprintf(fp,"ESTAMOS AGORA NA ZONA %d", zonaTempoAtual);
				break;
			default:
				return;
				break;
	   }
	 evento=evento->nseg;
	}
	
	imprimeFilaVendedores();
	imprimeFilaPagamento();
	imprimeFilaLevantamento();
	printf("\n\n %d clientes e %d eventos processados\n", cli, eventosprocessados);
	printf("\n Eventos de chegada            : %d",eventoschegada);
	printf("\n Eventos de saida vendedores   : %d",eventosvendedor);
	printf("\n Eventos de saida pagamento    : %d",eventospagamento);
	printf("\n Eventos de saida levantamento : %d\n",eventoslevantamento);
	//printf("\nRETORNADOS                    : %d", retornados);
	printf("\n SAIDAS NOS VENDEDORES         : %d", saidavendedores);
	printf("\n EVENTOS PASSAGEM              : %d\n", eventospassagem);
	int i;
	for(i=0;i<4;i++){
		printf(" CLIENTES ZONA TEMPORAL %d    : %d\n",i+1,clientesZonaTempo[i]);
	}
	
	printf("\n CLIENTES COLOCADOS POR PRIORIDADE: %d",clientescolocadosprioridade);
	printf("\n CLIENTES PRIORITARIOS           :  %d",clientesprioritarios);
	printf("\n ESPERAS                        ");
	printf("\n 10h-13h:");
	printf("\n FASE DE VENDEDORES             : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[0][0]/clientesZonaTempo[0],espera00,esperam00);
	printf("\n FASE DE PAGAMENTO              : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[1][0]/clientesZonaTempo[0],espera10,esperam10);
	printf("\n FASE DE LEVANTAMENTO           : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[2][0]/clientesZonaTempo[0],espera20,esperam20);
	printf("\n 13h-16h:");
	printf("\n FASE DE VENDEDORES             : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[0][1]/clientesZonaTempo[1],espera01,esperam01);
	printf("\n FASE DE PAGAMENTO              : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[1][1]/clientesZonaTempo[1],espera11,esperam11);
	printf("\n FASE DE LEVANTAMENTO           : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[2][1]/clientesZonaTempo[1],espera21,esperam21);
	printf("\n 16h-19h:");
	printf("\n FASE DE VENDEDORES             : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[0][2]/clientesZonaTempo[2],espera02,esperam02);
	printf("\n FASE DE PAGAMENTO              : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[1][2]/clientesZonaTempo[2],espera12,esperam12);
	printf("\n FASE DE LEVANTAMENTO           : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[2][2]/clientesZonaTempo[2],espera22,esperam22);
	printf("\n 19h-22h:");
	printf("\n FASE DE VENDEDORES             : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[0][3]/clientesZonaTempo[3],espera03,esperam03);
	printf("\n FASE DE PAGAMENTO              : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[1][3]/clientesZonaTempo[3],espera13,esperam13);
	printf("\n FASE DE LEVANTAMENTO           : MEDIA:%d MINIMA:%d MAXIMA:%d",tempos[2][3]/clientesZonaTempo[3],espera23,esperam23);
	int mediasMinimas  [12]  = {espera00,espera10,espera20,espera01,espera11,espera21,espera02,espera12,espera22,espera03,espera13,espera23};
	int mediasMaximas  [12] = {esperam00,esperam10,esperam20,esperam01,esperam11,esperam21,esperam02,esperam12,esperam22,esperam03,esperam13,esperam23};
	 minimo  = bubble_sort(mediasMinimas,12);
     maximo  = bubble_sort(mediasMaximas,12);
     int mediaTotal=((tempos[0][0]/clientesZonaTempo[0]+tempos[1][0]/clientesZonaTempo[0]+tempos[2][0]/clientesZonaTempo[0]+tempos[0][1]/clientesZonaTempo[1]+
	 tempos[1][1]/clientesZonaTempo[1]+tempos[2][1]/clientesZonaTempo[1]+tempos[0][3]/clientesZonaTempo[3]+tempos[1][3]/clientesZonaTempo[3]
	 +tempos[2][3]/clientesZonaTempo[3])/12);
	printf("\n TEMPO DE ESPERA MINIMO NAS 12H: %d",minimo[0]);
	printf("\n TEMPO DE ESPERA MAXIMO NAS 12H: %d",maximo[11]);
	printf("\n TEMPO DE ESPERA MEDIO  NAS 12H: %d",mediaTotal);
	printf("\n Press ENTER key to Continue\n");  
	fprintf(fp,"\n-------   FIM   -------");
	fclose(fp);
	
	//getchar(); 
	
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
	fprintf(fp,"\n Novo Evento gerado:\n ETIPO:2\n TEMPO:%d\n",tempo);	
	//imprimeLista();

}

void processarEventoSaidaLevantamento(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
  	
  	if(prob<6){
    // Clientes que regressam ao pagamento
    	fprintf(fp,"\n CLIENTE DECIDIU REGRESSAR AO PAGAMENTO");
    	//alteramos a prioridade do cliente para retornado
    	levantamentoPostos[posto]->prioridade=1;
		int flag=0;
		for(i=0;i<4;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				makeEventPagamentoSaida(i); //LEVANTAMENTO!
				pagamentoPostos[i] = levantamentoPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso nao exista posto livre ocupar fila com prioridade R
    	if(flag==0){
	    	fprintf(fp,"\nCLIENTE COLOCADO EM FILA");
    		//selecionar o lugar correcto
    		int lugar;
    		for(i=0;i<maxClients;i++){
    			if(pagamentoFila[i]==NULL){
    				lugar=i;
    				break;
    			}
    			if(pagamentoFila[i]==NULL || pagamentoFila[i]->prioridade==0){
    				lugar=i;
    				break;
    			}
    		}
    		int j;
    		fprintf(fp," POR PRIORIDADE RETORNADO NO LUGAR %d",i+1);
    		//mover os clientes na fila para trás
			for(j=maxClients-1;j>=lugar;j--){
				if(pagamentoFila[j]==NULL){
					continue;
				}
				pagamentoFila[j+1]=pagamentoFila[j];
			}
			//colocar o cliente no seu lugar
			pagamentoFila[lugar]=levantamentoPostos[posto];
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
			if(levantamentoPostos[posto]->tempoComeco<10800){
				if(espera20>(relogio - levantamentoPostos[posto]->tempoComeco))
					espera20=relogio - levantamentoPostos[posto]->tempoComeco;
				if(esperam20<(relogio - levantamentoPostos[posto]->tempoComeco))
					esperam20=relogio - levantamentoPostos[posto]->tempoComeco;
				tempos[2][0]+=relogio - levantamentoPostos[posto]->tempoComeco;
			}if(levantamentoPostos[posto]->tempoComeco>10799 && levantamentoPostos[posto]->tempoComeco<21600){
				if(espera21>(relogio - levantamentoPostos[posto]->tempoComeco))
					espera21=relogio - levantamentoPostos[posto]->tempoComeco;
				if(esperam21<(relogio - levantamentoPostos[posto]->tempoComeco))
					esperam21=relogio - levantamentoPostos[posto]->tempoComeco;
				tempos[2][1]+=relogio - levantamentoPostos[posto]->tempoComeco;
			}if(levantamentoPostos[posto]->tempoComeco>21599 && levantamentoPostos[posto]->tempoComeco<32400){
				if(espera22>(relogio - levantamentoPostos[posto]->tempoComeco))
					espera22=relogio - levantamentoPostos[posto]->tempoComeco;
				if(esperam22<(relogio - levantamentoPostos[posto]->tempoComeco))
					esperam22=relogio - levantamentoPostos[posto]->tempoComeco;
				tempos[2][2]+=relogio - levantamentoPostos[posto]->tempoComeco;
			}if(levantamentoPostos[posto]->tempoComeco>32399){
				if(espera23>(relogio - levantamentoPostos[posto]->tempoComeco))
					espera23=relogio - levantamentoPostos[posto]->tempoComeco;
				if(esperam23<(relogio - levantamentoPostos[posto]->tempoComeco))
					esperam23=relogio - levantamentoPostos[posto]->tempoComeco;
				tempos[2][3]+=relogio - levantamentoPostos[posto]->tempoComeco;
			}
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
		fprintf(fp,"\n CLIENTE DECIDIU IR AO PAGAMENTO");
		saidavendedores--;
		int flag=0;
		for(i=0;i<4;i++){
		//Ocupar posto, se existir
			if(pagamentoPostos[i]==NULL){
				fprintf(fp,"\nCLIENTE COLOCADO EM POSTO");
				makeEventPagamentoSaida(i);
				pagamentoPostos[i] = vendedoresPostos[posto];
				flag=1;
				break;
			}
    	}
    	//caso nao exista posto livre ocupar fila
    	if(flag==0){
	    	fprintf(fp,"\nCLIENTE COLOCADO EM FILA");
	    	//caso prioritario
	    	if(vendedoresPostos[posto]->prioridade==2){
	    		//selecionar o lugar correcto
	    		int lugar;
	    		for(i=0;i<maxClients;i++){
	    			if(pagamentoFila[i]==NULL){
	    				lugar=i;
	    				break;
	    			}
	    			if(pagamentoFila[i]->prioridade==0 || pagamentoFila[i]->prioridade == 1){
	    				lugar=i;
	    				break;
	    			}
	    		}
	    		fprintf(fp," POR PRIORIDADE NO LUGAR %d",i+1);
	    		//mover os clientes na fila para trás
				for(j=maxClients-1;j>=lugar;j--){
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
		    	fprintf(fp," SEM PRIORIDADE");
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
		fprintf(fp,"\n Nao ha ninguem na fila\n");
		vendedoresPostos[posto]=NULL;
		flag=1;
	}


	if(flag==0){
	    //ir buscar o próximo cliente em fila
	    vendedoresPostos[posto]=vendedoresFila[0][posto];
		
		//atualizar variaveis estatisticas
		if(vendedoresPostos[posto]->tempoComeco!=0){
			if(vendedoresPostos[posto]->tempoComeco<10800){
				if(espera00>(relogio - vendedoresPostos[posto]->tempoComeco))
					espera00=relogio - vendedoresPostos[posto]->tempoComeco;
				if(esperam00<(relogio - vendedoresPostos[posto]->tempoComeco))
					esperam00=relogio - vendedoresPostos[posto]->tempoComeco;
				tempos[0][0]+=relogio - vendedoresPostos[posto]->tempoComeco;
			}if(vendedoresPostos[posto]->tempoComeco>10799 && vendedoresPostos[posto]->tempoComeco<21600){
				if(espera01>(relogio - vendedoresPostos[posto]->tempoComeco))
					espera01=relogio - vendedoresPostos[posto]->tempoComeco;
				if(esperam01<(relogio - vendedoresPostos[posto]->tempoComeco))
					esperam01=relogio - vendedoresPostos[posto]->tempoComeco;
				tempos[0][1]+=relogio - vendedoresPostos[posto]->tempoComeco;
			}if(vendedoresPostos[posto]->tempoComeco>21599 && vendedoresPostos[posto]->tempoComeco<32400){
				if(espera02>(relogio - vendedoresPostos[posto]->tempoComeco))
					espera02=relogio - vendedoresPostos[posto]->tempoComeco;
				if(esperam02<(relogio - vendedoresPostos[posto]->tempoComeco))
					esperam02=relogio - vendedoresPostos[posto]->tempoComeco;
				tempos[0][2]+=relogio - vendedoresPostos[posto]->tempoComeco;
			}if(vendedoresPostos[posto]->tempoComeco>32399){
				if(espera03>(relogio - vendedoresPostos[posto]->tempoComeco))
					espera03=relogio - vendedoresPostos[posto]->tempoComeco;
				if(esperam03<(relogio - vendedoresPostos[posto]->tempoComeco))
					esperam03=relogio - vendedoresPostos[posto]->tempoComeco;
				tempos[0][3]+=relogio - vendedoresPostos[posto]->tempoComeco;
			}
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
	fprintf(fp,"\n Novo Evento gerado:\n ETIPO:1\n TEMPO:%d\n",tempo);

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
	fprintf(fp,"\n Novo Evento gerado:\n ETIPO:3\n TEMPO:%d\n",tempo);

}

void processarEventoChegada(){
	int i=0,j=0;
	Cliente *cliente=(Cliente*) malloc(1*sizeof(Cliente));
    //Gerar numero clientes prioritarios
	cliente->tempoComeco=0;
	if(rand() % + 101 < (probPrioritario+1)){
		cliente->prioridade=2;
		clientesprioritarios++;
		fprintf(fp,"\n CLIENTE PRIORITARIO");
	}
	else{
		cliente->prioridade=0;
		fprintf(fp,"\n CLIENTE NAO PRIORITARIO");
	}
	//2 filas prioritarias:8 e 9. Clientes gerais: Escolher onde ha menos gente. Se iguais, nao prioritaria.
	for(i=0;i<10;i++){
		//Ocupar posto, se existir
		if(vendedoresPostos[i]==NULL){
			fprintf(fp,"\n CLIENTE COLOCADO NO POSTO %d",i);
			makeEventVendedoresSaida(i);
			vendedoresPostos[i] = cliente;
			return;
		}
    }

	fprintf(fp,"\nA COLOCAR CLIENTE EM FILA");
	//Qdo os postos estao cheios, colocar o cliente na fila otima	
	//caso cliente prioritario
	if(cliente->prioridade==2){ //Escolher a fila prioritaria com menos pessoas prioritarias em fila
		fprintf(fp,"\nCliente colocado por prioridade");
		int filasprioridade[2]={0}; //# de clientes prioritarios nas filas prioritarias
		for(i=0;i<2;i++){
			for(j=0;j<maxClients;j++){
				if(vendedoresFila[j][i+8]==NULL || vendedoresFila[j][i+8]->prioridade==0){
					filasprioridade[i]=j;
					fprintf(fp,"|%d",filasprioridade[i]);
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
		fprintf(fp,"\nCLIENTE NAO PRIORITARIO");
		int filas[10]={0};
		for(i=0;i<10;i++){
			for(j=0;j<maxClients;j++){
				if(vendedoresFila[j][i]==NULL){
					filas[i]=j;
					fprintf(fp,"|%d",filas[i]);
					break;
				}
			}
		}
		
		int filaMin=maxClients;
		for(i=0;i<10;i++){
			if(filas[i]<filaMin)
				filaMin=filas[i];
		}
		fprintf(fp,"\n FILA MINIMA=%d",filaMin);
		
		//Escolher fila nao prioritaria preferencialmente
		for(i = 0;i<10;i++){
			if(filas[i]==filaMin){
				cliente->tempoComeco=relogio;
				vendedoresFila[filaMin][i]=cliente;
				fprintf(fp,"\n FILA ESCOLHIDA = %d",i);
				fprintf(fp,"\nCLIENTE GERADO PRIORIDADE: %d     TEMPOCOMECO: %d",vendedoresFila[filaMin][i]->prioridade,vendedoresFila[filaMin][i]->tempoComeco);
				break;
			}
		}
	}
}

void processarEventoSaidaPagamento(){
	int i;
	int posto=evento->posto;
	int prob= rand() % 101;
	
	//clientes retornados vão embora
	if(pagamentoPostos[posto]->prioridade==1){
		//vamos buscar o proximo cliente em fila geramos o seu evento e movemos a fila
		//Fila de pagamento vazia
		fprintf(fp,"\nCLIENTE FOI EMBORA DEVIDO A SER RETORNADO\n");
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
			if(pagamentoPostos[posto]->tempoComeco<10800){
				if(espera10>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera10=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam10<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam10=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][0]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>10799 && pagamentoPostos[posto]->tempoComeco<21600){
				if(espera11>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera11=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam11<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam11=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][1]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>21599 && pagamentoPostos[posto]->tempoComeco<32400){
				if(espera12>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera12=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam12<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam12=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][2]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>32399){
				if(espera13>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera13=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam13<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam13=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][3]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}
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
		return;
	}
	// Clientes que levantam
	if(prob<61){
		fprintf(fp,"\n CLIENTE DECIDIU IR AO LEVANTAMENTO");
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
    	if(flag==0){
	    	fprintf(fp,"\nCLIENTE COLOCADO EM FILA");
	    	//caso prioritario
	    	if(pagamentoPostos[posto]->prioridade==2){
	    		//selecionar o lugar correcto
	    		int lugar;
	    		for(i=0;i<maxClients;i++){
	    			if(levantamentoFila[i]==NULL){
	    				lugar=i;
	    				break;	    				
	    			}
	    			if(levantamentoFila[i]==NULL || levantamentoFila[i]->prioridade==0){
	    				lugar=i;
	    				break;
	    			}
	    		}
	    		int j;
	    		fprintf(fp," POR PRIORIDADE NO LUGAR %d",i+1);
	    		//mover os clientes na fila para trás
				for(j=maxClients-1;j>=lugar;j--){
					if(levantamentoFila[j]==NULL){
						continue;
					}
					levantamentoFila[j+1]=levantamentoFila[j];
				}
				//colocar o cliente no seu lugar
				levantamentoFila[lugar]=pagamentoPostos[posto];
	    	}
	    	else{
	    	//caso nao prioritario
		    	int i;
		    	fprintf(fp," SEM PRIORIDADE");
		    	for(i=0;i<maxClients;i++){
		    		if(levantamentoFila[i]==NULL && flag == 0){
		    			//inicializar o novo tempo
		    			pagamentoPostos[posto]->tempoComeco=relogio;
		    			levantamentoFila[i] = pagamentoPostos[posto];
		    			break;
		    		}
		    	}
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
			if(pagamentoPostos[posto]->tempoComeco<10800){
				if(espera10>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera10=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam10<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam10=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][0]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>10799 && pagamentoPostos[posto]->tempoComeco<21600){
				if(espera11>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera11=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam11<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam11=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][1]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>21599 && pagamentoPostos[posto]->tempoComeco<32400){
				if(espera12>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera12=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam12<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam12=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][2]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}if(pagamentoPostos[posto]->tempoComeco>32399){
				if(espera13>(relogio - pagamentoPostos[posto]->tempoComeco))
					espera13=relogio - pagamentoPostos[posto]->tempoComeco;
				if(esperam13<(relogio - pagamentoPostos[posto]->tempoComeco))
					esperam13=relogio - pagamentoPostos[posto]->tempoComeco;
				tempos[1][3]+=relogio - pagamentoPostos[posto]->tempoComeco;
			}
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
	//geracao do numero de clientes de minClients-maxClients
	int numberOfClients=minClients + rand() % (maxClients-minClients);
	int clientsLeft = numberOfClients;
	//fprintf(fp,"%d # de clientes", numberOfClients);
	cli=numberOfClients;
	if(percentagem1+percentagem2+percentagem3 >100){
		//printf("\nPercentagens invalidas");
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
 	
 	//printf("\n%d , %d , %d , %d total: %d\n", clientesTempo1, clientesTempo2, clientesTempo3, clientesTempo4,
	  //clientesTempo1+clientesTempo2+clientesTempo3+clientesTempo4);
	
	//gerar os eventos de chegada de clientes
	int temp;
	for(temp=1 ; temp < 4; temp++){
		int tempo = 10800*temp;
		makeEvent( 4, tempo, -1);
		//fprintf(fp,"%d", temp);
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
		int tempo = rand() % 10800 + 32400;
		makeEvent( 0, tempo, -1);
	}
	
	//fprintf(fp,"\n\n\n");
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
	fprintf(fp,"\nLista de eventos:");
	if(aux==NULL)
		fprintf(fp,"Vazio");
		while(aux!=NULL){
			fprintf(fp,"%d,%d,%d-> ",aux->tipo,aux->tempoOcorrencia,aux->posto);
			aux=aux->nseg;
		}
	fprintf(fp,"\n");
}

void imprimeFilaLevantamento(){
	int i;
	fprintf(fp,"\n\nFila de Levantamento\n");
	fprintf(fp,"Postos: ");
	for(i=0;i<2;i++){
		if((levantamentoPostos[i]!=NULL)){
			if(levantamentoPostos[i]->prioridade==2)
				fprintf(fp," P |");
			if(levantamentoPostos[i]->prioridade==1)
				fprintf(fp," R |");
			if(levantamentoPostos[i]->prioridade==0)
				fprintf(fp," G |");
		}
		else
			fprintf(fp," 0 |");
	}
	fprintf(fp,"\nFilas :");
	for(i = 0; i< maxClients ;i++){
		if(levantamentoFila[i]!=NULL){
			if(levantamentoFila[i]->prioridade==2)
				fprintf(fp," P |");
			if(levantamentoFila[i]->prioridade==1)
				fprintf(fp," R |");
			if(levantamentoFila[i]->prioridade==0)
				fprintf(fp," G |");
		}
		else break;
			
	}
	fprintf(fp,"\n");
}

void imprimeFilaPagamento(){
	int i;
	fprintf(fp,"\n\nFila de pagamento\n");
	fprintf(fp,"Postos: ");
	for(i=0;i<4;i++){
		if((pagamentoPostos[i]!=NULL)){
			if(pagamentoPostos[i]->prioridade==2)
				fprintf(fp," P |");
			if(pagamentoPostos[i]->prioridade==1)
				fprintf(fp," R |");
			if(pagamentoPostos[i]->prioridade==0)
				fprintf(fp," G |");
		}
		else
			fprintf(fp," 0 |");
	}
	fprintf(fp,"\nFilas :");
	for(i = 0; i< maxClients ;i++){
		if(pagamentoFila[i]!=NULL){
			if(pagamentoFila[i]->prioridade==0)
				fprintf(fp,"|G|");
			if(pagamentoFila[i]->prioridade==2)
				fprintf(fp,"|P|");
			if(pagamentoFila[i]->prioridade==1)
				fprintf(fp,"|R|");
		}
		else break;
	}
	fprintf(fp,"\n");
}
void imprimeFilaVendedores(){
	int i,j;
	int finish=0;
	fprintf(fp,"\n\nFila de vendedores\n");
	fprintf(fp,"Postos|");
	for(i=0;i<10;i++){
		if(vendedoresPostos[i]==NULL)
			fprintf(fp,"|0|");
		else{
			if(vendedoresPostos[i]->prioridade==0)
				fprintf(fp,"|G|");
			if(vendedoresPostos[i]->prioridade==2)
				fprintf(fp,"|P|");
		}
	}
	fprintf(fp,"\nFILAS |");
	for(j=0;j<maxClients;j++){
		for(i=0;i<10;i++){
			if(vendedoresFila[j][i]==NULL){
				fprintf(fp,"|0|");
				finish++;
			}
			else{
				if(vendedoresFila[j][i]->prioridade==0){
					finish=0;
					fprintf(fp,"|G|");
					
				}
				if(vendedoresFila[j][i]->prioridade==2){
					finish=0;
					fprintf(fp,"|P|");
					
				}
			}
		}
		if(finish<20)
		fprintf(fp,"\n      |");
		else return;
	}
	fprintf(fp,"\n");
}
	
int* bubble_sort(int *a, int n) {
   int i = 0, j = 0, tmp;
   for (i = 0; i < n; i++) {   
       for (j = 0; j < n - i - 1; j++) {           
            if (a[j] > a[j + 1]) {  
               tmp = a[j];
               a[j] = a[j + 1];
               a[j + 1] = tmp;
           }
       }
       
   }

 return a;
}




