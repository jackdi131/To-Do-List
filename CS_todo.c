// z5444507
// Jack Di
// Assignment 2 COMP1511
// This code creates a linked list to create a todo list where users can input
// what they want

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INVALID_PRIORITY -1

#define MAX_TASK_LENGTH 200
#define MAX_CATEGORY_LENGTH 40
#define MAX_STRING_LENGTH 1024

// You *should* #define each command
#define COMMAND_ADD_TASK 'a'
#define COMMAND_PRINT_TODO 'p'
#define COMMAND_UPDATE_PRIORITY 'i'
#define COMMAND_COUNT_TASKS 'n'
#define COMMAND_COMPLETE_TASKS 'c'
#define COMMAND_PRINT_COMPLETED 'P'
#define COMMAND_ESTIMATION 'e'
#define COMMAND_DELETE 'd'
#define COMMAND_FREE_LIST 'f'
#define COMMAND_REPEAT_TASK 'r'
#define NULL_NEXT 0
#define LAST_LIST 1
#define MID_LIST 2


enum priority { LOW, MEDIUM, HIGH };

struct task {
    char task_name[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    enum priority priority;

    struct task *next;
};

struct completed_task {
    // might need to change to 'next_task' for better clarification
    struct task *task;
    int start_time;
    int finish_time;
    struct completed_task *next;
};

struct todo_list {
    struct task *tasks;
    struct completed_task *completed_tasks;
};

////////////////////////////////////////////////////////////////////////////////
///////////////////// YOUR FUNCTION PROTOTYPES GO HERE /////////////////////////
////////////////////////////////////////////////////////////////////////////////


void add_task(struct todo_list *todo);
void command_loop(struct todo_list *todo);
struct todo_list *create_todo_list();

struct task *create_task(
    char name[MAX_TASK_LENGTH],
    char category[MAX_CATEGORY_LENGTH],
    enum priority priority
);

void add_task_end (
    struct todo_list *todo,
    struct task *new_task
);

void view_tasks(struct todo_list *todo);
void update_priority(struct todo_list *todo);

struct task *find_task(
    struct todo_list *todo,
    char task[MAX_TASK_LENGTH],
    char category[MAX_CATEGORY_LENGTH]
);

void count_tasks(struct todo_list *todo);

void unlink(struct task *mark_off, struct todo_list *todo);

struct completed_task *create_completed_task( 
    struct task *mark_off,
    int start_time,
    int finish_time
);    


void complete_task(struct todo_list *todo);

void add_completed_head(
    struct todo_list *todo, 
    struct completed_task *new_completed_task
);

void view_completed_tasks(struct todo_list *todo);

int count_return(struct todo_list *todo);

void estimate_time(struct todo_list *todo);

void delete_task(struct todo_list *todo);

void free_list(struct todo_list *todo);

////////////////////////////////////////////////////////////////////////////////
//////////////////////// PROVIDED HELPER PROTOTYPES ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], enum priority *prio
);
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]
);
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], int *start_time, int *finish_time
);

enum priority string_to_priority(char priority[MAX_STRING_LENGTH]);
void remove_newline(char input[MAX_STRING_LENGTH]);
void trim_whitespace(char input[MAX_STRING_LENGTH]);
void print_one_task(int task_num, struct task *task);
void print_completed_task(struct completed_task *completed_task);

int task_compare(struct task *t1, struct task *t2);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    // Stage 1.1
    // You should initialize the `todo` variable below. You will need
    // to use the malloc() function to allocate memory for it!

    printf("Welcome to CS ToDo!\n");
    struct todo_list *todo = create_todo_list();
    
    command_loop(todo);
    
    return 0;
}

/**
 * The central loop that executes commands until the program is completed.
 *
 * Parameters:
 *     todo - The todo list to execute commands on.
 *
 * Returns:
 *     Nothing
 */

// Creates the head of the linked list that points to the completed tasks and
// need to be completed tasks
// Input: void
// Output: struct todo (which is a struct containing pointers to task and
// completed)
struct todo_list *create_todo_list() {
    struct todo_list *todo = malloc(sizeof(struct todo_list));

    todo->tasks = NULL;
    todo->completed_tasks = NULL;

    return todo;
}

void command_loop(struct todo_list *todo) {
    printf("Enter Command: ");
    char command;

    while (scanf(" %c", &command) == 1) {
        // TODO: Add to this loop as you complete the assignment

        if (command == COMMAND_ADD_TASK) {
            // TODO: Add a task to the todo list here
            add_task(todo);       
        }

        else if (command == COMMAND_PRINT_TODO) {
            view_tasks(todo);
        }

        else if (command == COMMAND_UPDATE_PRIORITY) {
            update_priority(todo);
        }

        else if (command == COMMAND_COUNT_TASKS) {
            count_tasks(todo);
        }

        else if (command == COMMAND_COMPLETE_TASKS) {
            complete_task(todo);
        }

        else if (command == COMMAND_PRINT_COMPLETED) {
            view_completed_tasks(todo);
        }

        else if (command == COMMAND_ESTIMATION) {
            estimate_time(todo);
        }

        else if (command == COMMAND_DELETE) {
            delete_task(todo);
        }

        else if (command == COMMAND_FREE_LIST) {
            free_list(todo);
        }

        else if (command == COMMAND_REPEAT_TASK) {
            
        }
        printf("Enter Command: ");
    }

    printf("All done!\n");
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////// YOUR HELPER FUNCTIONS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// This function takes input head of list and frees up all the memory allocated
// to the completed tasks 
// output nothing
void free_list(struct todo_list *todo) {
    struct completed_task *current_completed = todo->completed_tasks;
    struct completed_task *next = NULL;

    if (current_completed == NULL) {
        return;
    }

    if (current_completed->next == NULL) {
        free(current_completed->task);
        free(current_completed);
        todo->completed_tasks = NULL;
        return;
    }

    // struct completed_task *saved_next = current_completed->next;

    while (current_completed != NULL) {
        next = current_completed->next;
        free(current_completed);
        current_completed = next;
        todo->completed_tasks = NULL;
    }
}

// This function takes the input header as an argument, finds the task
// unlinks it from the linked list and frees the memory
// output nothing
void delete_task(struct todo_list *todo) {
    
    struct task *current_task = todo->tasks;
    struct task *previous = NULL;

    // Fetch `[task] [category]` from stdin
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);

    // Create strings for `task`/`category` and populate them using the contents
    // of `buffer`
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    parse_task_category_line(buffer, task, category);

    /* if task list is empty
    if (current_task == NULL) {
        return;
    }*/

    struct task *found_task = find_task(todo, task, category);

    if (found_task == NULL) {

        // printf("%d", current_task->priority);
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return;
    }

    if (found_task == current_task) {
        todo->tasks = current_task->next;
        free(current_task);
        return;
    }

    while (current_task != NULL) {
        if (current_task->next == found_task) {
            previous = current_task;
            current_task = current_task->next;
            previous->next = current_task->next;
            free(current_task);
            return;
        }

        current_task = current_task->next;
    }

}

// This function takes in input header as argument and estimates the time 
// to complete a task of specific category. If the task category has not been
// compelted yet then assume it takes 100 minutes 
void estimate_time(struct todo_list *todo) {
    struct completed_task *current_completed_task = todo->completed_tasks;
    struct task *current_task = todo->tasks;

    if (current_completed_task == NULL) {

        int count_case_empty = 0;

        printf("Expected completion time for remaining tasks:\n\n");

        while (current_task != NULL) {
            count_case_empty++;
            print_one_task(count_case_empty, current_task);
            printf("Expected completion time: 100 minutes\n\n");
            current_task = current_task->next;
        }
        return;
    }

    int counter = 0;


    printf("Expected completion time for remaining tasks:\n\n");

    while (current_task != NULL) {
        double count_same_cat = 0;
        double sum_same_cat = 0;
        double average;

        while (current_completed_task != NULL) {
            struct task *task_of_completed = current_completed_task->task;

            if (strcmp(current_task->category, task_of_completed->category)
                == 0) {
                count_same_cat++;
                sum_same_cat = sum_same_cat + 
                current_completed_task->finish_time
                - current_completed_task->start_time;
            } else {

            }

            current_completed_task = current_completed_task->next;
        }

        if (count_same_cat > 0) {
            average = floor(sum_same_cat/count_same_cat);
            counter++;

            print_one_task(counter, current_task);
            printf("Expected completion time: %.0lf minutes\n\n", average);
            current_task = current_task->next;
            current_completed_task = todo->completed_tasks;

        } else {
            counter++;
            print_one_task(counter, current_task);
            printf("Expected completion time: 100 minutes\n\n");
            current_task = current_task->next;
            current_completed_task = todo->completed_tasks;
        }
    }
}

// This function takes an input argument as the header of the linked list and 
// moves the input task into the completed task list
// output nothing

void complete_task(struct todo_list *todo) {

    // Fetch `[task] [category] [start_time] [finish_time]` from stdin
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);

    // Create strings for `task`/`category` and ints for times, then populate
    // them using the contents of `buffer`.
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    int start_time;
    int finish_time;
    parse_complete_task_line(buffer, task, category, &start_time, &finish_time);

    // Find the task that is being marked off
    struct task *mark_off = find_task(todo, task, category);

    if (mark_off == NULL) {

        // printf("%d", current_task->priority);
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return;
    } 

    // Case when it is head
    if (todo->tasks == mark_off) {
        struct task *after_mark = mark_off->next;
        unlink(mark_off, todo);
        struct completed_task *new_completed_task = 
            create_completed_task(mark_off, start_time, finish_time);
        
        add_completed_head(todo, new_completed_task);
        todo->tasks = after_mark;
        return;
    }


    // Unlink the node that is being marked off
    unlink(mark_off, todo);

    // printf("the completed task is: %p", completed_task);

    // Create a new struct of type completed_task
    struct completed_task *new_completed_task = 
        create_completed_task(mark_off, start_time, finish_time);

    // Add to head of complete list 
    add_completed_head(todo, new_completed_task);

}

// This function adds the completed task to the head of the list 
void add_completed_head(
    struct todo_list *todo, 
    struct completed_task *new_completed_task
) {
    struct completed_task *previous = NULL;
    struct completed_task *current_task = todo->completed_tasks;

    if (current_task == NULL) {
        todo->completed_tasks = new_completed_task;
        return;
    }


    previous = current_task;
    todo->completed_tasks = new_completed_task;
    new_completed_task->next = previous;

}


// This function takes in a pointer to the head of the task list as well as 
// a pointer to a specific node that needs to be marked off
// The function unlinks the specific node from the list 
// output nothing
void unlink(struct task *mark_off, struct todo_list *todo) {
    struct task *current_task = todo->tasks;

    if (current_task->next == NULL && current_task == mark_off) {
        todo->tasks = NULL;
        return;
    }

    while (current_task != NULL) {

        if (current_task->next == mark_off) {

            if (mark_off->next == NULL) {

                current_task->next = NULL;
                // current_task = current_task->next;
                return;
            }

            current_task->next = mark_off->next;
            mark_off->next = NULL;
            return;
        }

        current_task = current_task->next;
    }

    return;
    // the next task is at the required mark off pointer
    // current_task->next = mark_off->next;
    // mark_off->next = NULL;
    
}

// This function takes in a pointer to the head of the function and counts how 
// many tasks are in it then prints
// output nothing
void count_tasks(struct todo_list *todo) {
    struct task *current_task = todo->tasks;

    if (current_task == NULL) {
        printf("There are 0 items on your list!\n");
        return;
    }

    int count = 0;

    while (current_task != NULL) {
        count++;
        current_task = current_task->next;
    }

    printf("There are %d items on your list!\n", count);
}

// This function takes in a pointer to the head of the function and counts how 
// many tasks are in it then prints
// output that value
int count_return(struct todo_list *todo) {
    struct task *current_task = todo->tasks;

    if (current_task == NULL) {
        printf("There are 0 items on your list!\n");
        return 0;
    }

    int count = 0;

    while (current_task != NULL) {
        count++;
        current_task = current_task->next;
    }

    return count;
}

// This function updates the priority to one higher
void update_priority(struct todo_list *todo) {
    // Fetch `[task] [category]` from stdin
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);

    // Create strings for `task`/`category` and populate them using the contents
    // of `buffer`
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    parse_task_category_line(buffer, task, category);

    struct task *found_task = find_task(todo, task, category);

    if (found_task == NULL) {

        // printf("%d", current_task->priority);
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return;
    } 

    if (found_task->priority == LOW) {
        found_task->priority = MEDIUM;
        return;

    } else if (found_task->priority == MEDIUM) {
        found_task->priority = HIGH;
        return;

    } else if (found_task->priority == HIGH) {
        found_task->priority = LOW;
        return;
    }
    

}

// This function takes in the head of the linked list, the task and category
// name and cycles through the linked list and finds a match
// output a header to the linked list that is a match
// if no match is found it will print message 
struct task *find_task(
    struct todo_list *todo,
    char task[MAX_TASK_LENGTH],
    char category[MAX_CATEGORY_LENGTH]
) {

    struct task *current_task = todo->tasks;

    while (current_task != NULL) {
        
        if (strcmp(current_task->task_name, task) == 0 && 
            strcmp(current_task->category, category) == 0) {

            return current_task;
        }
        
        current_task = current_task->next;
    }


    return NULL;
}

// This function prints out the tasks
void view_tasks(struct todo_list *todo) {
    struct task *current_task = todo->tasks;
    int counter = 0;
    
    if (current_task == NULL) {
        printf("==== Your ToDo List ====\n");
        printf("All tasks completed, you smashed it!\n");
        printf("====   That's it!   ====\n");
        return;

    } else {
        printf("==== Your ToDo List ====\n");

        while (current_task != NULL) {
            counter++;
            print_one_task(counter, current_task);
            current_task = current_task->next;
        }

        printf("====   That's it!   ====\n");
    }

}

// This function prints out the completed tasks
void view_completed_tasks(struct todo_list *todo) {
    struct completed_task *current_task = todo->completed_tasks;
    // int counter = 0;
    
    if (current_task == NULL) {
        printf("==== Completed Tasks ====\n");
        printf("No tasks have been completed today!\n");
        printf("=========================\n");
        return;

    } else {
        printf("==== Completed Tasks ====\n");

        while (current_task != NULL) {
            // counter++;
            print_completed_task(current_task);
            current_task = current_task->next;
        }

        printf("=========================\n");
    }

}

// Takes in a struct 'todo_list' and scans in a new task, adding it to the list
void add_task(struct todo_list *todo) {

    // Create a string to scan the entire command input into.
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);

    // Create variables for each part of the command being scanned in
    // (name of task, category of task and priority of task)
    char task_name[MAX_TASK_LENGTH];
    char task_category[MAX_CATEGORY_LENGTH];
    enum priority task_priority;
    parse_add_task_line(buffer, task_name, task_category, &task_priority);

    // We need to create a node to store the data gathered above before adding
    struct task *new_task = create_task(task_name, task_category, task_priority);

    // Now that a node is created and the input values have been added we 
    // need to add the node at the end of the linked list
    add_task_end(todo, new_task);
}

// Function takes input pointer to a task and adds it to the end of the list
void add_task_end (
    struct todo_list *todo,
    struct task *new_task
) {
    struct task *current_task = todo->tasks;

    // If this is the first time a player is being added
    if (current_task == NULL) {
        todo->tasks= new_task;
        return;
    }

    // If it not the first time a player is being added we need to cycle through
    // the linked list and add it onto the end
    while (current_task->next != NULL) {
        current_task = current_task->next;
    }

    current_task->next = new_task;
}

// Function creates a struct node of type task
struct task *create_task(
    char name[MAX_TASK_LENGTH],
    char category[MAX_CATEGORY_LENGTH],
    enum priority priority
) {

    struct task *new_task = malloc(sizeof(struct task));
    strcpy(new_task->task_name, name);
    strcpy(new_task->category, category);
    new_task->priority = priority;
    new_task->next = NULL;

    return new_task;
}
// Function creates a struct node of type completed task
struct completed_task *create_completed_task( 
    struct task *mark_off,
    int start_time,
    int finish_time     
) {
    struct completed_task *new_completed = 
        malloc(sizeof(struct completed_task));
    
    new_completed->start_time = start_time;
    new_completed->finish_time = finish_time;
    new_completed->next = NULL;  
    new_completed->task = mark_off;

    return new_completed;
    
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////// PROVIDED HELPER FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * Given a raw string in the format: [string1] [string2] [enum priority]
 * This function will extract the relevant values into the given variables.
 * The function will also remove any newline characters.
 *
 * For example, if given: "finish_assignment_2 assignment2 high"
 * The function will copy the string:
 *     "finish_assignment_2" into the `task_name` array
 * Then copy the string:
 *     "assignment2" into the `task_category` array
 * And finally, copy over the enum:
 *     "high" into the memory that `prio` is pointing at.
 *
 * Parameters:
 *     buffer        - A null terminated string in the following format
 *                     [string1] [string2] [enum priority]
 *     task_name     - A character array for the [string1] to be copied into
 *     task_category - A character array for the [string2] to be copied into
 *     prio          - A pointer to where [enum priority] should be stored
 *
 * Returns:
 *     None
 */
void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    enum priority *prio
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }
    
    // Extract value 3 as string
    char *prio_str = strtok(NULL, " ");
    if (prio_str != NULL) {
        *prio = string_to_priority(prio_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        prio_str == NULL ||
        *prio == INVALID_PRIORITY
    ) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority`.
 */
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }

    if (name_str == NULL || category_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority` and addition of start/end times
 */
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    int *start_time,
    int *finish_time
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }
    
    // Extract value 2 as string
    char *start_str = strtok(NULL, " ");
    if (start_str != NULL) {
        *start_time = atoi(start_str);
    }
    
    // Extract value 2 as string
    char *finish_str = strtok(NULL, " ");
    if (finish_str != NULL) {
        *finish_time = atoi(finish_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        start_str == NULL ||
        finish_str == NULL
    ) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string, will return the corresponding `enum priority`,
 * or INVALID_PRIORITY if the string doesn't correspond with the enums.
 *
 * Parameters:
 *     priority - string representing the corresponding `enum priority` value
 * Returns:
 *     enum priority
 */
enum priority string_to_priority(char priority[MAX_STRING_LENGTH]) {
    if (strcmp(priority, "low") == 0) {
        return LOW;
    } else if (strcmp(priority, "medium") == 0) {
        return MEDIUM;
    } else if (strcmp(priority, "high") == 0) {
        return HIGH;
    }

    return INVALID_PRIORITY;
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given an priority and a character array, fills the array with the
 * corresponding string version of the priority.
 *
 * Parameters:
 *     prio - the `enum priority` to convert from
 *     out  - the array to populate with the string version of `prio`.
 * Returns:
 *     Nothing
 */
void priority_to_string(enum priority prio, char out[MAX_STRING_LENGTH]) {
    if (prio == LOW) {
        strcpy(out, "LOW");
    } else if (prio == MEDIUM) {
        strcpy(out, "MEDIUM");
    } else if (prio == HIGH) {
        strcpy(out, "HIGH");
    } else {
        strcpy(out, "Provided priority was invalid");
    }
}

/*
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string will remove and first newline it sees.
 * The newline character wil be replaced with a null terminator ('\0')
 *
 * Parameters:
 *     input - The string to remove the newline from
 *
 * Returns:
 *     Nothing
 */
void remove_newline(char input[MAX_STRING_LENGTH]) {
    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}

/*
 * Helper Function
 * You likely do not need to change this function.
 *
 * Given a raw string, will remove any whitespace that appears at the start or
 * end of said string.
 *
 * Parameters:
 *     input - The string to trim whitespace from
 *
 * Returns:
 *     Nothing
 */
void trim_whitespace(char input[MAX_STRING_LENGTH]) {
    remove_newline(input);
    
    int lower;
    for (lower = 0; input[lower] == ' '; ++lower);
    
    int upper;
    for (upper = strlen(input) - 1; input[upper] == ' '; --upper);
    
    for (int base = lower; base <= upper; ++base) {
        input[base - lower] = input[base];
    }

    input[upper - lower + 1] = '\0';
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a task, prints it out in the format specified in the assignment.
 *
 * Parameters:
 *     task_num - The position of the task within a todo list
 *     task     - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_one_task(int task_num, struct task *task) {
    char prio_str[MAX_STRING_LENGTH];
    priority_to_string(task->priority, prio_str);

    printf(
        "  %02d. %-30.30s [ %s ] %s\n",
        task_num, task->task_name, task->category, prio_str
    );

    int i = 30;
    while (i < strlen(task->task_name)) {
        printf("      %.30s\n", task->task_name + i);
        i += 30;
    }
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a completed task, prints it out in the format specified in the
 * assignment.
 *
 * Parameters:
 *     completed_task - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_completed_task(struct completed_task *completed_task) {
    int start_hour = completed_task->start_time / 60;
    int start_minute = completed_task->start_time % 60;
    int finish_hour = completed_task->finish_time / 60;
    int finish_minute = completed_task->finish_time % 60;
    
    printf(
        "  %02d:%02d-%02d:%02d | %-30.30s [ %s ]\n",
        start_hour, start_minute, finish_hour, finish_minute,
        completed_task->task->task_name, completed_task->task->category
    );

    int i = 30;
    while (i < strlen(completed_task->task->task_name)) {
        printf("      %.30s\n", (completed_task->task->task_name + i));
        i += 30;
    }
}

/**
 * Compares two tasks by precedence of category -> priority -> name and returns
 * an integer referring to their relative ordering
 * 
 * Parameters:
 *     t1 - The first task to compare
 *     t2 - The second task to compare
 *
 * Returns:
 *     a negative integer if t1 belongs before t2
 *     a positive integer if t1 belongs after t2
 *     0 if the tasks are identical (This should never happen in your program)
 */
int task_compare(struct task *t1, struct task *t2) {
    int category_diff = strcmp(t1->category, t2->category);
    if (category_diff != 0) {
        return category_diff;
    }
    
    int priority_diff = t2->priority - t1->priority;
    if (priority_diff != 0) {
        return priority_diff;
    }
    
    return strcmp(t1->task_name, t2->task_name);
}