/** @file
 * Interfejs klasy odpowiadającej za wywołanie wczytanych poleceń
 *
 * @author Anna Kalisz <ak406173@studensts.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef DROGI_EXECUTE_H
#define DROGI_EXECUTE_H

#include "input.h"
#include "map.h"

/**@brief Wykonuje komendy.
 * Odpowiada za wywołanie funkcji odpowiadających wczytanym komendom i zliczanie
 * wczytanych linii wejścia.
 * @param command - wczytana komenda;
 * @param map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param line - liczba wczytanych dotąd linii wejścia.
 * @return Jeśli podczas wczytywania komendy nastąpił błąd alokacji pamięci,
 * zwraca @p false. W każdym innym wypadku zwraca @p true.
 */
bool executeCommand(Command command, Map *map, int *line);

#endif //DROGI_EXECUTE_H
