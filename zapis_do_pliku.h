#ifndef ZAPIS_DO_PLIKU_H
#define ZAPIS_DO_PLIKU_H

#include <stdio.h>
#include <stdlib.h>
#include "pobieranie_z_pliku.h"

char *to_binary(int num);

int is_in_part(int node, int parts_amount, int part_node_nr, int graph_parts[]);

node* copy_graph(node* graph, node* graph_for_print, int node_amount);

void delete_edges(node* graph_for_print, int parts_amount, int *part_node_nr, int *graph_parts[]);

void sort_parts(int parts_amount, int* part_node_nr, int *graph_parts[]);

void write_to_file(FILE* file3, int node_amount, int parts_amount, int* part_node_nr, node *graph_for_print, int *graph_parts[]);

#endif