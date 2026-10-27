
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task
{
    int id;
    char title[100];
    int priority;
    char deadline[20];
    int completed;

    struct Task *next;
} Task;

typedef struct QueueNode
{
    int id;
    char title[100];
    int priority;
    char deadline[20];

    struct QueueNode *next;
} QueueNode;


typedef struct StackNode
{
    int id;
    char title[100];

    struct StackNode *next;
} StackNode;



Task *create_node(Task *head)
{
    Task *newnode, *temp;
    int id, duplicate;

    /* Check that the task ID is unique */
    do
    {
        duplicate = 0;

        printf("\nEnter Task ID: ");
        scanf("%d", &id);

        temp = head;

        while (temp != NULL)
        {
            if (temp->id == id)
            {
                duplicate = 1;
                printf("Task ID already exists! Enter a different ID.\n");
                break;
            }

            temp = temp->next;
        }

    } while (duplicate == 1);

    newnode = (Task *)malloc(sizeof(Task));

    if (newnode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newnode->id = id;

    printf("Enter Task Title: ");
    scanf(" %99[^\n]", newnode->title);

    do
    {
        printf("Enter Priority (1-High, 2-Medium, 3-Low): ");
        scanf("%d", &newnode->priority);

        if (newnode->priority < 1 || newnode->priority > 3)
            printf("Invalid priority! Enter 1, 2 or 3.\n");

    } while (newnode->priority < 1 || newnode->priority > 3);

    printf("Enter Deadline (DD/MM/YYYY): ");
    scanf(" %19s", newnode->deadline);

    newnode->completed = 0;
    newnode->next = NULL;

    return newnode;
}


Task *insert_end(Task *head)
{
    Task *newnode, *temp;

    newnode = create_node(head);

    if (head == NULL)
    {
        head = newnode;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newnode;
    }

    printf("\nTask added successfully!\n");

    return head;
}

void display(Task *head)
{
    Task *temp;

    if (head == NULL)
    {
        printf("\nNo tasks available!\n");
        return;
    }

    temp = head;

    printf("\n========== TASK LIST ==========\n");

    while (temp != NULL)
    {
        printf("\nTask ID: %d\n", temp->id);
        printf("Title: %s\n", temp->title);

        if (temp->priority == 1)
            printf("Priority: High\n");
        else if (temp->priority == 2)
            printf("Priority: Medium\n");
        else
            printf("Priority: Low\n");

        printf("Deadline: %s\n", temp->deadline);

        printf("Status: %s\n",
               temp->completed == 0 ? "Pending" : "Completed");

        temp = temp->next;
    }
}


Task *search_task(Task *head, int id)
{
    Task *temp;

    temp = head;

    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}


void search_and_display(Task *head)
{
    Task *temp;
    int id, found = 0;
    char title[100];

    printf("\nEnter Task ID: ");
    scanf("%d", &id);

    printf("Enter Exact Task Title: ");
    scanf(" %99[^\n]", title);

    temp = head;

    while (temp != NULL)
    {
        if (temp->id == id &&
            strcmp(temp->title, title) == 0)
        {
            printf("\nTask Found!\n");
            printf("Task ID: %d\n", temp->id);
            printf("Title: %s\n", temp->title);
            printf("Priority: %d\n", temp->priority);
            printf("Deadline: %s\n", temp->deadline);
            printf("Status: %s\n",
                   temp->completed == 0 ? "Pending" : "Completed");

            found = 1;
            break;
        }

        temp = temp->next;
    }

    if (found == 0)
        printf("\nNo task found with that ID and title!\n");
}


void update_task(Task *head)
{
    Task *temp;
    int id, choice;

    printf("\nEnter Task ID to update: ");
    scanf("%d", &id);

    temp = search_task(head, id);

    if (temp == NULL)
    {
        printf("\nTask not found!\n");
        return;
    }

    printf("\n1. Update Title\n");
    printf("2. Update Priority\n");
    printf("3. Update Deadline\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            printf("Enter New Title: ");
            scanf(" %99[^\n]", temp->title);
            break;

        case 2:
            do
            {
                printf("Enter New Priority (1-High, 2-Medium, 3-Low): ");
                scanf("%d", &temp->priority);

                if (temp->priority < 1 || temp->priority > 3)
                    printf("Invalid priority! Try again.\n");

            } while (temp->priority < 1 || temp->priority > 3);
            break;

        case 3:
            printf("Enter New Deadline (DD/MM/YYYY): ");
            scanf(" %19s", temp->deadline);
            break;

        default:
            printf("\nInvalid choice!\n");
            return;
    }

    printf("\nTask updated successfully!\n");
}


void save_tasks(Task *head)
{
    FILE *fp;
    Task *temp;

    fp = fopen("tasks.txt", "w");

    if (fp == NULL)
    {
        printf("\nError opening file for saving!\n");
        return;
    }

    temp = head;

    while (temp != NULL)
    {
        fprintf(fp, "%d\n", temp->id);
        fprintf(fp, "%s\n", temp->title);
        fprintf(fp, "%d\n", temp->priority);
        fprintf(fp, "%s\n", temp->deadline);
        fprintf(fp, "%d\n", temp->completed);

        temp = temp->next;
    }

    fclose(fp);
}


Task *delete_task(Task *head)
{
    Task *temp, *prev;
    int id;
    char title[100];

    if (head == NULL)
    {
        printf("\nNo tasks available!\n");
        return head;
    }

    printf("\nEnter Task ID to delete: ");
    scanf("%d", &id);

    printf("Enter Exact Task Title to delete: ");
    scanf(" %99[^\n]", title);

    temp = head;
    prev = NULL;

    while (temp != NULL)
    {
        if (temp->id == id &&
            strcmp(temp->title, title) == 0)
        {
            break;
        }

        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\nNo task found with that ID and title!\n");
        return head;
    }

    printf("\nTask selected for deletion:\n");
    printf("ID: %d\n", temp->id);
    printf("Title: %s\n", temp->title);

    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);

    printf("\nTask deleted successfully!\n");

    return head;
}


Task *load_tasks(Task *head)
{
    FILE *fp;
    Task *newnode, *temp;

    fp = fopen("tasks.txt", "r");

    if (fp == NULL)
        return head;

    while (1)
    {
        newnode = (Task *)malloc(sizeof(Task));

        if (newnode == NULL)
        {
            fclose(fp);
            printf("Memory allocation failed!\n");
            exit(1);
        }

        if (fscanf(fp, "%d\n", &newnode->id) != 1)
        {
            free(newnode);
            break;
        }

        if (fgets(newnode->title, sizeof(newnode->title), fp) == NULL ||
            fscanf(fp, "%d\n", &newnode->priority) != 1 ||
            fgets(newnode->deadline, sizeof(newnode->deadline), fp) == NULL ||
            fscanf(fp, "%d\n", &newnode->completed) != 1)
        {
            free(newnode);
            printf("Warning: Incomplete task data in file.\n");
            break;
        }

        newnode->title[strcspn(newnode->title, "\n")] = '\0';
        newnode->deadline[strcspn(newnode->deadline, "\n")] = '\0';
        newnode->next = NULL;

        if (head == NULL)
        {
            head = newnode;
        }
        else
        {
            temp = head;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newnode;
        }
    }

    fclose(fp);

    return head;
}

/* Insert a task into the priority queue */
QueueNode *enqueue(QueueNode *front, Task *task)
{
    QueueNode *newnode, *temp;

    newnode = (QueueNode *)malloc(sizeof(QueueNode));

    if (newnode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newnode->id = task->id;
    strcpy(newnode->title, task->title);
    newnode->priority = task->priority;
    strcpy(newnode->deadline, task->deadline);
    newnode->next = NULL;

    /* Insert at the front if it has higher priority */
    if (front == NULL || task->priority < front->priority)
    {
        newnode->next = front;
        front = newnode;
    }
    else
    {
        temp = front;

        /* Keep earlier tasks first when priorities are equal */
        while (temp->next != NULL &&
               temp->next->priority <= task->priority)
        {
            temp = temp->next;
        }

        newnode->next = temp->next;
        temp->next = newnode;
    }

    return front;
}

/* Build and display the priority schedule */
void display_schedule(Task *head)
{
    Task *temp;
    QueueNode *front = NULL;
    QueueNode *qtemp;

    temp = head;

    if (head == NULL)
    {
        printf("\nNo tasks available for scheduling!\n");
        return;
    }

    /* Add pending tasks to the priority queue */
    while (temp != NULL)
    {
        if (temp->completed == 0)
            front = enqueue(front, temp);

        temp = temp->next;
    }

    if (front == NULL)
    {
        printf("\nNo pending tasks to schedule!\n");
        return;
    }

    printf("\n====== PRIORITY SCHEDULE ======\n");

    qtemp = front;

    while (qtemp != NULL)
    {
        printf("\nTask ID: %d\n", qtemp->id);
        printf("Title: %s\n", qtemp->title);
        printf("Priority: %d\n", qtemp->priority);
        printf("Deadline: %s\n", qtemp->deadline);

        qtemp = qtemp->next;
    }

    /* Free the temporary queue nodes */
    while (front != NULL)
    {
        qtemp = front;
        front = front->next;
        free(qtemp);
    }
}


void task_statistics(Task *head)
{
    Task *temp;
    int total = 0;
    int completed = 0;
    int pending = 0;
    float percentage;

    temp = head;

    while (temp != NULL)
    {
        total++;

        if (temp->completed == 1)
            completed++;
        else
            pending++;

        temp = temp->next;
    }

    if (total == 0)
        percentage = 0;
    else
        percentage = (completed * 100.0f) / total;

    printf("\n====== TASK STATISTICS ======\n");
    printf("Total Tasks: %d\n", total);
    printf("Pending Tasks: %d\n", pending);
    printf("Completed Tasks: %d\n", completed);
    printf("Completion Percentage: %.2f%%\n", percentage);
}


/* Mark a task as completed and push it onto the stack */
void mark_completed(Task *head, StackNode **top)
{
    Task *temp;
    StackNode *newnode;
    int id;

    if (head == NULL)
    {
        printf("\nNo tasks available!\n");
        return;
    }

    printf("\nEnter Task ID to mark as completed: ");
    scanf("%d", &id);

    temp = head;

    while (temp != NULL && temp->id != id)
        temp = temp->next;

    if (temp == NULL)
    {
        printf("\nTask ID not found!\n");
        return;
    }

    if (temp->completed == 1)
    {
        printf("\nThis task is already completed!\n");
        return;
    }

    newnode = (StackNode *)malloc(sizeof(StackNode));

    if (newnode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newnode->id = temp->id;
    strcpy(newnode->title, temp->title);

    /* Push the new node onto the top of the stack */
    newnode->next = *top;
    *top = newnode;

    temp->completed = 1;

    printf("\nTask marked as completed!\n");
}

/* Display completed task history */
void display_history(StackNode *top)
{
    StackNode *temp;

    if (top == NULL)
    {
        printf("\nNo completed task history in this session!\n");
        return;
    }

    temp = top;

    printf("\n====== COMPLETED TASK HISTORY ======\n");

    while (temp != NULL)
    {
        printf("\nTask ID: %d\n", temp->id);
        printf("Title: %s\n", temp->title);

        temp = temp->next;
    }
}

/* Undo the most recent completion */
void undo_completion(Task *head, StackNode **top)
{
    StackNode *temp;
    Task *task;

    if (*top == NULL)
    {
        printf("\nNothing to undo!\n");
        return;
    }

    /* The top node is the last task completed */
    temp = *top;
    task = head;

    while (task != NULL && task->id != temp->id)
        task = task->next;

    if (task != NULL)
        task->completed = 0;

    printf("\nUndo successful!\n");
    printf("Task restored to pending: %s\n", temp->title);

    /* Pop the top node */
    *top = temp->next;
    free(temp);
}


int main()
{
    Task *head = NULL;
    StackNode *top = NULL;
    int choice;

    head = load_tasks(head);

    do
    {
        printf("\n====== SMART TO-DO LIST ======\n");
    printf("1. Add Task\n");
    printf("2. Display Tasks\n");
        printf("3. View Priority Schedule\n");
printf("4. Mark Task as Completed\n");
printf("5. View Completed History\n");
printf("6. Undo Last Completion\n");
printf("7. Search Task\n");
printf("8. Update Task\n");
printf("9. Delete Task\n");
printf("10. Task Statistics\n");
printf("11. Exit\n");
printf("Enter your choice: ");
scanf("%d", &choice);

        switch (choice)
{
    case 1:
        head = insert_end(head);
        save_tasks(head);
        break;

    case 2:
        display(head);
        break;

    case 3:
        display_schedule(head);
        break;

    case 4:
        mark_completed(head, &top);
        save_tasks(head);
        break;

    case 5:
        display_history(top);
        break;

    case 6:
        undo_completion(head, &top);
        save_tasks(head);
        break;
        
    case 7:
    search_and_display(head);
    break;

case 8:
    update_task(head);
    save_tasks(head);
    break;

case 9:
    head = delete_task(head);
    save_tasks(head);
    break;
case 10:
    task_statistics(head);
    break;

case 11:
        save_tasks(head);
        printf("\nExiting program...\n");
        break;

        default:
        printf("\nInvalid choice! Try again.\n");
}


    } while (choice != 10);

    return 0;
}

