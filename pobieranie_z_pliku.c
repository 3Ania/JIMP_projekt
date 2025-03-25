#include "pobieranie_z_pliku.h"

#include <stdio.h>
#include <stdlib.h>

void download(FILE *f_line3, FILE *f_line4){
    int target_line3 = 3;
    int target_line4 = 4;
    int curr_line = 0;

    char chr = getc(f_line3);
    char chr2 = getc(f_line4);
    while(chr != EOF && curr_line < target_line3){
        if(chr == '\n') curr_line++;
        chr = getc(f_line3);
        chr2 = getc(f_line4);
    }

    chr2 = getc(f_line4);
    while(chr2 != EOF && chr2 != '\n') chr2 = getc(f_line4);
    
    chr2 = getc(f_line4);
    while(chr2 != EOF && chr2 != '\n'){
        printf("%c", chr2);
        chr2 = getc(f_line4);
    }
    printf("\n\n");
    // chr = getc(f_line3);
    while(chr != EOF && chr != '\n'){
        printf("%c", chr);
        chr = getc(f_line3);
    }

    printf("\n\n");
}