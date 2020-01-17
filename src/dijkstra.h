/** @file
 * Interfejs algorytmu służącego do wyznaczania i modyfikowania dróg
 * krajowych.
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 20.03.2019
 */

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "structures.h"
#include "city_hashmap.h"

/**@brief Szuka najlepszej drogi.
 * Dla podanych w paramertach miast szuka najlepszej możliwej drogi krajowej bez
 * samoprzecięć i pętli.
 * @param city1 - początek wyznaczanej drogi;
 * @param city2 - koniec drogi;
 * @param hashMap - hashmapa miast, między którymi poprowadzone są odcinki dróg;
 * @param forbiddenId - numer drogi krajowej, której odcinki nie mogą być
 * wykorzystane;
 * @param forbiddenRoad - wskaźnik na odcinek drogi, który nie może należeć do
 * szukanej drogi krajowej.
 * @return Zwraca NULL, jeśli wystąpi błąd alokacji pamięci, któryś z
 * parametrów jest niepoprawny lub nie można wyznaczyć drogi krajowej w sposób
 * jednoznaczny. W przeciwnym razie zwraca listę odcinków dróg tworzących
 * wyznaczoną drogę krajową.
 */
RoadList *findBestRoute(City *city1, City *city2, CityHashMap *hashMap,
        unsigned forbiddenId, Road *forbiddenRoad);

#endif //DROGI_DIJKSTRA_H
