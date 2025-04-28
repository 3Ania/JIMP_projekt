#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"

typedef struct possible_next_node{
    int *poss_next;
    int idx;
    int amount;
} possible_next_node;

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

node min_node(node *graph, int n, possible_next_node *pn){ // znajduje wierzchołek, posiadający najmniejszą ilość krawędzi z całego grafu (w przypadku wielu takich, zwraca wierzchołek o najmniejszym indeksie)
    int i;
    int min = n+1;
    node min_n;
    for(i = 0; i < n; i++){
        if(graph[i].neighbors_count != -1 && graph[i].neighbors_count < min){ // jeżeli wierzchołek istnieje i ilość sąsiadów danego wierzchołka jest mniejsza od najmniejszej do tej pory znalezionej
            min = graph[i].neighbors_count; // przypisuje aktualną najmniejszą ilość krawędzi do min
            min_n = graph[i]; // szukany wierzchołek
        }
    }
    for(i = 0; i < pn->idx; i++){
        pn->poss_next[i] = -1; // usuwanie wszystkich wartości z mozliwych kolejnych wierzcholkow
    }
    pn->idx = 0; // od nowa tablica dla pn

    return min_n; // zwraca znaleziony wierzchołek, o najmniejszej ilości sąsiadów
}

int not_in(int idx, possible_next_node *pn){
    int i;
    int is_in = 1;
    for(i = 0; i < pn->idx; i++){
        if(idx == pn->poss_next[i]){
            is_in = 0;
            i = pn->idx;
        }
    }
    return is_in;
}

node min_neighbour(node start_node, node *graph, int n, possible_next_node *pn){ // znajduje wierzchołek o najmniejszej ilości krawędzi spośród sąsiadów wierzchołka start_node
    int min = n+1;
    node min_n;
    int pn_idx;

    int i;
    for(i = 0; i < start_node.neighbors_count; i++){

        if(not_in(graph[start_node.neighbors[i]].idx, pn) && graph[start_node.neighbors[i]].neighbors_count != -1 && graph[start_node.neighbors[i]].idx != start_node.idx) {
            pn->poss_next[pn->idx] = graph[start_node.neighbors[i]].idx;
            pn->idx += 1;
        }
        if(pn->idx >= pn->amount){
            pn->amount *= 2;
            pn->poss_next = realloc(pn->poss_next, pn->amount * sizeof(int));
        }
    }

    for(i = 0; i < pn->idx; i++){

        if( pn->poss_next[i] != -1 && graph[pn->poss_next[i]].neighbors_count < min ){
            min = graph[pn->poss_next[i]].neighbors_count;
            min_n = graph[pn->poss_next[i]];
            pn_idx = i;
        }
    }

    pn->poss_next[pn_idx] = -1;

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
    calculate_nr_of_nodes_in_parts(part_node_nr, parts_amount, n);

    int i = 0;
    for(; i < parts_amount; i++){
        graph_parts[i] = malloc(part_node_nr[i] * sizeof(int));
    }
    
    int curr_node_amount = 1; // ilość wierzchołków w aktualnej grupie grupie

    possible_next_node pn;
    pn.amount = 10;
    pn.idx = 0;
    pn.poss_next = malloc(pn.amount * sizeof(int));

    node next, start_node;
    for(i = 0; i < parts_amount; i++){
        start_node = min_node(graph, n, &pn); // pobiera pierwszy "początkowy" wierzchołek dla i-tej grupy
        graph_parts[i][0] = start_node.idx;

        for(curr_node_amount = 1; curr_node_amount < part_node_nr[i]; curr_node_amount++){ // powtarza tyle razy ile ma być wierzchołków w grupie
            next = min_neighbour(start_node, graph, n, &pn); // znajduje następny wierzchołek, który zostanie dodany do grupy
            graph_parts[i][curr_node_amount] = next.idx;

            delete_node(graph, start_node, n); // usuwa ostatni wierzchołek

            start_node = next;
        }

        delete_node(graph, start_node, n);
    }
}

int is_in_part(int node, int parts_amount, int part_node_nr, int graph_parts[parts_amount]){
    int is_in = 0;
    int i;

    for(i = 0; i < part_node_nr; i++){
        if(node == graph_parts[i]){ // node jest w grupie
            is_in = 1;
            i = part_node_nr;
        }
    }
    return is_in;
}

node* copy_graph(node* graph, node* graph_for_print, int node_amount){
    int i, j;

    for(i = 0; i < node_amount; i++){
        graph_for_print[i].idx = graph[i].idx;
        graph_for_print[i].neighbor_idx = graph[i].neighbor_idx;
        graph_for_print[i].neighbors_count = graph[i].neighbors_count;
        graph_for_print[i].neighbors = malloc(graph[i].neighbors_count * sizeof(int));
        for(j = 0; j < graph[i].neighbors_count; j++){
            graph_for_print[i].neighbors[j] = graph[i].neighbors[j];
        }
    }
    return graph_for_print;
}

void delete_edges(node* graph_for_print, int parts_amount, int *part_node_nr, int *graph_parts[parts_amount]){
    int i, j, k;

    for(i = 0; i < parts_amount; i++){
        for(j = 0; j < part_node_nr[i]; j++){
            for(k = 0; k < graph_for_print[graph_parts[i][j]].neighbors_count; k++){
                if(!is_in_part(graph_for_print[graph_parts[i][j]].neighbors[k], parts_amount, part_node_nr[i], graph_parts[i])){ // wierzcholek nie jest w danej partycji grafu
                    graph_for_print[graph_parts[i][j]].neighbors[k] = -1;
                }
            }
        }
    }
}

void sort_parts(int parts_amount, int* part_node_nr, int *graph_parts[parts_amount]){
    int i, j, k, temp;
    for(i = 0; i < parts_amount; i++){
        for(j = 0; j < part_node_nr[i]; j++){
            for(k = j; k < part_node_nr[i]; k++){
                if(graph_parts[i][j] > graph_parts[i][k]){
                    temp = graph_parts[i][j];
                    graph_parts[i][j] = graph_parts[i][k];
                    graph_parts[i][k] = temp;
                }
            }
        }
    }
}

void write_to_file(FILE* file3, int node_amount, int parts_amount, int* part_node_nr, node *graph_for_print, int *graph_parts[parts_amount]){
    int i, j, k, l;

    int size_l5 = node_amount;
    char *line5 = malloc(size_l5 * sizeof(char));
    int idx = 0, i2, x = 0;
    int number = 0, n2;
    char temp;
    for(i = 0; i < parts_amount; i++){
        for(j = 0; j < part_node_nr[i]; j++){
            node node_to_print = graph_for_print[graph_parts[i][j]];
            fprintf(file3, "%d;", node_to_print.idx);
            if(idx >= size_l5 - 3){
                size_l5 *= 2;
                line5 = realloc(line5, size_l5 * sizeof(char));
            }
            n2 = number;
            i2 = idx;
            x = 0;
            if(n2 == 0){
                line5[idx] = (char)('0'+(n2%10));
                idx++;
            }
            while(n2 > 0){
                line5[idx] = (char)('0'+(n2%10));
                n2 /= 10;
                idx++;
                x++;
            }
            for(l = 0; l < x/2; l++){
                temp = line5[i2 + l];
                line5[i2 + l] = line5[idx -1 -l];
                line5[idx -1 -l] = temp;
            }
            line5[idx] = ';';
            idx++;
            number++;
            for(k = 0; k < node_to_print.neighbors_count; k++){
                if(node_to_print.neighbors[k] != -1){
                    fprintf(file3, "%d;", node_to_print.neighbors[k]);
                    number++;
                }
            }
        }
        line5[idx] = '\n';
        idx++;
    }
    fprintf(file3, "\n");

    for(i = 0; i < idx; i++){
        fprintf(file3, "%c", line5[i]);
    }
}

int main(int argc, char *argv[]){
    if (argc<2) // brak pliku wejściowego
    {
        printf("Provide a file containing a graph as a parameter.\n");
        return 0;
    }

    char* plik = argv[1];

    int parts_amount = 2, margin = 10;

    if (argc>2) parts_amount = atoi(argv[2]);
    if (argc>3 && argv[3]>=0) margin = atoi(argv[3]);

    FILE *file = fopen(plik, "r");
    FILE *file2 = fopen(plik, "r");
    FILE *file3 = fopen("output.txt", "w");

    if (file == NULL) { // plik z grafem nie istnieje
        printf("File not found.\n");
        return 0;
    } else printf("\nFile: \"%s\"\n", argv[1]);

    int node_amount = skip_to(file, file2, file3);
    printf("\nNode amount: %d\n\n", node_amount);
    if(parts_amount>2 && node_amount<parts_amount) {parts_amount=2;}

    node* graph = malloc(node_amount * sizeof(node)); // tworzy graf - tablicę wierzchołków
    node* graph_for_print = malloc(node_amount * sizeof(node));
    create_graph(graph, file, file2, node_amount);
    printf("Graph:\n\n");
    print_graph(graph, node_amount);

    graph_for_print = copy_graph(graph, graph_for_print, node_amount);

   // int parts_amount = 4, margin = 10; // ilość części i margines

    int *graph_parts[parts_amount];
    int* part_node_nr = malloc(parts_amount * sizeof(int)); // ilość wierzchołków w grupie

    divide(graph, parts_amount, graph_parts, part_node_nr, node_amount); // dzieli graf

    printf("Number of parts: %d\nMarigin: %d\n\n", parts_amount, margin);

    sort_parts(parts_amount, part_node_nr, graph_parts);

    int i, j;
    for(i = 0; i < parts_amount; i++){
        printf("Group %d: ", i);
        for(j = 0; j < part_node_nr[i]; j++){
            printf("%d, ", graph_parts[i][j]);
        }printf("\n\n");
    }printf("\n");

    delete_edges(graph_for_print, parts_amount, part_node_nr, graph_parts);

    //zapisywanie wyniku do pliku

    write_to_file(file3, node_amount, parts_amount, part_node_nr, graph_for_print, graph_parts);

    fclose(file);
    fclose(file2);
    fclose(file3);
}