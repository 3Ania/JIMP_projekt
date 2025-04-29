#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"
#include "algorytm.h"

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

int not_in(int idx, possible_next_node *pn){ // sprawdza, czy idx znajduje się w tablicy ze strury pn
    int i;
    int is_in = 1;
    for(i = 0; i < pn->idx; i++){ // przechodzi po wszystkich elementach tablicy
        if(idx == pn->poss_next[i]){ // porównuje
            is_in = 0; // jeżeli znajdzie, zwraca fałsz (znaleziono)
            i = pn->idx;
        }
    }
    return is_in; // zwraca wynik
}

node min_neighbour(node start_node, node *graph, int n, possible_next_node *pn){ // znajduje wierzchołek o najmniejszej ilości krawędzi spośród sąsiadów wierzchołka start_node
    int min = n+1;
    node min_n;
    int pn_idx;

    int i;
    for(i = 0; i < start_node.neighbors_count; i++){ // przechodzi po wszystkich sąsiadach wierzchołka start_node
        if(not_in(graph[start_node.neighbors[i]].idx, pn) && graph[start_node.neighbors[i]].neighbors_count != -1 && graph[start_node.neighbors[i]].idx != start_node.idx) { // jeżeli wierzchołek nie jest w pn, nie został jeszcze usunięty i nie jest to start_node, to wchodzi
            pn->poss_next[pn->idx] = graph[start_node.neighbors[i]].idx; // dopisuje nową mozliwosc do pn
            pn->idx += 1; // zwieksza indeksowanie
        }
        if(pn->idx >= pn->amount){ // realokacja pamięci
            pn->amount *= 2;
            pn->poss_next = realloc(pn->poss_next, pn->amount * sizeof(int));
        }
    }

    for(i = 0; i < pn->idx; i++){ // przechodzi po wszystkich dostepnych wierzcholkach
        if( pn->poss_next[i] != -1 && graph[pn->poss_next[i]].neighbors_count < min ){ // znajduje wierzchołek który ma aktualnie najmniej sąsiadow
            min = graph[pn->poss_next[i]].neighbors_count;
            min_n = graph[pn->poss_next[i]];
            pn_idx = i;
        }
    }

    pn->poss_next[pn_idx] = -1; // usuwa wybrany wierzcholek z listy

    return min_n; // zwraca znaleziony wirzcholek
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

void calculate_nr_of_nodes_in_parts(int* part_node_nr, int parts_amount, int n){ // wylicza, ile ma być wierzchołków w każdej części grafu
    int i;
    int how_much_to_add = n % parts_amount;
    // dzieli ilosc wierzcholkow na grupy, jezeli nie da sie podzielic calkowicie, tworzy jak najmniejsze roznice miedzy czesciami
    for(i = 0; i < parts_amount; i++){
        part_node_nr[i] = n / parts_amount;
        if(i < how_much_to_add) part_node_nr[i]++;
    }
}

void divide(node *graph, int parts_amount, int *graph_parts[parts_amount], int* part_node_nr, int n){ // dzieli graf
    calculate_nr_of_nodes_in_parts(part_node_nr, parts_amount, n); // oblicza ilosc wierzcholkow dla kazdej czesci grafu

    int i = 0;
    for(; i < parts_amount; i++){
        graph_parts[i] = malloc(part_node_nr[i] * sizeof(int)); // mallocuje pamiec dla grafu
    }
    
    int curr_node_amount = 1; // ilość wierzchołków w aktualnej grupie grupie

    // tworzy startowe, puste pn
    possible_next_node pn;
    pn.amount = 10;
    pn.idx = 0;
    pn.poss_next = malloc(pn.amount * sizeof(int));

    node next, start_node;
    for(i = 0; i < parts_amount; i++){ // przechodzi po wszystkich czesciach
        start_node = min_node(graph, n, &pn); // pobiera pierwszy "początkowy" wierzchołek dla i-tej grupy
        graph_parts[i][0] = start_node.idx; // zapisuje znaleziony wierzcholek do danej czesci
        for(curr_node_amount = 1; curr_node_amount < part_node_nr[i]; curr_node_amount++){ // powtarza tyle razy ile ma być wierzchołków w grupie
            next = min_neighbour(start_node, graph, n, &pn); // znajduje następny wierzchołek, który zostanie dodany do grupy
            graph_parts[i][curr_node_amount] = next.idx; // zapisuje wierzcholek do partycji
            delete_node(graph, start_node, n); // usuwa ostatni wierzchołek
            start_node = next; // teraz ostatnim wierzcholkiem jest kolejny znaleziony
        }
        delete_node(graph, start_node, n); // usuwa ostatni, nieusuniety wierzcholek
    }
}