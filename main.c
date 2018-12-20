#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helper.h"

//alterar número de clientes aqui
int minClients = 300;
int maxClients = 320;

//percentagem de clients por hora
int percentagem1 = 10; //10-13h
int percentagem2 = 30; //13-16h
int percentagem3 = 15; //16-19h
//  percentagem 4 é o restante para 100

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
 	
 	printf("\n%d , %d , %d , %d total: %d", clientesTempo1, clientesTempo2, clientesTempo3, clientesTempo4,
	  clientesTempo1+clientesTempo2+clientesTempo3+clientesTempo4);
	
	//gerar os eventos de chegada de clientes
	int temp;
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 300 + 0;
		//makeEventChegada(tipo 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 300 + 300;
		//makeEventChegada(tipo 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 300 + 600;
		//makeEventChegada(tipo 0, tempo, -1);
	}
	for(temp=0 ; temp < clientesTempo1; temp++){
		int tempo = rand() % 300 + 900;
		//makeEventChegada(tipo 0, tempo, -1);
	}
}
