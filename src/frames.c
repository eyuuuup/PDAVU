#include <ijvm.h>
#include <stdlib.h>
#include <frames.h>

struct frame {
    int id;
    int data;
    struct frame* next;
};

static struct frame *head;

void add_frame(int id, int data) {
    struct frame* frame = (struct frame*)malloc(sizeof(struct frame));
    frame->data = data;
    frame->id = id;
    frame->next = NULL;

    if(head == NULL) {
        head = frame;
    } else {
        struct frame *curr = head;
        while (curr->next != NULL) {
            if(curr->id == id) {
                curr->data = data;
                free(frame);
                return;
            } else {
                curr = curr->next;
            }
            
        }

        if(id == curr->id) {
            curr->data = data;
            free(frame);
        } else {
            curr->next = frame;
        }
        
    }
}

void remove_frame() {
    head = head->next;
}

int find_var(int id) { 
    struct frame *curr = head;
    while (curr != NULL) {
        if(curr->id == id) {
            return curr->data;
        } else {
            curr = curr->next;
        }
    }
    return -1;
} 

void print_list() { 
    printf("LIST:");
    struct frame *curr = head;
    while (curr != NULL) { 
        printf("%d ", curr->data); 
        curr = curr->next; 
    } 
    printf("\n");
} 
  
void destroy_list() {
    struct frame *curr = head;
    while (curr != NULL) { 
        free(curr);
        curr = curr->next; 
    }
    head = NULL;
}