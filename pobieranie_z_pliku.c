#include "pobieranie_z_pliku.h"

#include <stdio.h>
#include <stdlib.h>
#include "zapis_do_pliku.h"

int skip_to(FILE *f_line3, FILE *f_line4, FILE *file3, int is_binary){ // przwesuwa wskaźniki na dobre miejsca i przepisuje pierwsze 
    int target_line3 = 3;
    int target_line4 = 4;
    int curr_line = 0;
    int node_amount = 1;

    char chr = getc(f_line3);
    char chr2 = getc(f_line4);
    // przepisuje do pliku wyjściowego
    if(!is_binary) putc(chr2, file3);

    int size = 100, idx = 1, temp_int;
    char *str = malloc(size * sizeof(char));
    char *bin = NULL;
    str[0] = chr;

    while(chr != EOF && curr_line < target_line3){
        if(chr == '\n') curr_line++;
        if(curr_line == 1 && chr == ';'){
            node_amount++;
        }
        if(curr_line == 3 && chr == '\n');
        else {
            chr = getc(f_line3);
            if(!is_binary) {
                putc(chr, file3);
            }
            else{
                if(chr == ';'){
                    str[idx] = '\0';
                    temp_int = atoi(str);
                    free(str);
                    idx = 0;
                    str = malloc(size * sizeof(char));
                    bin = malloc(temp_int * 10 * sizeof(char));
                    bin = to_binary(temp_int);
                    fprintf(file3, "%s;", bin);
                    free(bin);
                }else if(chr == '\n'){
                    str[idx] = '\0';
                    temp_int = atoi(str);
                    free(str);
                    idx = 0;
                    str = malloc(size * sizeof(char));
                    bin = malloc(temp_int * 10 * sizeof(char));
                    bin = to_binary(temp_int);
                    fprintf(file3, "%s\n", bin);
                    free(bin);
                }else{
                    if(idx >= size){
                        size *= 2;
                        str = realloc(str, size*sizeof(char));
                    }
                    str[idx] = chr;
                    idx++;
                    str[idx] = '\0';
                }
            }
        }
        chr2 = getc(f_line4);
    }
    chr2 = getc(f_line4);
    while(chr2 != EOF && chr2 != '\n') {chr2 = getc(f_line4);}
    // oba wskaźniki na dobrych miejscach i powinny być przepisane 3 pierwsze linijki

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
        for(; i < buff1; i++){
            if(fscanf(f_line3, "%d;", &buff2) == 1){
                if(first == 0){
                    idx = buff2;
                    graph[idx].idx = buff2;
                    first++;
                    if(graph[idx].neighbors_count == 0) graph[idx].neighbors = malloc(buff1 * sizeof(int));
                    else graph[idx].neighbors = realloc(graph[idx].neighbors, (graph[idx].neighbors_count + buff1) * sizeof(int));
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
            }else{
                printf("Error\n");
            }
        }
    }

    //pobieranie ostatniego wierzchołka i jego sąsiadów
    first = 0;
    int next_char;
    while (fscanf(f_line3, "%d;", &buff2) == 1) {
        if (first == 0) {
            idx = buff2;
            graph[idx].idx = buff2;
            first++;
            if (graph[idx].neighbors_count==0) graph[idx].neighbors = malloc(10 * sizeof(int));
        } else {
            graph[idx].neighbors[graph[idx].neighbor_idx] = buff2;
            graph[idx].neighbors_count++;
            if(graph[buff2].neighbors_count == 0) {
                graph[buff2].neighbors = malloc(10 * sizeof(int));
            } else if(graph[buff2].neighbor_idx > graph[buff2].neighbors_count) {
                graph[buff2].neighbors = realloc(graph[buff2].neighbors, 
                    graph[buff2].neighbors_count * 2 * sizeof(int));
            }
            graph[buff2].neighbors[graph[buff2].neighbor_idx] = idx;
            graph[buff2].neighbors_count++;
            graph[idx].neighbor_idx++;
            graph[buff2].neighbor_idx++;
        }
        next_char = fgetc(f_line3);
        if (next_char == '\n' || next_char == EOF) {
            break;
        } else {
            ungetc(next_char, f_line3); // Jeśli to nie '\n', zwróć znak na strumień
        }
    }

}