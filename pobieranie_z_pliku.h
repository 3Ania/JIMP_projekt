#ifndef POBIERANIE_Z_PLIKU_H
#define POBIERANIE_Z_PLIKU_H

#include <stdio.h>
#include <stdlib.h>

// struktura posiadająca pojedyńczy wierzchołek
typedef struct node{
    int idx; // indeks (numer) wierzchołka
    int neighbors_count; // ilość sąsiadów (krawędzi) danego wierzchołka
    int* neighbors; // lista sąsiadów (indeksy sąsiadów)
    int neighbor_idx;
} node;

// przeskakuje do odpowiednich miejsc w pliku (dla line3 do 3 linijki, dla line4 do czwartej)
int skip_to(FILE *plf_line3, FILE *f_line4, FILE *file3);

// tworzy graf na postawie pliku
void create_graph(node *graph, FILE *file, FILE *file2, int node_amount);

#endif