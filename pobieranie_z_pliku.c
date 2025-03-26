#include "pobieranie_z_pliku.h"

#include <stdio.h>
#include <stdlib.h>

int skip_to(FILE *f_line3, FILE *f_line4){ // przwesuwa wskaźniki na dobre miejsca
    int target_line3 = 3;
    int target_line4 = 4;
    int curr_line = 0;
    int node_amount = 1;

    char chr = getc(f_line3);
    char chr2 = getc(f_line4);
    while(chr != EOF && curr_line < target_line3){
        if(chr == '\n') curr_line++;
        if(curr_line == 1 && chr == ';'){
            node_amount++;
        }
        if(curr_line == 3 && chr == '\n');
        else chr = getc(f_line3);
        chr2 = getc(f_line4);
    }
    chr2 = getc(f_line4);
    while(chr2 != EOF && chr2 != '\n') chr2 = getc(f_line4);
    // oba wskaźniki na dobrych miejscach

    return node_amount;
}

void create_graph(node *graph, FILE *f_line3, FILE *f_line4, int node_amount){ // tworzy przykładowy graf

    int i;
    for(i = 0; i < node_amount; i++){
        graph[i].idx = i;
        graph[i].neighbor_idx = 0;
        graph[i].neighbors_count = 0;
    }

    //pobieram pierwszą wartość - 0, której nie potrzebuje potem
    int x;
    fscanf(f_line4, "%d;", &x);

    int buff1, buff2, idx = 0, first = 0;
    i = 0;

    while(fscanf(f_line4, "%d;", &buff1) == 1){
        first = 0;
        if(graph[idx].neighbors_count == 0) graph[idx].neighbors = malloc(buff1 * sizeof(int));
        else graph[idx].neighbors = realloc(graph[idx].neighbors, (graph[idx].neighbors_count + buff1) * sizeof(int));
        for(; i < buff1; i++){
            if(idx == 4) printf(".");
            if(fscanf(f_line3, "%d;", &buff2) == 1){
                if(first == 0){
                    graph[idx].idx = buff2;
                    if(idx == 4) printf("(%d) - ", graph[idx].idx);
                    first++;
                }else{
                    graph[idx].neighbors[graph[idx].neighbor_idx] = buff2;
                    graph[idx].neighbors_count++;
                    if(graph[buff2].neighbors_count == 0){
                        graph[buff2].neighbors = malloc(10 * sizeof(int));
                    }else if(graph[buff2].neighbor_idx > graph[buff2].neighbors_count){
                        graph[buff2].neighbors = realloc(graph[buff2].neighbors, graph[buff2].neighbors_count * 2 * sizeof(int));
                    }
                    graph[buff2].neighbors[graph[buff2].neighbor_idx] = idx;
                    graph[buff2].neighbors_count++;
                    graph[idx].neighbor_idx++;
                    graph[buff2].neighbor_idx++;
                }
            }
        }

        idx++;
    }
}
