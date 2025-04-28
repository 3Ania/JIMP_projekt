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

void print_graph(node *graph, int n);

node min_node(node *graph, int n, possible_next_node *pn);

int not_in(int idx, possible_next_node *pn);

node min_neighbour(node start_node, node *graph, int n, possible_next_node *pn);

void delete_node(node *graph, node n, int node_amount);

void calculate_nr_of_nodes_in_parts(int* part_node_nr, int parts_amount, int n);

void divide(node *graph, int parts_amount, int *graph_parts[parts_amount], int* part_node_nr, int n);

#endif