#ifndef DROGI_PRIORITY_QUEUE_H
#define DROGI_PRIORITY_QUEUE_H

#include <stdbool.h>

///Stała zwracana przy próbie usunięcia pierwszego indeksu z pustej kolejki.
#define EMPTY -1

/**
 * Struktura przechowująca kolejkę priorytetową.
 */
typedef struct PriorityQueue PriorityQueue;

/**@brief Tworzy nową strukturę.
 * Tworzy nową, pustą kolejkę priorytetową.
 * @return Zwraca wskaźnik na utworzoną strukturę lub NULL, jeśli nie uda się
 * zaalokować pamięci.
 */
PriorityQueue *newPriorityQueue(unsigned maxIndex);

/**@brief Sprawdza, czy kolejka jest pusta.
 * @param queue - kolejka, którą sprawdza.
 * @return @p false, jeśli w kolejce znajduje się przynajmniej 1 element;
 * @p true w przeciwnym razie.
 */
bool isEmpty(PriorityQueue *queue);

/**@brief Dodaje element do kolejki.
 * Dodaje nowy element, tak aby został zachowany porządek kolejki.
 * @param index - element dodawany;
 * @param head - początek kolejki.
 * @return Zwraca @p false jeśli wystąpi błąd alokacji pamięci. W przeciwnym
 * wypadku zwraca @p true.
 */
bool insert(unsigned index, PriorityQueue *queue, int (*comparator)(unsigned,
        unsigned));

/**@brief Usuwa pierwszy element.
 * Jeśli kolejka nie jest pusta, usuwa z niej pierwszy element.
 * @param head - początek kolejki.
 * @return Zwraca wartość usuniętego indeksu lub EMPTY, jeśli kolejka była
 * pusta.
 */
int pop(PriorityQueue *queue);

/**@brief Usuwa strukturę.
 * Zwalnia całą zaalokowaną pamięć i usuwa kolejkę.
 */
void freePriorityQueue(PriorityQueue *queue);

/** Usuwa element kolejki.
 * Usuwa podaną wartość z kolejki priorytetowej. Jeśli w kolejce nie ma elementu
 * o podanej wartości - nic nie zmienia.
 * @param index - wartość do usunięcia;
 * @param queue - wskaźnik na strukturę kolejki.
 */
void deleteIndex(int index, PriorityQueue *queue);

#endif //DROGI_PRIORITY_QUEUE_H
