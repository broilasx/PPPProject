#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include <stdbool.h>
#include "list.h"

//Verificar se a matricula é valida
bool isPlateValid(const char* plate);
//Verificar se foi digitado um Enter
int isFgetsEnter(const char* str);
int is_string_or_int(char *str);
void writeFile(char file[1024], Cliente cliente, Data data, Hora hora, int tipo_operacao);
//Funcao para verificar a existencia de uma reserva
int verificarExistenciaReserva(ListaReservas lista, Data data, Hora hora, int tipo_operacao);
// Função para reservar uma lavagem ou manutenção
void reservar(ListaReservas *lista, Cliente cliente, Data data, Hora hora, int tipo_operacao);
void checkPreReservaList(ListaReservas* lista_reservas, ListaReservas* lista_espera);
// Função para pré-reservar uma lavagem ou manutenção
void pre_reservar(ListaReservas* lista_espera, ListaReservas* lista_reservas, Cliente cliente, Data data, Hora hora, int tipo_operacao);
//Funcao para remover a reserva do ficheiro
void excludeReservation(char fileName[1024], int horaHora, int horaMinuto, int dataDia, int dataMes, int dataAno);
// Função para cancelar uma reserva
void cancelar(ListaReservas* lista, Hora hora, Data data);
// Função para cancelar uma pré-reserva
void cancelar_pre_reserva(ListaReservas* lista_espera, Hora hora, Data data);
// Função para listar as reservas e pré-reservas ordenadas por data (as mais antigas primeiro)
void listar_reservas(ListaReservas lista, ListaReservas lista_espera);
// Função para listar as reservas e pré-reservas de um cliente ordenadas por data (as mais recentes primeiro)
void listar_reservas_cliente(ListaReservas lista, ListaReservas lista_espera, Cliente cliente);
//Funcao que reserva na lista de Reservas as reservas presentes nos ficheiros
void parseFile(char file[1024], ListaReservas *lista);
//Funcao que verifica se uma reserva ja passou da hora e data do presente
void cancelPassedReservations(ListaReservas* lista_reservas, int hour, int minute, int day, int mon, int year);
//Funcao que verifica se uma pre-reserva ja passou da hora e data presente
void cancelPassedPreReservations(ListaReservas* lista_espera, int hour, int minute, int day, int mon, int year);
#endif