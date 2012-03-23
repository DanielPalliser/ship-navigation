#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "navigation.h"
extern char log_file[30];

location update_location(location loc, double speed, double course, int step) {
    /*degrees to radians for lat and long*/
    double d_step = step;
    double lstr, cgr, final_lat, final_lng;
    lstr = loc.lat * (M_PI / 180);
    cgr = course * (M_PI / 180);
    /*calculate new lat**/
    final_lat = (loc.lat + (speed * (cos(cgr)) * d_step) / 3600);
    /*calculate new long*/
    final_lng = (loc.lng + (speed * (sin(cgr)) * d_step * (cos(lstr))) / 3600);
    location final_loc = {final_lat, final_lng};
    return final_loc;
}

void check_distance(node *ship_1, node *ship_2, date_time time_stamp) {
    double distance = great_circle(ship_1->loc, ship_2->loc);
    double warn = 2, crash = 0.25;
    if (distance < crash) {
        FILE *log = fopen(log_file, "a");
        fprintf(log, "Collision detected between ships %s and %s",
                ship_1->ais_id,
                ship_2->ais_id);
        printf("Collision detected between ships %s and %s",
                ship_1->ais_id,
                ship_2->ais_id);

        ship_1->sunk = 1;
        ship_2->sunk = 1;
        fclose(log);
    } else if (distance < warn) {
        FILE *log = fopen(log_file, "a");
        printf("\nCollision likely between ships %s and %s\nTime: %d %d %d %d:%d:%d",
                ship_1->ais_id,
                ship_2->ais_id, 
                time_stamp.day, time_stamp.month,time_stamp.year,
                time_stamp.hours, time_stamp.minutes,time_stamp.seconds);
        fprintf(log, "\nPossibility of Collision. ships: %s and %s\nTime: %d %d %d %d:%d:%d",
                ship_1->ais_id,
                ship_2->ais_id, time_stamp.day, time_stamp.month,
                    time_stamp.year, time_stamp.hours, time_stamp.minutes,
                    time_stamp.seconds);
        fclose(log);
    }
}


int check_location(location loc, double course) {
    int ret;
    if (51.667 < loc.lat && loc.lat < 52.883 &&
            -6.667 < loc.lng && loc.lng < -3.833) {
        ret = 0;
    } else if (loc.lat > 52.833 && (90.000 > course || course > 270.000)) {
        ret = 1;
    } else if (loc.lat < 51.667 && (90.000 < course && course < 270.000)) {
        ret = 1;
    } else if (loc.lng < -6.667 && course > 180.000) {
        ret = 1;
    } else if (loc.lng > -3.833 && course < 180.000) {
        ret = 1;
    } else {
        ret = 2;
    }
    return ret;
}
