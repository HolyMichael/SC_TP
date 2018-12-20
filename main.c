#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

typedef struct EVENTO{

int tipo; // ChegadaCliente(0), FimZonaVendedor(1), FimZonaPagamento(2), FimZonaLevantamento(3)
int tempoOcorrencia;
int posto;
struct EVENTO *nseg;
struct EVENTO *nant;
}Evento;



typedef struct Cliente{

int prioridade; //Prioridade: geral(0),retomado(1), prioritário(2)
int tempoComeco;//Tempo em que o cliente chega ao sistema 

}Cliente;




int main() {
	

}

//Criar Evento 
Evento *makeEvento(int tipo,int tempo,int posto){

Evento *nv=(Evento*) malloc(1*sizeof(Evento));
nv->tempoOcorrencia=tempo; 
nv->tipo=tipo;
nv->posto=posto;
nv->nseg=NULL;

return(nv);
}

//Inserir Evento no Inicio da Lista
Evento *inserirEventoInicio(Evento*L, Evento*nv){

nv->nseg=L;
nv->nant=NULL;
return(nv);

}

//Inserir Evento no Fim da Lista
Evento *inserirEventoFim(Evento *L, Evento *nv){

Evento *aux=L;

if(L==NULL)
	return(NULL);

while(L->nseg!=NULL)
	L=L->nseg;

L->nseg=nv;
nv->nant=L;

return(aux);

}

//Inserir Evento na Lista de Eventos tendo em conta o tempo de Ocorrencia(key)
Evento * inserirEventoOrdem(Evento *L, Evento *nv){

Evento *aux= L;

while(aux!=NULL){
	if(Evento->tempoOCorrencia > nv->tempoOcorrencia)
		brak;
	aux=aux->nseg;
}


if(aux==NULL)
	return(insertLast(L,nv);

if(aux->nant==NULL)
	return(insertFirst(L,nv);

nv->nseg=aux;
nv->nant=aux->nant;
aux->nant=nv;
nv->nant->nseg=nv;

return(L);

}











