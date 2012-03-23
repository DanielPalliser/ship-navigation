/* 
 * File:   main.h
 * Author: Dan
 *
 * Created on 23 November 2011, 20:30
 */
#include "navigation.h"
#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
#define LOGFILE "./log.txt"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
    typedef struct time_struct {
        int day;
        int month;
        int year;
        int hours;
        int minutes;
        int seconds;
    } date_time;

    typedef struct node_struct {
        char ais_id[15];
        location loc;
        double course;
        double speed;
        int sunk;
        int in_area;
        struct node_struct *next;
    } node;
    /**
     * 
     * @param head
     */
    void display_node(node *node);
    void print_all(node **head);
    /**
     * 
     * @param head
     * @param step
     * @param time_stamp
     */
    void update_all(node **head, int step, date_time time_stamp);
    /**
     * creates a node with the given data
     * @param ais_id ais id of the ship
     * @param lat  starting latitude of the ship
     * @param lng starting longditude of the ship
     * @param course starting course of the ship
     * @param speed starting speed of the ship
     * @return the created node
     */
    node *make_node(char *ais_id, double lat, double lng, double course,
            double speed);
    /**
     * adds a node to the linked list
     * @param head head of the linked list
     * @param new node to be added
     */
    void add_node(node **head, node * new);
    /**
     * gets two ships from the linked list for comparison
     * @param head head of the linked list
     * @param time_stamp simulated time
     */
    void check_all(node **head, date_time time_stamp);
    /**
     * updates the location, using the paramaters given
     * @param loc the location(latitude and longditude)
     * @param speed speed of movement in knots
     * @param course direction of travel in degrees
     * @param step time to calculate distance travelled in
     * @return 0 = in area 1 = sailing away from area 2 = may be sailing towards area
     */
    location update_location(location loc, double speed, double course,
            int step);
    /**
     * takes in two ship nodes and tests if they are within 1/4 or within 2 
     * nautical miles
     * @param ship_1 first ship
     * @param ship_2 second ship
     * @param time_stamp current time in simulation. used for log file
     */
    void check_distance(node *ship_1, node *ship_2, date_time time_stamp /* = 0 */);
    /**
     * runs the simulation. updates time and location of ships and checks if they
     * are at risk of crashing or have crashed.
     * @param head
     * @param step
     * @param duration
     * @param time_stamp
     */
    void simulate(node **head, int step, int duration, date_time *time_stamp);
   
    /**
     * removes a node from the linked list of ships
     * @param head a pointer to the pointer to the head of the linked list
     */
    void remove_node(node **head);
    /**
     * updates the current simulation time, by the amount of minutes in duration
     * @param duration amount of time to update by
     * @param time_stamp current time
     */
    void update_time(int duration, date_time *time_stamp);
    /*
     * checks if ship is inside shipping area, and if it is leaving or entering
     * returns an int; 0 if in area, 1 if leaving, 2 if entering
     * @param loc current location of ship to be checked
     * @param course current course of ship to be checked
     * @return 
     */
    int check_location(location loc, double course);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

