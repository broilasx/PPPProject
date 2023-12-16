#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "list.h"
#include "list_functions.h"

#define MAX_INFO_SIZE 1000
#define MAX_ARRAY_SIZE 100


int main() {
    ListaReservas lista_reservas = NULL;
    ListaReservas lista_espera = NULL;
    parseFile("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", &lista_reservas);
    parseFile("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\listaEspera.txt", &lista_espera);
    checkPreReservaList(&lista_reservas, &lista_espera);
    int opcao;
    int var = 0;
    int isValidInput = 0;
    Cliente cliente;
    Data data;
    Hora hora;
    time_t currentTime;
    struct tm *timeInfo;

    int tipo_operacao;
    setlocale(LC_ALL, "");

    do {
        // Get the current time
        currentTime = time(NULL);
        timeInfo = localtime(&currentTime);

        // Extract the hour and minute from the current time
        int hour = timeInfo->tm_hour;
        int minute = timeInfo->tm_min;
        int day = timeInfo->tm_mday;
        int mon = timeInfo->tm_mon + 1;
        int year = timeInfo->tm_year + 1900;
        printf("Horas: %02d:%02d\n", hour, minute);
        printf("Data: %02d/%02d/%02d\n", day, mon, year);

        cancelPassedReservations(&lista_reservas, hour, minute, day, mon, year);
        cancelPassedPreReservations(&lista_espera, hour, minute, day, mon, year);

        printf("========= Oficina - Gestao de Reservas ==========\n");
        printf("|1. Reservar lavagem ou manutencao              |\n");
        printf("|2. Pre-reservar lavagem ou manutencao          |\n");
        printf("|3. Cancelar reserva                            |\n");
        printf("|4. Cancelar pre-reserva                        |\n");
        printf("|5. Listar reservas e pre-reservas por data     |\n");
        printf("|6. Listar reservas e pre-reservas de um cliente|\n");
        printf("|0. Sair                                        |\n");
        printf("=================================================\n");
        printf("Escolha uma opcao:");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome do cliente:");
                fflush(stdin);
                fgets(cliente.nome, MAX, stdin);
                while(1){
                    if (isFgetsEnter(cliente.nome)) {
                        printf("O nome do cliente esta vazio, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.nome) == 2 || is_string_or_int(cliente.nome) == 0){
                        printf("Nome invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                printf("Matricula (formato aa-11-aa):");
                fflush(stdin);
                fgets(cliente.matricula, MAX, stdin);
                while(1) {
                    if (isFgetsEnter(cliente.matricula)) {
                        printf("A matricula esta vazia, digite novamente:");
                        fflush(stdin);
                        fgets(cliente.matricula, MAX, stdin);
                    } else {
                        break;
                    }
                }
                while(1){
                    if(isPlateValid(cliente.matricula) == false){
                        printf("Matricula invalida, digite novamente:");
                        fflush(stdin);
                        fgets(cliente.matricula, MAX, stdin);
                    }
                    else{
                        break;
                    }
                }
                fflush(stdin);
                printf("Contacto do cliente:");
                scanf("%[^\n]",&cliente.contacto);
                while(1){
                    if(strlen(cliente.contacto) != 9){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.contacto) == 1){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                printf("Tipo (0=Lavagem, 1=Manutencao):");
                while(!isValidInput){
                    if (scanf("%d", &tipo_operacao) == 1) {
                        if (tipo_operacao == 0 || tipo_operacao == 1) {
                            isValidInput = 1;
                        } else {
                            printf("Tipo de operacao invalido, digite novamente:");
                        }
                    } else {
                        printf("Tipo de operacao invalido, digite novamente:");
                        scanf("%*[^\n]");  // Clear the input buffer
                    }
                }
                fflush(stdin);
                printf("Hora de inicio (HH:MM) ou (HH:00):");
                scanf("%02d:%02d", &hora.hora,&hora.minuto);
                while(1){
                    if((hora.hora<8) && (hora.minuto>=0) && (hora.minuto<=60)  || (hora.hora==17) && (hora.minuto>30) && (hora.minuto<=60) || (hora.hora>=18) && (hora.minuto>=0) && (hora.minuto<=60)){
                        printf("Hora para reserva indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d:%02d", &hora.hora,&hora.minuto);
                    }
                    else if (tipo_operacao == 1 && hora.hora == 17 && hora.minuto >=01) {
                        printf("Operacao indisponivel a essa hora.\n");
                        var = 1;
                        break;
                    } else {
                        break;
                    }
                }
                fflush(stdin);
                printf("Data(formato D/M/A): ");
                fflush(stdin);
                scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                while(1){
                    if (data.dia == 0 || data.mes == 0 || data.ano == 0 || data.dia>31 || (data.mes > 12) || data.ano < 2023 || data.dia<0 || (data.mes < 0) || data.ano < 0 || data.ano > 2100) {
                        printf("Data indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                    }
                    else{
                        break;
                    }
                }

                if (var == 1) {
                    printf("Erro na Reserva pois a manutencao nao pode ser reservada depois das 17!");
                } else {
                    writeFile("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", cliente, data, hora, tipo_operacao);
                    reservar(&lista_reservas, cliente, data, hora, tipo_operacao);
                }
                break;

            case 2:
                fflush(stdin);
                printf("Nome do cliente:");
                fflush(stdin);
                fgets(cliente.nome, MAX, stdin);
                while(1){
                    if (isFgetsEnter(cliente.nome)) {
                        printf("O nome do cliente esta vazio, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.nome) == 2 || is_string_or_int(cliente.nome) == 0){
                        printf("Nome invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                fflush(stdin);
                printf("Matricula (formato aa-11-aa):");
                fflush(stdin);
                fgets(cliente.matricula, MAX, stdin);
                while(1) {
                    if (isFgetsEnter(cliente.matricula)) {
                        printf("A matricula esta vazia, digite novamente:");
                        fflush(stdin);
                        fgets(cliente.matricula, MAX, stdin);
                    } else {
                        break;
                    }
                }
                while(1){
                    if(isPlateValid(cliente.matricula) == false){
                        printf("Matricula invalida, digite novamente:");
                        fflush(stdin);
                        fgets(cliente.matricula, MAX, stdin);
                    }
                    else{
                        break;
                    }
                }
                fflush(stdin);
                printf("Contacto do cliente:");
                scanf("%[^\n]",&cliente.contacto);
                while(1){
                    if(strlen(cliente.contacto) != 9){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.contacto) == 1){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                fflush(stdin);
                printf("Tipo (0=Lavagem, 1=Manutencao):");
                while(!isValidInput){
                    if (scanf("%d", &tipo_operacao) == 1) {
                        if (tipo_operacao == 0 || tipo_operacao == 1) {
                            isValidInput = 1;
                        } else {
                            printf("Tipo de operacao invalido, digite novamente:");
                        }
                    } else {
                        printf("Tipo de operacao invalido, digite novamente:");
                        scanf("%*[^\n]");  // Clear the input buffer
                    }
                }
                fflush(stdin);
                printf("Hora de inicio (HH:MM) ou (HH:00):");
                scanf("%02d:%02d", &hora.hora,&hora.minuto);
                while(1){
                    if((hora.hora<8) && (hora.minuto>=0) && (hora.minuto<=60)  || (hora.hora==17) && (hora.minuto>30) && (hora.minuto<=60) || (hora.hora>=18) && (hora.minuto>=0) && (hora.minuto<=60)){
                        printf("Hora para pre-reserva indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d:%02d", &hora.hora,&hora.minuto);
                    }
                    else if (tipo_operacao == 1 && hora.hora == 17 && hora.minuto >=01) {
                        printf("Operacao indisponivel a essa hora.\n");
                        var = 1;
                        break;
                    } else {
                        break;
                    }
                }
                fflush(stdin);
                printf("Data(formato D/M/A): ");
                fflush(stdin);
                scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                while(1){
                    if (data.dia == 0 || data.mes == 0 || data.ano == 0 || data.dia>31 || (data.mes > 12) || data.ano < 2023 || data.dia<0 || (data.mes < 0) || data.ano < 0 || data.ano > 2100) {
                        printf("Data indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                    }
                    else{
                        break;
                    }
                }
                writeFile("C:\\Users\\bruno\\OneDrive\\Ambiente de Trabalho\\listaEspera.txt", cliente, data, hora, tipo_operacao);
                pre_reservar(&lista_espera, &lista_reservas, cliente, data, hora, tipo_operacao);
                break;
            case 3:
                fflush(stdin);
                printf("Hora de inicio da reserva a cancelar:");
                scanf("%02d:%02d", &hora.hora,&hora.minuto);
                while(1){
                    if((hora.hora<8) && (hora.minuto>=0) && (hora.minuto<=60)  || (hora.hora==17) && (hora.minuto>30) && (hora.minuto<=60) || (hora.hora>=18) && (hora.minuto>=0) && (hora.minuto<=60)){
                        printf("Hora para cancelamento indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d:%02d", &hora.hora,&hora.minuto);
                    } else {
                        break;
                    }
                }
                fflush(stdin);
                printf("Data da reserva a cancelar(DD/MM/AAAA):");
                fflush(stdin);
                scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                while(1){
                    if (data.dia == 0 || data.mes == 0 || data.ano == 0 || data.dia>31 || (data.mes > 12) || data.ano < 2023 || data.dia<0 || (data.mes < 0) || data.ano < 0 || data.ano > 2100) {
                        printf("Data indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                    }
                    else{
                        break;
                    }
                }
                cancelar(&lista_reservas, hora, data);
                checkPreReservaList(&lista_reservas, &lista_espera);
                cancelar_pre_reserva(&lista_espera, hora, data);
                break;
            case 4:
                fflush(stdin);
                printf("Hora de inicio da pre-reserva a cancelar:");
                scanf("%02d:%02d", &hora.hora,&hora.minuto);
                while(1){
                    if((hora.hora<8) && (hora.minuto>=0) && (hora.minuto<=60)  || (hora.hora==17) && (hora.minuto>30) && (hora.minuto<=60) || (hora.hora>=18) && (hora.minuto>=0) && (hora.minuto<=60)){
                        printf("Hora para cancelamento indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d:%02d", &hora.hora,&hora.minuto);
                    } else {
                        break;
                    }
                }
                fflush(stdin);
                printf("Data da reserva a cancelar(DD/MM/AAAA):");
                fflush(stdin);
                scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                while(1){
                    if (data.dia == 0 || data.mes == 0 || data.ano == 0 || data.dia>31 || (data.mes > 12) || data.ano < 2023 || data.dia<0 || (data.mes < 0) || data.ano < 0 || data.ano > 2100) {
                        printf("Data indisponivel, digite novamente:");
                        fflush(stdin);
                        scanf("%02d/%02d/%04d", &data.dia, &data.mes, &data.ano);
                    }
                    else{
                        break;
                    }
                }
                cancelar_pre_reserva(&lista_espera,hora, data);
                break;
            case 5:
                listar_reservas(lista_reservas, lista_espera);
                break;
            case 6:
                fflush(stdin);
                printf("Nome do cliente:");
                fflush(stdin);
                fgets(cliente.nome, MAX, stdin);
                while(1){
                    if (isFgetsEnter(cliente.nome)) {
                        printf("O nome do cliente esta vazio, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.nome) == 2 || is_string_or_int(cliente.nome) == 0){
                        printf("Nome invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.nome);
                    }
                    else{
                        break;
                    }
                }
                fflush(stdin);
                printf("Contacto do cliente:");
                scanf("%[^\n]",&cliente.contacto);
                while(1){
                    if(strlen(cliente.contacto) != 9){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                while(1){
                    if(is_string_or_int(cliente.contacto) == 1){
                        printf("Contacto invalido, digite novamente:");
                        fflush(stdin);
                        scanf("%[^\n]",&cliente.contacto);
                    }
                    else{
                        break;
                    }
                }
                listar_reservas_cliente(lista_reservas, lista_espera, cliente);
                break;
            case 0:
                printf("A sair da aplicacao.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }

        cancelPassedReservations(&lista_reservas, hour, minute, day, mon, year);
        cancelPassedPreReservations(&lista_espera, hour, minute, day, mon, year);

        printf("\n");
    } while (opcao != 0);

    return 0;

// Após sair do loop, pode-se gravar o estado atual das reservas em ficheiros de texto (FEITO A CADA VEZ QUE ADICIONAMOS UMA RESERVA!!!)
// e libertar a memória alocada para as listas de reservas e pré-reservas


}