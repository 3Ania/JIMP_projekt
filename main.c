#include <stdio.h>
#include <stdlib.h>

#include "pobieranie_z_pliku.h"
#include "algorytm.h"
#include "zapis_do_pliku.h"

int main(int argc, char *argv[]){
    if (argc<2) // brak pliku wejściowego
    {
        printf("Provide a file containing a graph as a parameter.\n");
        return 0;
    }

    char* plik = argv[1];

    int parts_amount = 2, margin = 10; // wartosci w przypasku nie podania ich przez uzytkownika

    // jezeli uzytkownik poda parametry, nadpisują się
    if (argc>2) parts_amount = atoi(argv[2]);
    if (argc>3) margin = atoi(argv[3]);

    // pliki do odczytu
    FILE *file = fopen(plik, "r");
    FILE *file2 = fopen(plik, "r");
    // plik do zapisu
    FILE *file3 = fopen("output.txt", "w");

    if (file == NULL) { // plik z grafem nie istnieje
        printf("File not found.\n");
        return 0;
    } else printf("\nFile: \"%s\"\n", argv[1]);

    int node_amount = skip_to(file, file2, file3); // odczytywanie pliku, zwraca ilosc wierzcholkow w grafie, przechodzi do potrzebnego miejsca (linijka 4,5)
    printf("\nNode amount: %d\n\n", node_amount);
    if(parts_amount>2 && node_amount<parts_amount) {parts_amount=2;}

    node* graph = malloc(node_amount * sizeof(node)); // tworzy graf - tablicę wierzchołków
    node* graph_for_print = malloc(node_amount * sizeof(node)); // kopia graph potrzebna do wypisywania go do pliku
    create_graph(graph, file, file2, node_amount); // tworzy graf, korzystając z plików 
    printf("Graph:\n\n");
    print_graph(graph, node_amount); // wypisuje graf

    graph_for_print = copy_graph(graph, graph_for_print, node_amount); // kopiuje graf na potrzeby wypisywania

    int *graph_parts[parts_amount];
    int* part_node_nr = malloc(parts_amount * sizeof(int)); // ilość wierzchołków w grupie

    divide(graph, parts_amount, graph_parts, part_node_nr, node_amount); // dzieli graf

    printf("Number of parts: %d\nMarigin: %d\n\n", parts_amount, margin);

    sort_parts(parts_amount, part_node_nr, graph_parts); // sortuje części grafu (podzielonego)

    int i, j;
    for(i = 0; i < parts_amount; i++){ // przechodzi po wszystkich pratycjach i je wypisuje
        printf("Group %d: ", i);
        for(j = 0; j < part_node_nr[i]; j++){
            printf("%d, ", graph_parts[i][j]);
        }printf("\n");
    }printf("\n");

    delete_edges(graph_for_print, parts_amount, part_node_nr, graph_parts); // usuwa niepotrzebne krawędzie między wierzchołkami

    //zapisywanie wyniku do pliku
    write_to_file(file3, node_amount, parts_amount, part_node_nr, graph_for_print, graph_parts);

    fclose(file);
    fclose(file2);
    fclose(file3);
}