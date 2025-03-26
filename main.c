#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"

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
    return min_n; // zwraca znaleziony wierzchołek, o najmniejszej ilości sąsiadów
}

node min_neighbour(node start_node, node *graph, int n){ // znajduje wierzchołek o najmniejszej ilości krawędzi spośród sąsiadów wierzchołka start_node
    int min = n+1;
    node min_n;
    int i;
    for(i = 0; i < start_node.neighbors_count; i++){
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

void delete_node(node *graph, node n, int node_amount){ // usuwa wierzchołek (też z listy sąsiadów innych wierzchołków)
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

    node next, start_node;
    for(i = 0; i < parts_amount; i++){
        start_node = min_node(graph, n); // pobiera pierwszy "początkowy" wierzchołek dla i-tej grupy
        graph_parts[i][0] = start_node.idx;
        // printf("i, 0: %d\n", graph_parts[i][0]);
        // printf("start: %d\n", start_node.idx);
        // printf("i: %d, parts_mount: %d \n\n", i, parts_amount);
        for(curr_node_amount = 1; curr_node_amount < part_node_nr[i]; curr_node_amount++){ // powtarza tyle razy ile ma być wierzchołków w grupie
            next = min_neighbour(start_node, graph, n); // znajduje następny wierzchołek, który zostanie dodany do grupy
            graph_parts[i][curr_node_amount] = next.idx;
            // printf("next: %d\n", next.idx);
            delete_node(graph, start_node, n); // usuwa ostatni wierzchołek
            // print_graph(graph);
            start_node = next;
        }
        // printf("new part: \n\n");
        delete_node(graph, start_node, n);
        // print_graph(graph);
    }
}

int main(){
    FILE *file = fopen("graf.csrrg", "r");
    FILE *file2 = fopen("graf.csrrg", "r");

    int node_amount = skip_to(file, file2);
    printf("%d\n\n", node_amount);

    node* graph = malloc(node_amount * sizeof(node)); // tworzy graf - tablicę wierzchołków
    create_graph(graph, file, file2, node_amount);
    printf("\ngraph[4].neighbors[0]: %d\n", graph[4].neighbors[0]);
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