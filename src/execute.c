/**@file
 * Implementacja execute.h
 *
 * @author Anna Kalisz <ak406173@studensts.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include <stdio.h>
#include <malloc.h>

#include "execute.h"
#include "input.h"
#include "map.h"
#include "output.h"

#define MEMORY -1 ///< Wartość oznaczająca błąd alokacji pamięci.
#define YES 1 ///< Wartość oznaczająca potwierdzenie.
#define NO 0 ///< Wartość oznaczająca zaprzeczenie.

/**@brief Sprawdza, czy w tablicy napisów nie występują powtórzenia.
 * Tworzy hashmapę miast i dodaje do niej miata o nazwach reprezentowanych przez
 * napisy w tablicy napisów. Za każdym razem sprawdza, czy miasto o danej nazwie
 * nie zostało dodane wcześniej. Przed zakończeniem działana usuwa hashmapę
 * zwalniając zaalokowaną pamięć.
 * @param cities - wskaźnik na tablicę przechowującą nazwy miast;
 * @param citiesNumber - liczba napisów w tablicy nazw miast.
 * @return Zwraca wartość MEMORY jeśli przy tworzeniu hashmapy lub dodawania do
 * niej elementu wystąpił błąd alokacji pamięci. W przeciwnym wypadku zwraca
 * YES jeśli w tablicy istnieje nazwa, która się powtarza, a NO jeśli taka nazwa
 * nie istnieje.
 */
static int duplicateCities(char **cities, int citiesNumber) {
  CityHashMap *hashMap = newCityHashMap();
  if (!hashMap) {
    return MEMORY;
  }
  for (int i = 0; i < citiesNumber; i++) {
    if (findCity(cities[i], hashMap)) {
      freeCityHashMap(hashMap);
      return YES;
    }
    if (!addCity(cities[i], hashMap)) {
      freeCityHashMap(hashMap);
      return MEMORY;
    }
  }
  freeCityHashMap(hashMap);
  return NO;
}

bool executeCommand(Command command, Map *map, int *line) {
  if (command.commandType == MEMORY_ERROR) {
    return false;
  }
  if (command.commandType == WRONG_COMMAND || !(validCommand(command))) {
    executeError(*line);
  }
  else if (command.commandType == ADD_ROAD){
    if (!addRoad(map, command.city1, command.city2, command.length,
            command.lastRepair)) {
      executeError(*line);
    }
  }
  else if (command.commandType == REPAIR_ROAD) {
    if (!repairRoad(map, command.city1, command.city2, command.lastRepair)) {
      executeError(*line);
    }
  }
  else if (command.commandType == GET_ROUTE) {
    if (duplicateCities(command.cities, command.citiesNumber) == YES) {
      executeError(*line);
    }
    else if (!getRoute(map, command.routeID, command.cities, command.lengthArr,
            command.lastRepairArr, command.citiesNumber)) {
      executeError(*line);
    }
  }
  else if (command.commandType == GET_ROUTE_DESCR) {
    char const *str = getRouteDescription(map, command.routeID);
    if (!str) {
      executeError(*line);
    }
    printf("%s\n", str);
    free((void *) str);
  }
  else if (command.commandType == NEW_ROUTE) {
    if (!newRoute(map, command.routeID, command.city1, command.city2)) {
      executeError(*line);
    }
  }
  else if (command.commandType == EXTEND_ROUTE) {
    if (!extendRoute(map, command.routeID, command.city1)) {
      executeError(*line);
    }
  }
  else if (command.commandType == REMOVE_ROAD) {
    if (!removeRoad(map, command.city1, command.city2)) {
      executeError(*line);
    }
  }
  else if (command.commandType == REMOVE_ROUTE) {
    if (!removeRoute(map, command.routeID)) {
      executeError(*line);
    }
  }
  (*line)++;
  return true;
}
