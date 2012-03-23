#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "navigation.h"
#include "main.h"

void display_node(node *node);

node *make_node(char *ais_id, double lat, double lng, double course, double speed) {
    node *new_ship;
    new_ship = malloc(sizeof (node));
    strcpy(new_ship->ais_id, ais_id);
    new_ship->loc.lat = lat;
    new_ship->loc.lng = lng;
    new_ship->course = course;
    new_ship->speed = speed;
    new_ship->sunk = 0;
    return new_ship;
}

void add_node(node **head, node *new) {

    if (NULL == *head) {
        *head = new;
    } else {
        new->next = *head;
        *head = new;
    }
}

void display_list(node **head, int listsize) {
    int i;
    node *current = *head;
    for (i = 0; i < listsize; i++) {
        display_node(current);
        current = current->next;
    }
}

void display_node(node *node) {
    printf("\nAIS ID:%s\nLatitude:%lf\nLongditude:%lf\n",
            node->ais_id,
            node->loc.lat,
            node->loc.lng);
}

void remove_node(node **head) {
    node *curr, *prev = NULL;
    for (curr = *head; curr != NULL; prev = curr, curr = curr->next) {
        if (curr->in_area == 1 || curr->sunk) {
            if (prev == NULL) {
                (*head) = (*head)->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
        }
    }
}

