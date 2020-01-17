#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"
#include "structures.h"

/**
 * Struktura przechowująca listę liczb całkowitych.
 */
typedef struct IntList {
  int index; ///< Liczba całkowita
  struct IntList *next; ///< Wskaźnik na następny element listy
} IntList;

/**
 * Struktura przechowująca kolejkę priorytetową.
 */
struct PriorityQueue {
  IntList *head; ///< Początek listy przechowującej liczby.
  unsigned maxIndex; ///< Największy możliwy indeks w kolejce.
};

PriorityQueue *newPriorityQueue(unsigned maxIndex) {
  PriorityQueue *new  = malloc(sizeof(PriorityQueue));
  if (!new) {
    return NULL;
  }
  new->head = NULL;
  new->maxIndex = maxIndex;
  return new;
}

bool isEmpty(PriorityQueue *queue) {
  return (!queue || !(queue->head));
}

/**
 * Usuwa pojedynczy wierzchołek listy zwalniając zaalokowaną na niego pamięć.
 * @param node - wskaźnik na usuwany wierzchołek.
 */
static void deleteNode(IntList *node) {
  if (node) {
    free(node);
  }
}

/**Tworzy nową strukturę pojedynczego wierzchołka listy liczb całkowitych.
 * @param index - liczba
 * @return Zwraca wskaźnik na utworzony wierzchołek lub NULL jeśli nie udało się
 * zaalokować pamięci.
 */
static IntList *newIntListNode(unsigned index) {
  IntList *new = malloc(sizeof(IntList));
  if (!new) {
    return NULL;
  }
  new->index = (int) index;
  new->next = NULL;
  return new;
}

bool insert(unsigned index, PriorityQueue *queue, int(*comparator)(unsigned,
                                                                   unsigned)) {
  if (!queue) {
    return false;
  }
  if (!(queue->head) || comparator(index, queue->head->index) <= 0) {
    IntList *temp = newIntListNode(index);
    if (!temp) {
      return false;
    }
    temp->next = queue->head;
    queue->head = temp;
    return true;
  }
  IntList *temp = queue->head;
  IntList *prev = NULL;

  while (temp && comparator(temp->index, index) < 0) {
    prev = temp;
    temp = temp->next;
  }

  if (!prev) {
    IntList *newNode = newIntListNode(index);
    if (!newNode) {
      return false;
    }
    newNode->next = temp->next;
    temp->next = newNode;
    return true;
  }
  IntList *newNode = newIntListNode(index);
  if (!newNode) {
    return false;
  }
  newNode->next = prev->next;
  prev->next = newNode;
  return true;
}

int pop(PriorityQueue *queue) {
  if (queue && queue->head) {
    unsigned value = queue->head->index;
    IntList *temp = queue->head;
    queue->head = queue->head->next;
    deleteNode(temp);
    return (int) value;
  }
  return EMPTY;
}

///Zwalnia całą pamięć zaalokowaną na listę liczb całkowitych.
static void freeIntList(IntList *head) {
  if (head) {
    freeIntList(head->next);
    deleteNode(head);
  }
}

void freePriorityQueue(PriorityQueue *queue) {
  if (queue) {
    freeIntList(queue->head);
    free(queue);
  }
}

void deleteIndex(int index, PriorityQueue *queue) {
  if (queue && queue->head) {
    if (queue->head->index == index) {
      pop(queue);
    }
    else {
      IntList *head = queue->head;
      while (head && head->next) {
        if (head->next->index == index) {
          IntList *toRemoval = head->next;
          head->next = head->next->next;
          deleteNode(toRemoval);
        }
        head = head->next;
      }
    }
  }
}