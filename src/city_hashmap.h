/** @file
 * Interfejs klasy przechowującej mapę dróg krajowych
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef DROGI_CITY_HASHMAP_H
#define DROGI_CITY_HASHMAP_H

#include <stdbool.h>
#include "structures.h"

#define CITIES_NUMBER 6113 ///< Stała wyznaczająca długość tablicy w hashmapie.

/**
 * Struktura hashmapy miast.
 */
typedef struct CityHashMap CityHashMap;

/**@brief Tworzy nową strukturę.
 * Tworzy nową pustą strukturę niezawierającą żadnych miast.
 * @return Wskaźnik na utworzoną strukturę lub NULL jeśli nie udało się
 * zaalokować pamięci.
 */
CityHashMap *newCityHashMap();

/**@brief Dodaje miasto do hashmapy.
 * Tworzy nową struktuę miasta o podanej nazwie, alokując potrzebną pamięć.
 * Następnie umieszcza ją w hashmapie.
 * @param cityName - wskaźnik na nazwę miasta, które ma być dodane;
 * @param hashMap - wskaźnik na strukturę przechowującą hashmapę.
 * @return Zwraca wskaźnik na dodaną lub już istniejącą trukturę lub NULL jeśli
 * wystąpił błąd alokacji pamięci.
 */
City *addCity(const char *cityName, CityHashMap *hashMap);

/**@brief Szuka miasta w hashmapie.
 * Szuka miasta o podanej nazwie w hashmapie.
 * @param cityName - nazwa szukanego miasta;
 * @param hashMap - hashmapa, w której będzie szukane.
 * @return wskaźnik na strukturę City znajdującą się w hashmapie lub NULL, jeśli
 * miasto o podanej nazwie nie istnieje lub któryś z parametrów jest
 * nieprawidłowy.
 */
City *findCity(const char *cityName, CityHashMap *hashMap);

/**@brief Usuwa hashmapę.
 * Zwalnia całą zaalokowaną pamięć - miasta oraz łączące je drogi i samą
 * hashmapę.
 * @param cityHashMap - hashmapa, która ma zostać usunięta.
 */
void freeCityHashMap(CityHashMap *cityHashMap);

/**
 * Podaje liczbę miast w hashmapie.
 * @param hashMap - wskaźnik na strukturę hashmapy.
 * @return Zwraca liczbę miast w hashmapie.
 */
unsigned numberOfCities(CityHashMap *hashMap);

/**
 * Pokazuje miasta w hashmapie.
 * @param hashMap - wskaźnik na strutkturę hashmapy.
 * @return Wskaźnik na tablicę list miast zawartą w hashmapie.
 */
CityList **showCities(CityHashMap *hashMap);

#endif //DROGI_CITY_HASHMAP_H
