/** @file
 * Implementacja klasy city_hashmap.h
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include <stdlib.h>

#include "city_hashmap.h"
#include "structures.h"
#include "road_hashmap.h"
#include <string.h>

#define TABLE_LENGTH 10 ///< Długość tablicy hashującej.

/**
 * Struktura hashmapy miast.
 */
struct CityHashMap {
  unsigned length; ///< Długość tablicy list miast.
  CityList **cities; ///< Tablica przechowująca wskaźniki na początki list miast
  int *hashTable; ///< Tablica hashująca.
  unsigned numberOfCities; ///< Liczba miast w hashmapie.
};

/**@brief Tworzy tablicę.
 * Tworzy nową tablicę hashującą o długości zdefiniowanej stałą TABLE_LENGTH.
 * Wypełnia kolejne jej komórki kolejnymi liczbami pierwszymi.
 * @return Zwraca wskaźnik na utworzoną strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
static int *createHashTable() {
  int *array;
  if (!(array = malloc(TABLE_LENGTH * sizeof(int)))) {
    return NULL;
  }
  array[0] = 11;
  array[1] = 13;
  array[2] = 17;
  array[3] = 19;
  array[4] = 23;
  array[5] = 29;
  array[6] = 31;
  array[7] = 37;
  array[8] = 41;
  array[9] = 43;
  return array;
}

/**@brief Generuje indeks.
 * Na podstawie nazwy miasta i stałych CITIES_NUMBER, TABLE_LENGTH generuje nowy
 * indeks określający potencjalną pozycję miasta w hashmapie.
 * @param cityName - wskaźnik na napis będący nazwą miasta;
 * @param hashMap - wskaźnik na hashmapę.
 * @return Zwraca wygenerowany indeks lub -1, jeśli któryś z parametrów wskazuje
 * na NULL;
 */
static int generateIndex(const char *cityName, CityHashMap *hashMap) {
  if (!cityName || !hashMap) {
    return -1;
  }
  int index = 0;
  unsigned i = 0;
  unsigned length = strlen(cityName);
  while (i < length) {
    index += abs((int)(cityName[i] - '0') * hashMap->hashTable[i % TABLE_LENGTH]) %
            CITIES_NUMBER;
    index = abs (index % CITIES_NUMBER);
    i++;
  }
  return index;
}

CityHashMap *newCityHashMap() {
  CityHashMap *new;
  if (!(new = malloc(sizeof(CityHashMap)))) {
    return NULL;
  }
  new->length = CITIES_NUMBER;
  if (!(new->cities = malloc(CITIES_NUMBER * sizeof(CityList *)))) {
    free(new);
    return NULL;
  }
  for (int i = 0; i < CITIES_NUMBER; i++) {
    new->cities[i] = NULL;
  }
  if (!(new->hashTable = createHashTable())) {
    free(new->cities);
    free(new);
    return NULL;
  }
  new->numberOfCities = 0;
  return new;
}

City *findCity(const char *cityName, CityHashMap *hashMap) {
  int index = generateIndex(cityName, hashMap);
  if (index == -1) {
    return NULL;
  }
  CityList *temp = hashMap->cities[index];

  if (!temp) {
    return NULL;
  }
  while (temp != NULL && strcmp(temp->city->name, cityName) != 0) {
    temp = temp->next;
  }
  if (!temp) {
    return NULL;
  }
  return temp->city;
}

/**@brief Kopiuje napis.
 * Alokuje potrzebną pamięć i kopiuje podaną nazwę miasta.
 * @param cityName - napis reprezentujący nazwę miasta.
 * @return Zwraca skopiowany napis lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
static char *copyCityName(const char *cityName) {
  if (!cityName) {
    return NULL;
  }
  char *newName = malloc((strlen(cityName) + 1) * sizeof(char));
  if (!newName) {
    return false;
  }
  strcpy(newName, cityName);
  return newName;
}

/**@brief Tworzy nową strukturę.
 * Tworzy strutkurę miasta o podanej nazwie. Alokuje potrzebną pamięć. Nazwa
 * zostaje skopiowana i zapisana w strukturze.
 * @param name - napis reprezentujący nazwę miasta.
 * @return Zwraca utworzoną strukturę lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
static City *newCity(const char *name) {
  City *new = malloc(sizeof(City));
  if (!new) {
    return NULL;
  }
  new->name = copyCityName(name);
  if (!new->name) {
    free(new);
    return NULL;
  }
  new->roads = newRoadHashMap(new);
  new->index = 0;
  if (!(new->routesPassing = malloc(ROUTES_NUMBER * sizeof(bool)))) {
    free((void *) new->name);
    free(new);
    return NULL;
  }
  for (int i = 0; i < ROUTES_NUMBER; i++) {
    new->routesPassing[i] = false;
  }
  return new;
}

City *addCity(const char *cityName, CityHashMap *hashMap) {
  City *city = findCity(cityName, hashMap);
  if (city != NULL) {
    return city;
  }
  int index = generateIndex(cityName, hashMap);
  CityList *temp = hashMap->cities[index];
  CityList *newNode = newCityNode();
  if (!newNode) {
    return NULL;
  }
  city = newCity(cityName);
  if (!city) {
    free(newNode);
    return NULL;
  }
  city->index = index;
  newNode->city = city;
  if (!temp) {
    hashMap->cities[index] = newNode;
    (hashMap->numberOfCities)++;
    return newNode->city;
  }
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
  (hashMap->numberOfCities)++;
  return newNode->city;
}

/**@brief Usuwa strukturę.
 * Usuwa strukturę miasta zwalniając całą zaalokowaną pamięć.
 * @param city - wskaźnik na strukturę miasta.
 */
static void deleteCity(City *city) {
  if (city) {
    freeRoadHashMap(city->roads);
    if (city->name) {
      free((void *) city->name);
    }
    if (city->routesPassing) {
      free(city->routesPassing);
    }
    free(city);
  }
}

/**@brief Usuwa strukturę.
 * Usuwa listę miast razem z miastami, zwalniając całą zaalokowaną pamięć.
 * @param head - wskaźnik na początek listy.
 */
static void deleteCityList(CityList *head) {
  if (head) {
    if (head->city) {
      deleteCity(head->city);
    }
    deleteCityList(head->next);
    free(head);
  }
}

void freeCityHashMap(CityHashMap *cityHashMap) {
  if (cityHashMap) {
    if (cityHashMap->hashTable) {
      free(cityHashMap->hashTable);
    }
    if (cityHashMap->cities) {
      for (unsigned i = 0; i < cityHashMap->length; i++) {
        deleteCityList(cityHashMap->cities[i]);
      }
      free(cityHashMap->cities);
    }
    free(cityHashMap);
  }
}

unsigned numberOfCities(CityHashMap *hashMap) {
  if (hashMap) {
    return hashMap->numberOfCities;
  }
  return 0;
}

CityList **showCities(CityHashMap *hashMap) {
  if (!hashMap) {
    return NULL;
  }
  return hashMap->cities;
}