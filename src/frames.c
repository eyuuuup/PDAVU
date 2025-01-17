#include <ijvm.h>
#include <stdlib.h>
#include <frames.h>
#include <util.h>

struct frame
{
    int id;
    int data;
    struct frame *next;
};

static struct frame *head;

void add_frame(int id, int data)
{

    if (head == NULL)
    {
        struct frame *frame = (struct frame *)malloc(sizeof(struct frame) + 1);
        frame->data = data;
        frame->id = id;
        frame->next = NULL;
        head = frame;
    }
    else
    {
        struct frame *curr = head;
        while (curr->next != NULL)
        {
            if (curr->id == id)
            {
                curr->data = data;
                return;
            }
            else
            {
                curr = curr->next;
            }
        }
        if (id == curr->id)
        {
            curr->data = data;
        }
        else
        {
            struct frame *frame = (struct frame *)malloc(sizeof(struct frame) + 1);
            frame->data = data;
            frame->id = id;
            frame->next = NULL;
            curr->next = frame;
        }
    }
}

int find_var(int id)
{
    struct frame *curr = head;
    while (curr != NULL)
    {
        if (curr->id == id)
        {
            return curr->data;
        }
        else
        {
            curr = curr->next;
        }
    }
    return -1;
}

void print_list()
{
    dprintf("LIST:");
    struct frame *curr = head;
    while (curr != NULL)
    {
        dprintf("{%d, %d} ", curr->id, curr->data);
        curr = curr->next;
    }
    dprintf("\n");
}

void destroy_list()
{
    struct frame *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
        temp = NULL;
    }
    head = NULL;
}