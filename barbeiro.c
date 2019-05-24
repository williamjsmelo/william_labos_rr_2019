/*
Autor: William Juan da Silva Melo
Professor: Herbert Rocha
Disciplina: Análise de Algoritmos
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <windows.h>

sem_t sem_cadeiras_cliente;
sem_t sem_cadeira_barbeiro;
sem_t sem_cadeiras_desocupadas;
sem_t sem_cabelo_cortado;


int main(){

    void * func_cliente (void * v);
    void * func_barbeiro(void * v);

	printf("Informe a quatidade de clientes e a quantidade de cadeiras:\n");
	int clientes , cadeiras;
	scanf("%d %d", &clientes, &cadeiras);
    printf("\n\n");


	int i , id[clientes+1];							//#i e o id dos clientes
	pthread_t t_cliente[clientes]; 					//#thread clientes
	pthread_t t_barbeiro;		   					//#thread barbeiro

	sem_init(&sem_cadeiras_cliente    , 0, cadeiras); //#semaforo min e max de cadeiras espera
	sem_init(&sem_cadeira_barbeiro    , 0, 1);		  //#semaforo min e max de cadeiras de barbear
	sem_init(&sem_cadeiras_desocupadas, 0, 0);		  //#semaforo min e max de cadeiras desocupadas ou seja tudo vazio
	sem_init(&sem_cabelo_cortado      , 0, 0);		  //#semaforo min e max de cadeiras do barbeiro desocupado

	for(i=1 ; i<clientes+1 ; i++){
		id[i] = i;
		pthread_create(&t_cliente[i-1] , NULL, func_cliente , (void*)&id[i]); //#cria as threads dos clientes
	}

	pthread_create(&t_cliente, NULL, func_barbeiro , NULL); 					//#cria a thread do barbeiro

	for(i=1 ; i<clientes+1 ; i++){
		pthread_join(t_cliente[i], NULL);										//#espera terminar as threads
	}

	return 0;
}

void * func_cliente(void * v){
	int id = *(int*) v;
	Sleep(id%3);
	if(sem_trywait(&sem_cadeiras_cliente) == 0){
		printf("1 Cliente %d entrou na barbearia.\n", id);
		sem_wait(&sem_cadeira_barbeiro); 											//#acordou o barbeiro
		printf("2 Cliente %d sentou na cadeira do barbeiro.(inicio procedimento)\n" );//#cadeira foi ocupada pelo cliente
		sem_post(&sem_cadeiras_desocupadas);
		sem_post(&sem_cadeiras_cliente);
		sem_post(&sem_cabelo_cortado);
		sem_post(&sem_cadeira_barbeiro);
		printf("3 Cliente %d deixou a barbearia.(terminou o procedimento)\n" );
	}else{
		printf("4 Cliente %d nao entrou, estava sem cadeira.\n" );
	}
	return NULL;
}

void * func_barbeiro(void * v){
	while(1){									//#fica rodando porque sempre pode aparecer um cliente para atender
		sem_wait(&sem_cadeiras_cliente); 		//#bloqueia a cadeira por estar em uso
		printf("5 Barbeiro cortando o cabelo de um cliente.\n");
		sem_post(&sem_cabelo_cortado);			//#libera a cadeira para quem não cortou o cabelo
	}
	return NULL;
}
