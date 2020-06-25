#ifndef FRAMES_H
#define FRAMES_H

/**
 * Add a frame in the linked list.
 * Overwrites existing frame if id already exists.
 **/
void add_frame(int id, int data);

/**
 * Print every element's id and data in the list
 **/
void print_list();

/**
 * Find and return a specific element.
 **/
int find_var(int id);

/**
 * Free the list and point the head to null.
 **/
void destroy_list();

#endif