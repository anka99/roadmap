/** @file
 * Interfejs klasy przechowującej hashmapę odcinków dróg.
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef DROGI_ROAD_HASHMAP_H
#define DROGI_ROAD_HASHMAP_H

#define ROADS_NUMBER 6113 ///< Długość tablicy odcinków dróg w hashmapie.

#include "structures.h"

/**
 * Struktura przechowująca hashmapę odcinków dróg.
 */
typedef struct RoadHashMap RoadHashMap;

/**@brief Szuka odcinka drogi w hashmapie.
 * Szuka odcinka drogi zawierającego wskaźnik na strukturę miasta o podanej
 * nazwie w hashmapie odcików dróg należącej do innego miasta.
 * @param cityName - wskaźnik na nazwę miasta;
 * @param hashMap - wskaźnik na strukturę przechowująca hashmapę odcinków dróg
 * @return Zwraca wskaźnik na znaleziony odcinek drogi lub NULL, jeśli taki
 * odcinek nie istnieje.
 */
Road *findRoad(const char *cityName, RoadHashMap *hashMap);

/**@brief Dodaje drogę do hashmapy.
 * Dodaje gotową drogę do hashmapy należącej do jednego z miast. Zakłada, że
 * miasto do którego należy hashmapa jest jednym z końców dodawanej drogi, a
 * drugie jest od niego różne.
 * @return Zwraca wskaźnik na dodaną drogę lub NULL w przypadku błędu alokacji
 * pamięci.
 */
Road *addRoadToHashmap(Road *road, RoadHashMap *hashMap);

/**@brief Usuwa odcinek drogi.
 * Usuwa odcinek drogi z hashmap obydwu miast, które są jej końcami.
 * @param road  - wskaźnik na odcinek drogi.
 */
void deleteRoad(Road *road);

/**@brief Usuwa strukturę.
 * Usuwa hashmapę zwalniając całą zaalokowaną pamięć.
 * @param roadHashMap - wskaźnik na strukturę przechowującą hashmapę odcinków
 * dróg.
 */
void freeRoadHashMap(RoadHashMap *roadHashMap);

/**@brief Tworzy nową strukturę.
 * Tworzy nową, pustą hashmapę odcików dróg. Alokuje potrzebną pamięć.
 * @param owner - wskaźnik na strukturę miasta, do którego ma należeć utworzona
 * hashmapa.
 * @return Zwraca wskaźnik na utworzoną struktruę lub NULL, jeśli nie uda się
 * zaalokować pamięci.
 */
RoadHashMap *newRoadHashMap(City *owner);

/**
 * Daje dostęp do listy odcinków dróg znajdującej się pod podanym indeksem.
 * @param index - indeks w tablicy list odcinków dróg;
 * @param hashMap - wskaźnik na strukturę hashmapy.
 * @return Zwraca wskaźnik na początek listy odcinków dróg znajdującej się pod
 * podanym indeksem.
 */
RoadList *showElement(int index, RoadHashMap *hashMap);

#endif //DROGI_ROAD_HASHMAP_H
