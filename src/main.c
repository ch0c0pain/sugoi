#include <stdio.h>
#include <stdlib.h>

struct Item {
    int score;
    char name[100];
};

void help(char* cmd) {
    printf("Usage: %s <file>\n", cmd);
}

int get_nb_items(FILE* f) {
    int lines = 0;

    while (!feof(f))
        lines += (fgetc(f) == '\n');
    return lines;
}

int load_items(FILE* f, struct Item* items, int nbItems) {
    char cur;
    int i = 0, strLen = 0;

    rewind(f);
    while (!feof(f) && i < nbItems) {
        cur = fgetc(f);
        if (cur == '\n') {
            items[i].name[strLen] = '\0';
            i++;
            strLen = 0;
        } else {
            if (strLen < 99) {
                items[i].name[strLen] = cur;
                strLen++;
            }
        }
    }
    return 1;
}

int comp_items(const struct Item* item1, const struct Item* item2) {
    char choice;

    printf("[f] %s vs %s [l]\n", item1->name, item2->name);
    choice = getchar();
    getchar();
    return (choice == 'f');
}

int find_item_pos(const struct Item* item, const struct Item* sorted, int nbSorted) {
    int inf = 0;
    int sup = nbSorted;

    while (inf != sup) {
        if (comp_items(item, &(sorted[(inf+sup)/2])))
            inf = (inf+sup)/2+1;
        else
            sup = (inf+sup)/2;
    }
    return inf;
}

void insert_item(struct Item item, int pos, struct Item* items, int nbItems) {
    int i = nbItems;

    for (i = nbItems; i > pos; i--)
        items[i] = items[i-1];
    items[pos] = item;
}

int sort_items(struct Item* items, struct Item* sortedItems, int nbItems) {
    int nbSortedItems = 0;

    for (nbSortedItems = 0; nbSortedItems < nbItems; nbSortedItems++) {
        int newPos = find_item_pos(&(items[nbSortedItems]), sortedItems, nbSortedItems);
        insert_item(items[nbSortedItems], newPos, sortedItems, nbSortedItems);
    }

    return 1;
}

void print_items(struct Item* items, int nbItems) {
    int i;

    for (i = 0; i < nbItems; i++)
        printf("%s\n", items[i].name);
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
    nbItems = get_nb_items(inputFile);

    if (!(items = malloc(nbItems * sizeof(struct Item)))
            || !(sortedItems = malloc(nbItems * sizeof(struct Item)))) {
        fprintf(stderr, "Error: could not malloc mem for items\n");
        goto failure;
    }

    if (!(load_items(inputFile, items, nbItems))) {
        fprintf(stderr, "Error: could not load items\n");
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
