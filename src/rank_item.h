#ifndef RANK_ITEM_H
#define RANK_ITEM_H

struct Item {
    int score;
    char name[100];
};


int load_items(FILE* f, struct Item** items, int* nbItems);
int sort_items(struct Item* items, struct Item* sortedItems, int nbItems);
void print_items(struct Item* items, int nbItems);
#endif
