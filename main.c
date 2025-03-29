#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"

typedef struct possible_next_node{
    int pn_size;
    int *poss_next;
    int pn_nr;
} pn;

// int n = 12; // ilość wierzchołków w grafie

void print_graph(node *graph, int n){ // wypisuje graf
    int i, j;
    for(i = 0; i < n; i++){
        printf("%d: ", graph[i].idx); // wypisuje wierzchołek
        int amount = graph[i].neighbors_count; // ilość sąsiadów
        for(j = 0; j < amount; j++){
            printf("%d, ", graph[i].neighbors[j]); // wypisuje każdego sąsiada
        }
        printf("nr of nodes: %d\n", amount); // wypisuje ilość sąsiadów
    }
    printf("\n");
}

node min_node(node *graph, int n){ // znajduje wierzchołek, posiadający najmniejszą ilość krawędzi z całego grafu (w przypadku wielu takich, zwraca wierzchołek o najmniejszym indeksie)
    int i;
    int min = n+1;
    node min_n;
    for(i = 0; i < n; i++){
        if(graph[i].neighbors_count != -1 && graph[i].neighbors_count < min){ // jeżeli wierzchołek istnieje i ilość sąsiadów danego wierzchołka jest mniejsza od najmniejszej do tej pory znalezionej
            min = graph[i].neighbors_count; // przypisuje aktualną najmniejszą ilość krawędzi do min
            min_n = graph[i]; // szukany wierzchołek
        }
    }
    // printf("min_node: %d, neighbors_count: %d\n", min_n.idx, min_n.neighbors_count);
    return min_n; // zwraca znaleziony wierzchołek, o najmniejszej ilości sąsiadów
}

int last_min(pn ps_next, node *graph, int n){
    if(ps_next.pn_nr == 0) return -1;
    
    // printf("\n\nHEY!!\n\n");
    int i = 0, min = n+1, min_idx = -1;
    for(; i < ps_next.pn_nr; i++){
        if(graph[ps_next.poss_next[i]].neighbors_count < min){
            min = graph[ps_next.poss_next[i]].neighbors_count;
            min_idx = ps_next.poss_next[i];
        }
    }
    return min_idx;
}

node min_neighbour(node start_node, node last_min, node *graph, int n, pn *ps_next){ // znajduje wierzchołek o najmniejszej ilości krawędzi spośród sąsiadów wierzchołka start_node
    node min_n;
    int min;
    if(last_min.idx == -1){
        min = n+1;
    }else{
        // printf("\n\nyay!\n\n");
        min = last_min.neighbors_count;
        min_n = last_min;
    }
    int i;
    for(i = 0; i < start_node.neighbors_count; i++){
        if(ps_next->pn_nr >= ps_next->pn_size){
            ps_next->pn_size *= 2;
            ps_next->poss_next = realloc(ps_next->poss_next, ps_next->pn_size * sizeof(int));
            if (!ps_next->poss_next) {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
        }
        ps_next->poss_next[ps_next->pn_nr] = start_node.neighbors[i];
        ps_next->pn_nr++;
        printf("ps_next->pn_nr: %d\n", ps_next->pn_nr);
        if( graph[start_node.neighbors[i]].neighbors_count != -1 && graph[start_node.neighbors[i]].neighbors_count < min ){
            min = graph[start_node.neighbors[i]].neighbors_count;
            min_n = graph[start_node.neighbors[i]];
        }
    }
    return min_n;
}

void calculate_nr_of_nodes_in_parts(int* part_node_nr, int parts_amount, int n){ // wylicza, ile ma być wierzchołków w każdej części grafu
    int i;
    int how_much_to_add = n % parts_amount;
    for(i = 0; i < parts_amount; i++){
        part_node_nr[i] = n / parts_amount;
        if(i < how_much_to_add) part_node_nr[i]++;
    }
}

void delete_node(node *graph, node n, int node_amount, pn *ps_next){ // usuwa wierzchołek (też z listy sąsiadów innych wierzchołków)
    // printf("n number: %d\n", n.idx);
    graph[n.idx].neighbors_count = -1;
    int i, j, k;
    for(i = 0; i < node_amount; i++){
        for(j = 0; j < graph[i].neighbors_count; j++){
            if(graph[i].neighbors[j] == n.idx){
                k = j;
                for(; k < graph[i].neighbors_count - 1; k++){
                    graph[i].neighbors[k] = graph[i].neighbors[k+1]; // przesuwa sąsiadów w liście sąsiadów tak, aby te które jeszcze nie są usunięte były na początku
                }
                graph[i].neighbors[k] = graph[i].neighbors[j]; // ostatni sąsiad, który nie będzie już widoczny to ten, który usuneliśmy
                j = k;
                graph[i].neighbors_count--; // zmniejsza ilość sąsiadów o 1
            }
        }
    }
    i=0;
    while(i < ps_next->pn_nr){
        if(n.idx == ps_next->poss_next[i]){
            k = i;
            for(; k < ps_next->pn_nr - 1; k++){
                ps_next->poss_next[k] = ps_next->poss_next[k+1];
            }
            ps_next->pn_nr--;
        }else i++;
    }
}

void del_from_pn(pn *ps_next, int idx){
    int i = 0, k;
    while(i < ps_next->pn_nr){
        if(idx == ps_next->poss_next[i]){
            k = i;
            for(; k < ps_next->pn_nr - 1; k++){
                ps_next->poss_next[k] = ps_next->poss_next[k+1];
            }
            ps_next->pn_nr--;
        }else i++;
    }
}

void divide(node *graph, int parts_amount, int *graph_parts[parts_amount], int* part_node_nr, int n){ // dzieli graf
    // node start_node = min_node(graph);
    // printf("start: %d\n", start_node.idx);

    
    calculate_nr_of_nodes_in_parts(part_node_nr, parts_amount, n);

    int i = 0;
    for(; i < parts_amount; i++){
        graph_parts[i] = malloc(part_node_nr[i] * sizeof(int));
    }
    
    int curr_node_amount = 1; // ilość wierzchołków w aktualnej grupie grupie
    
    pn ps_next;
    ps_next.pn_size = 10;
    ps_next.poss_next = malloc(ps_next.pn_size * sizeof(int));
    ps_next.pn_nr = 0;

    pn *ps_next_ptr = &ps_next;

    node l_min;
    int lst_idx;

    node next, start_node;
    for(i = 0; i < parts_amount; i++){
        start_node = min_node(graph, n); // pobiera pierwszy "początkowy" wierzchołek dla i-tej grupy
        graph_parts[i][0] = start_node.idx;
        printf("i, 0: %d\n", graph_parts[i][0]);
        printf("start: %d\n", start_node.idx);
        printf("i: %d, parts_mount: %d \n\n", i, parts_amount);
        for(curr_node_amount = 1; curr_node_amount < part_node_nr[i]; curr_node_amount++){ // powtarza tyle razy ile ma być wierzchołków w grupie
            lst_idx = last_min(ps_next, graph, n);
            if(lst_idx == -1) l_min.idx = -1; // nie ma żadnych
            else{ 
                l_min = graph[lst_idx];
                printf("lmin: %d\n", lst_idx);
            }
            printf("ps_next.pn_nr: %d\n", ps_next.pn_nr);
            next = min_neighbour(start_node, l_min, graph, n, ps_next_ptr); // znajduje następny wierzchołek, który zostanie dodany do grupy
            del_from_pn(ps_next_ptr, next.idx);
            printf("ps_next.pn_nr: %d\n", ps_next.pn_nr);
            graph_parts[i][curr_node_amount] = next.idx;
            printf("next: %d\n", next.idx);
            delete_node(graph, start_node, n, ps_next_ptr); // usuwa ostatni wierzchołek
            // print_graph(graph);
            start_node = next;
        }
        printf("new part: \n\n");
        delete_node(graph, start_node, n, ps_next_ptr);
        // print_graph(graph);
    }
}

int main(){
    FILE *file = fopen("graf.csrrg", "r");
    FILE *file2 = fopen("graf.csrrg", "r");

    int node_amount = skip_to(file, file2);

    node* graph = malloc(node_amount * sizeof(node)); // tworzy graf - tablicę wierzchołków
    create_graph(graph, file, file2, node_amount);
    print_graph(graph, node_amount);

    int parts_amount = 4, margin = 10; // ilość części i margines

    int *graph_parts[parts_amount];
    int* part_node_nr = malloc(parts_amount * sizeof(int)); // ilość wierzchołków w grupie

    divide(graph, parts_amount, graph_parts, part_node_nr, node_amount); // dzieli graf

    int i, j;
    for(i = 0; i < parts_amount; i++){
        printf("Grupa %d: ", i);
        for(j = 0; j < part_node_nr[i]; j++){
            printf("%d, ", graph_parts[i][j]);
        }printf("\n");
    }

    return 0;
}