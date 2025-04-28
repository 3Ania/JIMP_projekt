#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"
#include "zapis_do_pliku.h"

int is_in_part(int node, int parts_amount, int part_node_nr, int graph_parts[parts_amount]){ // sprawdza, czy node jest w danej grupie
    int is_in = 0;
    int i;

    for(i = 0; i < part_node_nr; i++){ // przechodzi po wierzcholkach w grupie
        if(node == graph_parts[i]){ // node jest w grupie
            is_in = 1;
            i = part_node_nr;
        }
    }
    return is_in; // zwraca wynik
}

node* copy_graph(node* graph, node* graph_for_print, int node_amount){ // kopiuje graf
    int i, j;

    for(i = 0; i < node_amount; i++){ // przechodzi po wszystkich wierzcholkach z grafu
        // kopiuje wszystkie informacje o wierzcholku
        graph_for_print[i].idx = graph[i].idx;
        graph_for_print[i].neighbor_idx = graph[i].neighbor_idx;
        graph_for_print[i].neighbors_count = graph[i].neighbors_count;
        graph_for_print[i].neighbors = malloc(graph[i].neighbors_count * sizeof(int)); // alokuje pamiec dla tablicy
        for(j = 0; j < graph[i].neighbors_count; j++){ // kopiuje wszystkich sasiadow
            graph_for_print[i].neighbors[j] = graph[i].neighbors[j];
        }
    }
    return graph_for_print; // zwraca skopiowany graf
}

void delete_edges(node* graph_for_print, int parts_amount, int *part_node_nr, int *graph_parts[parts_amount]){ // usuwa niepotrzebne krawedze
    int i, j, k;

    for(i = 0; i < parts_amount; i++){
        for(j = 0; j < part_node_nr[i]; j++){
            for(k = 0; k < graph_for_print[graph_parts[i][j]].neighbors_count; k++){
                if(!is_in_part(graph_for_print[graph_parts[i][j]].neighbors[k], parts_amount, part_node_nr[i], graph_parts[i])){ // wierzcholek nie jest w danej partycji grafu
                    graph_for_print[graph_parts[i][j]].neighbors[k] = -1; // usuwa wierzcholek z listy sąsiadow
                }
            }
        }
    }
}

void sort_parts(int parts_amount, int* part_node_nr, int *graph_parts[parts_amount]){ // sortuje czesci, na ktore graf zostal podzielony
    int i, j, k, temp;
    for(i = 0; i < parts_amount; i++){ // przechodzi po wszystkich partycjach
        for(j = 0; j < part_node_nr[i]; j++){ // przechodzi po wszystkich wierzcholkach w danej grupie 
            for(k = j; k < part_node_nr[i]; k++){ 
                if(graph_parts[i][j] > graph_parts[i][k]){ // zamiana wierzcholkow, dzieki czemu są posortowane
                    temp = graph_parts[i][j];
                    graph_parts[i][j] = graph_parts[i][k];
                    graph_parts[i][k] = temp;
                }
            }
        }
    }
}

void write_to_file(FILE* file3, int node_amount, int parts_amount, int* part_node_nr, node *graph_for_print, int *graph_parts[parts_amount]){ // zapisuje do pliku
    int i, j, k, l;

    int size_l5 = node_amount; // wielkosc dla tablicy char line5
    char *line5 = malloc(size_l5 * sizeof(char)); // alokacja pamieci dla line5
    int idx = 0, i2, x = 0;
    int number = 0, n2;
    char temp;
    for(i = 0; i < parts_amount; i++){ // przechodzi po wszystkich partycjach
        for(j = 0; j < part_node_nr[i]; j++){ // przechodzi po wierzcholkach w danej grupie
            node node_to_print = graph_for_print[graph_parts[i][j]]; // zapisuje aktualny node do wypisania
            fprintf(file3, "%d;", node_to_print.idx); // zapisuje node do pliku

            // pobieranie danych do linijki 5 pliku
            if(idx >= size_l5 - 3){
                size_l5 *= 2;
                line5 = realloc(line5, size_l5 * sizeof(char)); // jezeli potrzeba, reallocuje
            }
            n2 = number;
            i2 = idx;
            x = 0;
            if(n2 == 0){ // dla zera zapisuje 0
                line5[idx] = (char)('0'+(n2%10));
                idx++;
            }
            while(n2 > 0){ // dla innych zapisuje cyfry (jezeli liczba wieksza od 9, zapisuje wiecej niz jedna cyfre)
                line5[idx] = (char)('0'+(n2%10));
                n2 /= 10;
                idx++;
                x++;
            }
            for(l = 0; l < x/2; l++){  // odwraca kolejnosc
                temp = line5[i2 + l];
                line5[i2 + l] = line5[idx -1 -l];
                line5[idx -1 -l] = temp;
            }
            line5[idx] = ';'; // dopisuje po kazdej liczbie średnik
            idx++;
            number++;
            for(k = 0; k < node_to_print.neighbors_count; k++){ // wypisuje w linijce czwartej pliku wszytskich sąsiadów aktualnego wierzcholka
                if(node_to_print.neighbors[k] != -1){
                    fprintf(file3, "%d;", node_to_print.neighbors[k]);
                    number++;
                }
            }
        }
        line5[idx] = '\n';
        idx++;
    }
    fprintf(file3, "\n"); // dopisuje znak nowej linii

    for(i = 0; i < idx; i++){
        fprintf(file3, "%c", line5[i]); // wypisuje piątą linijkę pliku
    }
}