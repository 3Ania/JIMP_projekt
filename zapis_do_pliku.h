#ifndef ZAPIS_DO_PLIKU_H
#define ZAPIS_DO_PLIKU_H

#include <stdio.h>
#include <stdlib.h>
#include "pobieranie_z_pliku.h"

// zamienia int na binarny (char*)
char *to_binary(int num);

// sprawdza, czy wierzcholek o indeksie node jest w danej partycji graph_parts
int is_in_part(int node, int parts_amount, int part_node_nr, int graph_parts[]);

// kopiuje nistniejacy graf
node* copy_graph(node* graph, node* graph_for_print, int node_amount);

// usuwa niepotrzebne krawedzie miedzy wierzcholkami z roznych partycji
void delete_edges(node* graph_for_print, int parts_amount, int *part_node_nr, int *graph_parts[]);

// soruje partycje, aby w kazdej czesci byly wierzcholki od tych o najmniejszym do tych o najwiekszym indeksie
void sort_parts(int parts_amount, int* part_node_nr, int *graph_parts[]);

// zapisuje do pliku
void write_to_file(FILE* file3, int node_amount, int parts_amount, int* part_node_nr, node *graph_for_print, int *graph_parts[]);

#endif