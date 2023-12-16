#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define MAX 1024

// estrutura de dados para representar um cliente
typedef struct {
    char nome[50];
    char matricula[10];
    char contacto[20];
} Cliente;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int hora;
    int minuto;
} Hora;

// estrutura de dados para representar uma reserva
typedef struct {
    Cliente cliente;
    Hora hora;
    Hora horafim;
    Data data;
    int tipo_operacao; // 0 para lavagem(30min), 1 para manutenção(1hora)
} Reserva;

// estrutura de dados para representar uma lista de reservas
typedef struct no {
    Reserva reserva;
    struct no *prox;
} NodoReserva;

typedef NodoReserva *ListaReservas;


#endif