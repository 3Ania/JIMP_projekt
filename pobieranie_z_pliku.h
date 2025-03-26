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

int skip_to(FILE *plf_line3, FILE *f_line4);

void create_graph(node *graph, FILE *file, FILE *file2, int node_amount);

#endif