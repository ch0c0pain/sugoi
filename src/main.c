#include <stdio.h>
#include <stdlib.h>

#include "rank_item.h"

void help(char* cmd) {
    printf("Usage: %s <file>\n", cmd);
}

int main(int argc, char** argv) {
    struct Item* items = NULL;
    struct Item* sortedItems = NULL;
    int nbItems = 0;
    FILE* inputFile;

    if (argc < 2) {
        help(argv[0]);
        return 1;
    }

    if (!(inputFile = fopen(argv[1], "r"))) {
        fprintf(stderr, "Error: could not open file\n");
        return 1;
    }

    if (!(load_items(inputFile, &items, &nbItems))) {
        fprintf(stderr, "Error: could not load items\n");
        goto failure;
    }

    if (!(sortedItems = malloc(nbItems*sizeof(struct Item)))) {
        fprintf(stderr, "Error: could not allocate memory for sorted array\n");
        goto failure;
    }

    printf("Correctly loaded items\n");

    sort_items(items, sortedItems, nbItems);
    print_items(sortedItems, nbItems);
    
    free(sortedItems);
    free(items);
    fclose(inputFile);
    return 0;

failure:
    free(items);
    free(sortedItems);
    fclose(inputFile);
    return 1;
}
