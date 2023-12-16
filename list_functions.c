#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "list.h"

bool isPlateValid(const char* plate) {
    if (plate[2] != '-' || plate[5] != '-')
        return false;

    for (int i = 0; i < 2; i++) {
        if (!isalpha(plate[i]) || !isalpha(plate[i + 6]))
            return false;
    }

    for (int i = 3; i < 5; i++) {
        if (!isdigit(plate[i]))
            return false;
    }

    return true;
}

void removerNewline(char* string) {
    size_t length = strlen(string);
    if (length > 0 && string[length - 1] == '\n') {
        string[length - 1] = '\0';
    }
}

void writeFile(char file[1024], Cliente cliente, Data data, Hora hora, int tipo_operacao){
    FILE *fp = fopen(file, "a");

    if (fp == NULL) {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    // Remover o caractere de nova linha das strings
    removerNewline(cliente.nome);
    removerNewline(cliente.matricula);
    removerNewline(cliente.contacto);

    fprintf(fp, "%s;%s;%s;%02d;%02d;%02d;%02d;%04d;%01d\n", cliente.nome, cliente.matricula, cliente.contacto, hora.hora, hora.minuto, data.dia, data.mes, data.ano, tipo_operacao);
    fclose(fp);
}

int isFgetsEnter(const char* str) {
    size_t length = strlen(str);

    if (length == 1 && str[0] == '\n') {
        return 1;  // Foi pressionado apenas o enter
    }

    return 0;  // Foi lida uma string
}

int is_string_or_int(char *str) {
    int i = 0;

    // Verifica se a primeira posição é um sinal de menos.
    if (str[0] == '-') {
        i = 1;
        if (str[1] == '\0') {
            // Se o sinal de menos for o último caractere, não é um inteiro ou string válidos.
            return 0;
        }
    }
    int valor = 0;
    // Verifica se todos os caracteres são dígitos.
    for (; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            valor += 1; // Se encontrar algum caractere que não seja um dígito, é uma string.
        }
    }
    if(valor > 0){
        return 0;
    }
    else{
        return 1;
    }
}


int verificarExistenciaReserva(ListaReservas lista, Data data, Hora hora, int tipo_operacao) {
    NodoReserva* atual = lista;
    while (atual != NULL) {
        NodoReserva* reservaAtual = atual;
        while (reservaAtual != NULL) {
            if(tipo_operacao == 0){
                if ((reservaAtual->reserva.data.ano == data.ano &&
                     reservaAtual->reserva.data.mes == data.mes &&
                     reservaAtual->reserva.data.dia == data.dia &&
                     hora.hora == reservaAtual->reserva.hora.hora &&
                     hora.minuto+30 > reservaAtual->reserva.hora.minuto) ||
                    (reservaAtual->reserva.data.ano == data.ano &&
                     reservaAtual->reserva.data.mes == data.mes &&
                     reservaAtual->reserva.data.dia == data.dia &&
                     hora.hora - reservaAtual->reserva.hora.hora == 1 &&
                     reservaAtual->reserva.hora.minuto + 30 > hora.minuto + 60 ))
                {
                    return 1;  // Conflito no tempo de reserva
                }
                reservaAtual = reservaAtual->prox;
            }else if(tipo_operacao == 1){
                if ((reservaAtual->reserva.data.ano == data.ano &&
                     reservaAtual->reserva.data.mes == data.mes &&
                     reservaAtual->reserva.data.dia == data.dia &&
                     hora.hora == reservaAtual->reserva.hora.hora) ||
                    (reservaAtual->reserva.data.ano == data.ano &&
                     reservaAtual->reserva.data.mes == data.mes &&
                     reservaAtual->reserva.data.dia == data.dia &&
                     hora.hora - reservaAtual->reserva.hora.hora == 1 &&
                     reservaAtual->reserva.hora.minuto > hora.minuto))
                {
                    return 1;  // Conflito
                }
                reservaAtual = reservaAtual->prox;
            }
        }
        atual = atual->prox;
    }
    return 0;  // Sem conflito no tempo de reservas
}

// Função para reservar uma lavagem ou manutenção
void reservar(ListaReservas *lista, Cliente cliente, Data data, Hora hora, int tipo_operacao) {

    if(verificarExistenciaReserva(*lista, data, hora, tipo_operacao) == 1) {
        printf("Ja existe uma reserva para a data e hora especificadas.\n");
        return;
    }else{
        // Verificar se existe uma reserva no mm dia e hora
        Reserva reserva;
        reserva.cliente = cliente;
        reserva.hora = hora;
        reserva.data = data;
        reserva.tipo_operacao = tipo_operacao;

        NodoReserva* novo = (NodoReserva*)malloc(sizeof(NodoReserva));
        novo->reserva = reserva;
        novo->prox = NULL;

        if(tipo_operacao == 0){
            if(reserva.hora.minuto + 30 <= 60){
                reserva.horafim.hora = reserva.hora.hora;
                reserva.horafim.minuto = reserva.hora.minuto + 30;
            }
            else{
                reserva.horafim.hora = reserva.hora.hora + 1;
                reserva.horafim.minuto = ((reserva.hora.minuto + 30)%60);
            }
        }
        if(tipo_operacao == 1){
            if(reserva.hora.minuto + 60 == 60){
                reserva.horafim.hora = reserva.hora.hora;
                reserva.horafim.minuto = reserva.hora.minuto + 60;
            }
            else{
                reserva.horafim.hora = reserva.hora.hora + 1;
                reserva.horafim.minuto = ((reserva.hora.minuto + 60)%60);
            }
        }

        if(verificarExistenciaReserva(*lista, data, hora, tipo_operacao) == 1) {
            printf("Já existe uma reserva para a data e hora especificadas.\n");
            return;
        }
        if (*lista == NULL) {
            *lista = novo;
        } else {
            NodoReserva* atual = *lista;
            while (atual->prox != NULL) {
                atual = atual->prox;
            }
            atual->prox = novo;
        }

        printf("Reserva efetuada com sucesso.\n");}
}

// Função para pré-reservar uma lavagem ou manutenção
void pre_reservar(ListaReservas* lista_espera, ListaReservas* lista_reservas, Cliente cliente, Data data, Hora hora, int tipo_operacao) {
    Reserva reserva;
    reserva.cliente = cliente;
    reserva.data = data;
    reserva.hora = hora;
    reserva.tipo_operacao = tipo_operacao;

    NodoReserva* novo = (NodoReserva*)malloc(sizeof(NodoReserva));
    novo->reserva = reserva;
    novo->prox = NULL;

    if (*lista_reservas == NULL) {
        // se estiver vazia vai diretamente para a lista de reservas
        *lista_reservas = novo;
        printf("Reserva efetuada com sucesso.\n");
    } else {
        // verifiar se existe uma reserva
        if (verificarExistenciaReserva(*lista_reservas, data, hora, tipo_operacao)) {
            printf("Já existe uma reserva para a data e hora especificadas.\n");
            // colocar a pre-reserva na lista de espera
            if (*lista_espera == NULL) {
                *lista_espera = novo;
            } else {
                NodoReserva* atual = *lista_espera;
                while (atual->prox != NULL) {
                    atual = atual->prox;
                }
                atual->prox = novo;
            }
            printf("Pre-reserva efetuada com sucesso e colocada na lista de espera.\n");
        } else {
            NodoReserva* atual = *lista_reservas;
            while (atual->prox != NULL) {
                atual = atual->prox;
            }
            atual->prox = novo;
            printf("Reserva efetuada com sucesso.\n");
        }
    }
}

void excludeReservation(char fileName[1024], int horaHora, int horaMinuto, int dataDia, int dataMes, int dataAno) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    const int maxLineLength = 100;
    char line[maxLineLength];

    // ficheiro temp
    FILE* tempFile = fopen("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\temp.txt", "w");
    if (tempFile == NULL) {
        printf("Failed to create a temporary file.\n");
        fclose(file);
        return;
    }


    while (fgets(line, maxLineLength, file) != NULL) {
        int foundMatch = 0;
        char *name, *contacto, *matricula, *minuto_ini, *hora_ini, *dia, *mes, *ano, *type;
        int hora, minuto, dia_int, mes_int, ano_int, tipoOperacao;
        name = strtok(line, ";");
        contacto = strtok(NULL, ";");
        matricula = strtok(NULL, ";");
        hora_ini = strtok(NULL, ";");
        hora = atoi(hora_ini);
        minuto_ini = strtok(NULL, ";");
        minuto = atoi(minuto_ini);
        dia = strtok(NULL, ";");
        dia_int = atoi(dia);
        mes = strtok(NULL, ";");
        mes_int = atoi(mes);
        ano = strtok(NULL, ";");
        ano_int = atoi(ano);
        type = strtok(NULL, "\n");
        tipoOperacao = atoi(type);

        // verificar se a linha coincide
        if (hora == horaHora && minuto == horaMinuto && dia_int == dataDia && mes_int == dataMes && ano_int == dataAno) {
            foundMatch = 1;
        }

        removerNewline(name);
        removerNewline(matricula);
        removerNewline(contacto);

        // colocar os valores no ficheiro temp
        if(foundMatch == 0) {
            fprintf(tempFile, "%s;%s;%s;%02d;%02d;%02d;%02d;%4d;%1d\n", name, matricula, contacto, hora, minuto, dia_int, mes_int, ano_int, tipoOperacao);
        }
    }

    fclose(file);
    fclose(tempFile);

    // trocar o ficheiro atual com o temp
    remove(fileName);
    rename("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\temp.txt", fileName);
}

// Função para cancelar uma reserva
void cancelar(ListaReservas* lista, Hora hora, Data data) {
    if (*lista == NULL) {
        printf("Nao existem reservas.\n");
        return;
    }

    NodoReserva* anterior = NULL;
    NodoReserva* atual = *lista;

    while (atual != NULL) {
        if ((atual->reserva.hora.hora == hora.hora) && (atual->reserva.hora.minuto == hora.minuto) && (atual->reserva.data.dia == data.dia) && (atual->reserva.data.mes == data.mes) && (atual->reserva.data.ano == data.ano)) {
            NodoReserva* temp = atual;
            if (anterior == NULL) {
                *lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", hora.hora, hora.minuto, data.dia, data.mes, data.ano);
            free(temp);
            printf("Reserva cancelada.\n");
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Reserva nao encontrada.\n");
}

// Função para cancelar uma pré-reserva
void cancelar_pre_reserva(ListaReservas* lista_espera, Hora hora, Data data) {
    if (*lista_espera == NULL) {
        printf("Nao existem pre-reservas.\n");
        return;
    }
    NodoReserva* anterior = NULL;
    NodoReserva* atual = *lista_espera;

    while (atual != NULL) {
        if ((atual->reserva.hora.hora == hora.hora) && (atual->reserva.hora.minuto == hora.minuto) && (atual->reserva.data.dia == data.dia) && (atual->reserva.data.mes == data.mes) && (atual->reserva.data.ano == data.ano)) {
            NodoReserva* temp = atual;
            if (anterior == NULL) {
                *lista_espera = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\listaEspera.txt", atual->reserva.hora.hora, atual->reserva.hora.minuto, atual->reserva.data.dia, atual->reserva.data.mes, atual->reserva.data.ano);
            free(temp);
            printf("Pre-reserva cancelada.\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Pre-reserva nao encontrada.\n");
}

//Funcao que passa uma pre-reserva para a reserva se esta estiver disponivel
void checkPreReservaList(ListaReservas* lista_reservas, ListaReservas* lista_espera){
    NodoReserva* espera = *lista_espera;
    while(espera != NULL) {
        if (verificarExistenciaReserva(*lista_reservas, espera->reserva.data, espera->reserva.hora, espera->reserva.tipo_operacao) == 0) {
            reservar(lista_reservas, espera->reserva.cliente, espera->reserva.data, espera->reserva.hora, espera->reserva.tipo_operacao);
            writeFile("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", espera->reserva.cliente, espera->reserva.data, espera->reserva.hora, espera->reserva.tipo_operacao);
            cancelar_pre_reserva(lista_espera, espera->reserva.hora, espera->reserva.data);
        }
        espera = espera->prox;
    }
}

int compare_dates(Hora hora1, Hora hora2, Data data1, Data data2) {
    if (data1.ano > data2.ano)
        return -1;
    else if (data1.ano < data2.ano)
        return 1;
    else if (data1.mes > data2.mes)
        return -1;
    else if (data1.mes < data2.mes)
        return 1;
    else if (data1.dia > data2.dia)
        return -1;
    else if (data1.dia < data2.dia)
        return 1;
    else if (hora1.hora > hora2.hora)
        return -1;
    else if (hora1.hora < hora2.hora)
        return 1;
    else if (hora1.minuto > hora2.minuto)
        return -1;
    else if (hora1.minuto < hora2.minuto)
        return 1;
    else
        return 0;
}

// Função para listar as reservas e pré-reservas ordenadas por data (as mais antigas primeiro)
void listar_reservas(ListaReservas lista, ListaReservas lista_espera) {
    printf("Reservas:\n");
    NodoReserva* atual = lista;
    NodoReserva* reservas[100];
    int count = 0;

    while (atual != NULL) {
        reservas[count] = atual;
        count++;
        atual = atual->prox;
    }

    // Ordernar por data e hora
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compare_dates(reservas[j]->reserva.hora, reservas[j + 1]->reserva.hora, reservas[j]->reserva.data, reservas[j + 1]->reserva.data) < 0) {
                NodoReserva* temp = reservas[j];
                reservas[j] = reservas[j + 1];
                reservas[j + 1] = temp;
            }
        }
    }

    //Print reservas ordenadas
    for (int i = 0; i < count; i++) {
        printf("\tCliente: %s\n", reservas[i]->reserva.cliente.nome);
        printf("\tContacto: %s\n", reservas[i]->reserva.cliente.contacto);
        printf("\tHora de Inicio: %02d:%02d\n", reservas[i]->reserva.hora.hora, reservas[i]->reserva.hora.minuto);
        printf("\tData: %02d/%02d/%02d\n", reservas[i]->reserva.data.dia, reservas[i]->reserva.data.mes, reservas[i]->reserva.data.ano);
        printf("\tTipo de Operacao: %s\n\n", reservas[i]->reserva.tipo_operacao == 0 ? "Lavagem" : "Manutencao");
    }

    printf("Pre-reservas:\n");
    atual = lista_espera;
    while (atual != NULL) {
        printf("\tCliente: %s\n", atual->reserva.cliente.nome);
        printf("\tContacto: %s\n", atual->reserva.cliente.contacto);
        printf("\tHora de Inicio: %02d:%02d\n", atual->reserva.hora.hora, atual->reserva.hora.minuto);
        printf("\tData: %02d/%02d/%02d\n", atual->reserva.data.dia, atual->reserva.data.mes, atual->reserva.data.ano);
        printf("\tTipo de Operacao: %s\n\n", atual->reserva.tipo_operacao == 0 ? "Lavagem" : "Manutencao");
        atual = atual->prox;
    }
}

// Função para listar as reservas e pré-reservas de um cliente ordenadas por data (as mais recentes primeiro)
void listar_reservas_cliente(ListaReservas lista, ListaReservas lista_espera, Cliente cliente) {
    printf("Reservas e Pré-reservas para o cliente %s:\n", cliente.nome);

    int count = 0;
    NodoReserva* atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->reserva.cliente.nome, cliente.nome) == 0 && strcmp(atual->reserva.cliente.contacto, cliente.contacto) == 0) {
            count++;
        }
        atual = atual->prox;
    }
    Reserva* reservas1 = malloc(count * sizeof(Reserva));
    int i = 0;
    atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->reserva.cliente.nome, cliente.nome) == 0 && strcmp(atual->reserva.cliente.contacto, cliente.contacto) == 0) {
            reservas1[i++] = atual->reserva;
        }
        atual = atual->prox;
    }

    // Ordenar por data e hora
    for (int j = 0; j < count - 1; j++) {
        for (int k = 0; k < count - j - 1; k++) {
            if (compare_dates(reservas1[k].hora, reservas1[k + 1].hora, reservas1[k].data, reservas1[k + 1].data) > 0) {
                Reserva temp = reservas1[k];
                reservas1[k] = reservas1[k + 1];
                reservas1[k + 1] = temp;
            }
        }
    }

    // Print das reservas
    printf("Reservas:\n");
    for (int j = 0; j < count; j++) {
        printf("Hora de Inicio: %02d:%02d\n", reservas1[j].hora.hora, reservas1[j].hora.minuto);
        printf("Data: %02d/%02d/%02d\n", reservas1[j].data.dia, reservas1[j].data.mes,reservas1[j].data.ano);
        printf("Tipo de Operacao: %s\n\n", reservas1[j].tipo_operacao == 0 ? "Lavagem" : "Manutencao");
    }

    // Print das pre-reservas
    printf("Pre-Reservas:\n");
    atual = lista_espera;
    while (atual != NULL) {
        if (strcmp(atual->reserva.cliente.nome, cliente.nome) == 0 && strcmp(atual->reserva.cliente.contacto, cliente.contacto) == 0) {
            printf("Hora de Inicio: %02d:%02d (Pre-reserva)\n", atual->reserva.hora.hora, atual->reserva.hora.minuto);
            printf("Data: %02d/%02d/%02d (Pre-reserva)\n", atual->reserva.data.dia, atual->reserva.data.mes, atual->reserva.data.ano);
            printf("Tipo de Operacao: %s (Pre-reserva)\n\n", atual->reserva.tipo_operacao == 0 ? "Lavagem" : "Manutencao");
        }
        atual = atual->prox;
    }

    free(reservas1);
}

void parseFile(char file[1024], ListaReservas *lista) {
    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    char line[100];
    Cliente cliente;
    Hora hora;
    Data data;
    int tipo_operacao;
    while (fgets(line, 1024, fp) != NULL) {
        char *name, *contacto, *matricula, *minuto_ini, *hora_ini, *dia, *mes, *ano, *type;
        name = strtok(line, ";");
        strcpy(cliente.nome, name);
        contacto = strtok(NULL, ";");
        strcpy(cliente.contacto, contacto);
        matricula = strtok(NULL, ";");
        strcpy(cliente.matricula, matricula);
        hora_ini = strtok(NULL, ";");
        hora.hora = atoi(hora_ini);
        minuto_ini = strtok(NULL, ";");
        hora.minuto = atoi(minuto_ini);
        dia = strtok(NULL, ";");
        data.dia = atoi(dia);
        mes = strtok(NULL, ";");
        data.mes = atoi(mes);
        ano = strtok(NULL, ";");
        data.ano = atoi(ano);
        type = strtok(NULL, "\n");
        tipo_operacao = atoi(type);
        reservar(lista, cliente, data, hora, tipo_operacao);
    }
    fclose(fp);
}

//Funcao que verifica se uma reserva ja passou da hora e data do presente
void cancelPassedReservations(ListaReservas* lista_reservas, int hour, int minute, int day, int mon, int year) {
    ListaReservas current = *lista_reservas;
    ListaReservas prev = NULL;

    while (current != NULL) {
        // Comparar a horafim e a data com a atual hora e data
        if (year > current->reserva.data.ano ||
            (year == current->reserva.data.ano && mon > current->reserva.data.mes) ||
            (year == current->reserva.data.ano && mon == current->reserva.data.mes && day > current->reserva.data.dia) ||
            (year == current->reserva.data.ano && mon == current->reserva.data.mes && day == current->reserva.data.dia &&
             (hour > current->reserva.horafim.hora || (hour == current->reserva.horafim.hora && minute >= current->reserva.horafim.minuto)))) {
            // Se a atual passou entao esta reserva é removida
            if (prev == NULL) {
                printf("Data ultrapassada!\n");
                *lista_reservas = current->prox;
                excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", current->reserva.hora.hora, current->reserva.hora.minuto, current->reserva.data.dia, current->reserva.data.mes, current->reserva.data.ano);
                free(current);
                current = *lista_reservas;
            } else {
                printf("Data ultrapassada!\n");
                prev->prox = current->prox;
                excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\reservas.txt", current->reserva.hora.hora, current->reserva.hora.minuto, current->reserva.data.dia, current->reserva.data.mes, current->reserva.data.ano);
                free(current);
                current = prev->prox;
            }
        } else {
            prev = current;
            current = current->prox;
        }
    }
}

//Funcao que verifica se uma pre-reserva ja passou da hora e data presente
void cancelPassedPreReservations(ListaReservas* lista_espera, int hour, int minute, int day, int mon, int year) {
    ListaReservas current = *lista_espera;
    ListaReservas prev = NULL;

    while (current != NULL) {
        // Comparar a horafim e a data com a atual hora e data
        if (year > current->reserva.data.ano ||
            (year == current->reserva.data.ano && mon > current->reserva.data.mes) ||
            (year == current->reserva.data.ano && mon == current->reserva.data.mes && day > current->reserva.data.dia) ||
            (year == current->reserva.data.ano && mon == current->reserva.data.mes && day == current->reserva.data.dia &&
             (hour > current->reserva.horafim.hora || (hour == current->reserva.horafim.hora && minute >= current->reserva.horafim.minuto)))) {
            // Se a atual passou entao esta pre-reserva é removida
            if (prev == NULL) {
                printf("Data ultrapassada!\n");
                *lista_espera = current->prox;
                excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\listaEspera.txt", current->reserva.hora.hora, current->reserva.hora.minuto, current->reserva.data.dia, current->reserva.data.mes, current->reserva.data.ano);
                free(current);
                current = *lista_espera;
            } else {
                printf("Data ultrapassada!\n");
                prev->prox = current->prox;
                excludeReservation("C:\\Users\\ruico\\OneDrive\\Ambiente de Trabalho\\PPP_project\\Projeto PPP 22_23\\listaEspera.txt", current->reserva.hora.hora, current->reserva.hora.minuto, current->reserva.data.dia, current->reserva.data.mes, current->reserva.data.ano);
                free(current);
                current = prev->prox;
            }
        } else {
            prev = current;
            current = current->prox;
        }
    }
}