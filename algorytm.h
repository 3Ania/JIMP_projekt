#ifndef ALGORYTM_H
#define ALGORYTM_H

#include <stdio.h>
#include <stdlib.h>
#include "pobieranie_z_pliku.h"

typedef struct possible_next_node{ // struktura przechowująca wszystkie opcje na następny wierzchołek do dodania do partycji
    int *poss_next; // lista możliwych następnych wierzchołków
    int idx; // ostatni indeks
    int amount; // ilość zaalokowanej pamięci
} possible_next_node;

// wypisuje graf
void print_graph(node *graph, int n);

// znajduje wierzcholek o najmniejszej ilosci sasiadow
node min_node(node *graph, int n, possible_next_node *pn);

// sprawdza, czy wierzcholek o indeksie idx znajduje sie w liście w strukturze pn
int not_in(int idx, possible_next_node *pn);

// znajduje wierzcholek sposrod listy mozliwych wierzcholkow ze struktury pn, ktory ma najmniejsza ilosc sasiadow
node min_neighbour(node start_node, node *graph, int n, possible_next_node *pn);

// usuwa wierzcholki, ktore zostaly juz dodane do jakiejs partycji
void delete_node(node *graph, node n, int node_amount);

// wylicza ile ma byc wierzcholkow w kazdej partycji
void calculate_nr_of_nodes_in_parts(int* part_node_nr, int parts_amount, int n);

// ogolna funkcja dzielaca graf
void divide(node *graph, int parts_amount, int *graph_parts[parts_amount], int* part_node_nr, int n);

#endif