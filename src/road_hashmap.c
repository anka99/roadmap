/** @file
 * Implementacja klasy road_hashmap.h
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include "road_hashmap.h"
#include "structures.h"

#include <stdlib.h>
#include <string.h>

#define TABLE_LENGTH 10 ///< Długość tablicy hashującej.

/**
 * Struktura przechowująca hashmapę odcinków dróg.
 */
struct RoadHashMap {
  City *owner; ///< Miasto do którego należy hashmapa.
  unsigned length; ///< Długość tablicy wskaźników na listy odcinków dróg.
  RoadList **roads; ///< Tablica wskaźników na początki list odcików dróg.
  int *hashTable; ///< Tablica hashująca.
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
 * Na podstawie nazwy miasta i stałych ROADS_NUMBER, TABLE_LENGTH generuje nowy
 * indeks określający potencjalną pozycję miasta w hashmapie.
 * @param cityName - wskaźnik na napis będący nazwą miasta;
 * @param hashMap - wskaźnik na hashmapę.
 * @return Zwraca wygenerowany indeks lub -1, jeśli któryś z parametrów wskazuje
 * na NULL;
 */
static int generateIndex(const char *cityName, RoadHashMap *hashMap) {
  if (!cityName || !hashMap) {
    return -1;
  }
  int index = 0;
  unsigned i = 0;
  unsigned length = strlen(cityName);
  while (i < length) {
    index += abs((int)(cityName[i] - '0') * hashMap->hashTable[i % TABLE_LENGTH]) %
             ROADS_NUMBER;
    index = abs (index % ROADS_NUMBER);
    i++;
  }
  return index;
}

RoadHashMap *newRoadHashMap(City *owner) {
  RoadHashMap *new;
  if (!(new = malloc(sizeof(RoadHashMap)))) {
    return NULL;
  }
  new->length = ROADS_NUMBER;
  if (!(new->roads = malloc(ROADS_NUMBER * sizeof(RoadList *)))) {
    free(new);
    return NULL;
  }
  for (int i = 0; i < ROADS_NUMBER; i++) {
    new->roads[i] = NULL;
  }
  if (!(new->hashTable = createHashTable())) {
    free(new->roads);
    free(new);
    return NULL;
  }
  new->owner = owner;
  return new;
}

Road *findRoad(const char *cityName, RoadHashMap *hashMap) {
  if (!hashMap) {
    return NULL;
  }

  int index = generateIndex(cityName, hashMap);
  if (index == -1) {
    return NULL;
  }
  RoadList *temp = hashMap->roads[index];

  if (!temp) {
    return NULL;
  }
  while (temp != NULL && strcmp(temp->road->city1->name, cityName) != 0 &&
         strcmp(temp->road->city2->name, cityName) != 0) {
    temp = temp->next;
  }
  if (!temp) {
    return NULL;
  }
  return temp->road;
}

Road *addRoadToHashmap(Road *road, RoadHashMap *hashMap) {
  const char *name;
  if (strcmp(hashMap->owner->name, road->city1->name) == 0) {
    name = road->city2->name;
    Road *check = findRoad(name, hashMap);
    if (check != NULL) {
      return check;
    }
    road->index1 = generateIndex(name, hashMap);
    addToRoadList(road, &(hashMap->roads[road->index1]));
    return hashMap->roads[road->index1]->road;
  }
  else {
    name = road->city1->name;
    Road *check = findRoad(name, hashMap);
    if (check != NULL) {
      return check;
    }
    road->index2 = generateIndex(name, hashMap);
    addToRoadList(road, &(hashMap->roads[road->index2]));
    return hashMap->roads[road->index2]->road;
  }
}

void freeRoadHashMap(RoadHashMap *roadHashMap) {
  if (roadHashMap) {
    if (roadHashMap->hashTable) {
      free(roadHashMap->hashTable);
    }
    if (roadHashMap->roads) {
      for (unsigned i = 0; i < roadHashMap->length; i++) {
        deleteRoadList(roadHashMap->roads[i]);
      }
      free(roadHashMap->roads);
    }
    free(roadHashMap);
  }
}

/**@brief Wyszukuje poprzedni element listy w hashmapie.
 * @param cityName - nazwa pierwszego miasta.
 * @param hashMap - hashmapa dróg w drugim mieście.
 * @return Zwraca poprzedni element listy pod wygenerowanym dla nazwy miasta
 * indeksem. Jeśli droga o końcu cityName jest pierwsza na liście lub nie ma jej
 * w hashmapie - zwraca NULL.
 */
static RoadList *findPrevious(const char *cityName, RoadHashMap *hashMap) {
  RoadList *temp = hashMap->roads[generateIndex(cityName, hashMap)];
  if (!temp || !(temp->next)) {
    return NULL;
  }
  while (temp->next && strcmp(temp->road->city1->name, cityName) != 0 &&
        strcmp(temp->road->city2->name, cityName) != 0) {
    temp = temp->next;
  }
  if (temp->next == NULL) {
    return NULL;
  }
  return temp;
}

/**
 * Usuwa informacje o odcinku drogi z miasta. Zwalnia pamięć zaalokowaną na
 * wierzchołek listy przechowujący wskaźnik na dany odcinek drogi w hashmapie
 * odcinków dróg należącej do miasta.
 * @param city - wskaźnik na struktruę miasta;
 * @param previous - wskaźnik na wierzchołek listy poprzedzający usuwany;
 * @param index - indeks listy odcików dróg w hashmapie odcinków dróg.
 */
static void deleteHalf(City *city, RoadList *previous, unsigned index) {
  if (!previous) {
    RoadList *temp = city->roads->roads[index];
    city->roads->roads[index] = temp->next;
    free(temp);
  }
  else {
    RoadList *temp = previous->next;
    previous->next = temp->next;
    free(temp);
  }
}

void deleteRoad(Road *road) {
  if (road) {
    RoadList *previous1 = findPrevious(road->city2->name, road->city1->roads);
    RoadList *previous2 = findPrevious(road->city1->name, road->city2->roads);
    deleteHalf(road->city1, previous1, road->index1);
    deleteHalf(road->city2, previous2, road->index2);
    if (road->routes) {
      deleteRouteList(road->routes);
    }
    free(road);
  }
}

RoadList *showElement(int index, RoadHashMap *hashMap) {
  return hashMap->roads[index];
}