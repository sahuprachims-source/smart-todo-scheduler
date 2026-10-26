#include <stdio.h>
#include "history.h"

/* Index of the entry with this ID, or -1 if not present. */
static int findHistoryIndex(const History *history, int id)
{
    int i;
    for (i = 0; i <= history->top; i++) {
        if (history->items[i].id == id) {
            return i;
        }
    }
    return -1;
}

void initHistory(History *history)
{
    history->top = -1;
}

int isStackEmpty(const History *history)
{
    return history->top == -1;
}

int isStackFull(const History *history)
{
    return history->top == MAX_CAPACITY - 1;
}

/* PUSH a copy of a completed task on top of the stack. */
int pushCompleted(History *history, const Task *task)
{
    if (task == NULL || task->completed != STATUS_COMPLETED) {
        return HISTORY_INVALID;           /* only completed tasks allowed */
    }
    if (findHistoryIndex(history, task->id) != -1) {
        return HISTORY_DUPLICATE;
    }
    if (isStackFull(history)) {
        return HISTORY_FULL;
    }
    history->top++;
    history->items[history->top] = *task;
    return HISTORY_OK;
}

/* POP the top task into *result. Returns 1 on success, 0 if empty. */
int popCompleted(History *history, Task *result)
{
    if (isStackEmpty(history)) {
        return 0;
    }
    if (result != NULL) {
        *result = history->items[history->top];
    }
    history->top--;
    return 1;
}

/* PEEK: look at the top task without removing it. Returns 1/0. */
int peekCompleted(const History *history, Task *result)
{
    if (isStackEmpty(history)) {
        return 0;
    }
    *result = history->items[history->top];
    return 1;
}

/* Remove the entry with this ID from ANYWHERE in the stack.
   Entries above it are shifted down one place, so the order is preserved. */
int removeTaskFromHistory(History *history, int id)
{
    int index = findHistoryIndex(history, id);
    int i;

    if (index == -1) {
        return 0;
    }
    for (i = index; i < history->top; i++) {
        history->items[i] = history->items[i + 1];
    }
    history->top--;
    return 1;
}

/* Refresh the stored copy after the task was edited. Returns 1/0. */
int updateTaskInHistory(History *history, const Task *updatedTask)
{
    int index;

    if (updatedTask == NULL) {
        return 0;
    }
    index = findHistoryIndex(history, updatedTask->id);
    if (index == -1) {
        return 0;
    }
    history->items[index] = *updatedTask;
    return 1;
}

/* Show the most recently completed task first (top down to bottom). */
void displayCompletedHistory(const History *history)
{
    int i;

    if (isStackEmpty(history)) {
        printf("The completed-task history is empty.\n");
        return;
    }
    printf("Completed tasks (most recent first):\n");
    printTaskHeader();
    for (i = history->top; i >= 0; i--) {
        printTaskRow(&history->items[i]);
    }
}
