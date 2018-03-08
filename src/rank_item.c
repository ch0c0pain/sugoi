#include <stdio.h>
#include <stdlib.h>

#include "rank_item.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define KBOLD "\e[1m"
#define KNBOLD "\e[22m"

static int get_nb_items(FILE* f) {
    int lines = 0;

    while (!feof(f))
        lines += (fgetc(f) == '\n');
    return lines;
}

int load_items(FILE* f, struct Item** items, int* nbItems) {
    char cur;
    int i = 0, strLen = 0;

    *nbItems = get_nb_items(f);
    if (!(*items = malloc(*nbItems * sizeof(struct Item)))) {
        fprintf(stderr, "Error: could not malloc mem for items\n");
        return 0;
    }

    rewind(f);
    while (!feof(f) && i < *nbItems) {
        cur = fgetc(f);
        if (cur == '\n') {
            (*items)[i].name[strLen] = '\0';
            i++;
            strLen = 0;
        } else {
            if (strLen < 99) {
                (*items)[i].name[strLen] = cur;
                strLen++;
            }
        }
    }
    return 1;
}

static int comp_items(const struct Item* item1, const struct Item* item2) {
    char choice;

    printf("[" KRED "f" KNRM "] "
            KBOLD "%s" KNBOLD " vs " KBOLD "%s" KNBOLD
            " [" KRED "l" KNRM "]\n", item1->name, item2->name);
    choice = getchar();
    getchar();
    return (choice == 'f');
}

static int find_item_pos(const struct Item* item,
                         const struct Item* sorted, int nbSorted) {
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

static void insert_item(struct Item item, int pos, struct Item* items, int nbItems) {
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

    for (i = nbItems-1; i >= 0; i--)
        printf("%s\n", items[i].name);
}
