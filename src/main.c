/* 
 * File:   main.c
 * Author: Dan
 *
 * Created on 23 November 2011, 20:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "navigation.h"
char log_file[30];

/**
 * main function prompts user for input, reads file and runs simulate function.
 */
int main(int argc, char** argv) {
    date_time time_stamp;
    char path[100], ais_id[21];
    double duration, lat, lng, course, speed;
    int mins_duration, step;
    FILE *file;
    node *list_head = NULL;
/*get information from user*/
    printf("Please enter the path of the file of ship data: ");
    scanf(" %s", path);
    printf("Please enter the size of each time step in mins: ");
    scanf("%d", &step);
    printf("Please enter the duration of the simulation in hours(1.5 = 1h 30m): ");
    scanf("%lf", &duration);
    mins_duration = duration * 60; //multiply by 60 to get duration in minutes.
    /*attempt to read file, and simulate if read succeeds*/
    file = fopen(path, "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d %d %d %d", 
                &time_stamp.day, &time_stamp.month, &time_stamp.year,
                &time_stamp.hours, &time_stamp.minutes, &time_stamp.seconds);
        sprintf(log_file, "./LOG_%d_%d_%d_%d_%d_%d", 
                time_stamp.day, time_stamp.month, time_stamp.year,
                time_stamp.hours, time_stamp.minutes, time_stamp.seconds);
        while ((fscanf(file, " %s %lf %lf %lf %lf",
                ais_id, &lat, &lng, &course, &speed)) != EOF) {
            node *new_node = make_node(ais_id, lat, lng, course, speed);
            new_node->in_area = check_location(new_node->loc, new_node->course);
            add_node(&list_head, new_node);
        }
        fclose(file);
        simulate(&list_head, step, mins_duration, &time_stamp);
        print_all(&list_head);
    } else {
        printf("\nfile not found");
    }
    return (EXIT_SUCCESS);
}

void simulate(node **head, int step, int duration, date_time *time_stamp) {
    FILE *log = fopen(log_file, "w");
    fclose(log);
    int elapsed = 0;
    while (elapsed <= duration) {
        update_all(head, step, *time_stamp);
        check_all(head, *time_stamp);
        remove_node(head);
        elapsed = elapsed + step;
        update_time(step, time_stamp);
    }
}

void check_all(node **head, date_time time_stamp) {
    node *current = *head;
    while (NULL != current) {// compare all ships to other ships
        node *current2 = *head;
        while (NULL != current2) {
            if (strcmp(current->ais_id, current2->ais_id) != 0) {
                check_distance(current, current2, time_stamp);
            }
            current2 = current2->next;
        }
        current = current->next;
    }
}

void update_all(node **head, int step, date_time time_stamp) {
    node *current = *head;
    for (current = *head; current != NULL; current = current->next) {

        current->loc = update_location(current->loc, current->speed,
                current->course, step);

        int new_area = check_location(current->loc, current->course);
        FILE *log = fopen(log_file, "a");
        if (current->in_area == 0 && new_area != 0) {
            printf("\nShip %s has left shipping area", current->ais_id);

            fprintf(log, "\nShip left: %s\nTime: %d %d %d %d:%d:%d",
                    current->ais_id,
                    time_stamp.day, time_stamp.month, time_stamp.year,
                    time_stamp.hours, time_stamp.minutes, time_stamp.seconds);

        } else if (current->in_area != 0 && new_area == 0) {
            printf("\nShip %s has entered shipping area", current->ais_id);

            fprintf(log, "Ship Entered: %s\nTime: %d %d %d %d:%d:%d",
                    current->ais_id,
                    time_stamp.day, time_stamp.month, time_stamp.year,
                    time_stamp.hours, time_stamp.minutes, time_stamp.seconds);
        }
        fclose(log);
        current->in_area = new_area;
    }
}

void print_all(node **head) {
    node *current = *head;
    for (current = *head; current != NULL; current = current->next) {
        display_node(current);
    }
}

void update_time(int step, date_time *time_stamp) {
    time_stamp->minutes = time_stamp->minutes + step;
    time_stamp->hours = time_stamp->minutes / 60 + time_stamp->hours;
    time_stamp->minutes = time_stamp->minutes % 60;
    time_stamp->day = time_stamp->hours / 24 + time_stamp->day;
    time_stamp->hours = time_stamp->hours % 24;
    switch (time_stamp->month) {
        case 2:
            if ((0 == time_stamp->year % 4 && 0 != time_stamp->year % 100)
                    || 0 == time_stamp->year % 400) {
                time_stamp->month = time_stamp->month + time_stamp->day / 29;
                time_stamp->day = time_stamp->day % 29 + 1;
            } else {
                time_stamp->month = time_stamp->month + time_stamp->day / 28;
                time_stamp->day = time_stamp->day % 28 + 1;
            }
            break;
        case 1:case 3: case 5:case 7:case 8:case 10:
            time_stamp->month =time_stamp->month + time_stamp->day / 31;
            time_stamp->day = time_stamp->day % 31 + 1;
            break;
        case 4:case 6:case 9:case 11:
            time_stamp->month = time_stamp->month + time_stamp->day / 30;
            time_stamp->day = time_stamp->day % 30 + 1;
            break;
        case 12:
        {
            if (time_stamp->day > 31) {
                time_stamp->year++;
                time_stamp->month = 1;
                time_stamp->day = time_stamp->day % 31 + 1;
            }
            break;
        }
    }
}
