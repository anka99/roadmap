/** @file
 * Implementacja input.h
 *
 * @author Anna Kalisz <ak406173@studensts.mimuw.edu.pl>
 * @date 01.09.2019
 */
#ifdef _STDC_ALLOC_LIB_
#define _STDC_WANT_LIB_EXT2_ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#define ADD_ROAD_TEXT "addRoad" ///< Pierwszy wyraz polecenia addRoad
#define REPAIR_TEXT "repairRoad" ///< Pierwszy wyraz polecenia repairRoad

///Początek polecenia getRouteDescription
#define GET_DESCR_TEXT "getRouteDescription"
#define NEW_ROUTE_TEXT "newRoute" ///< Początek polecenia newRoute
#define EXTEND_ROUTE_TEXT "extendRoute" ///< Początek polecenia extendRoute
#define REMOVE_ROAD_TEXT "removeRoad" ///< Początek polecenia removeRoad
#define REMOVE_ROUTE_TEXT "removeRoute" ///< Początek polecenia removeRoute

#define BASE 10 ///< System, w którym zapisane są odczytywane liczby

/**@brief Tworzy nową strukturę.
 * Tworzy strukturę pustej komendy.
 * @return Zwraca utworzoną strukturę.
 */
Command initCommand() {
  Command new;
  new.commandType = WRONG_COMMAND;
  new.lastRepair = 0;
  new.lengthArr = NULL;
  new.cities = NULL;
  new.city1 = NULL;
  new.city2 = NULL;
  new.lastRepairArr = NULL;
  new.length = 0;
  new.lastRepairArr = 0;
  new.citiesNumber = 0;
  new.strBeginning = NULL;
  return new;
}

void deleteCommand(Command command) {
  if (command.cities) {
    free(command.cities);
  }
  if (command.lastRepairArr) {
    free(command.lastRepairArr);
  }
  if (command.lengthArr) {
    free(command.lengthArr);
  }
  if (command.strBeginning) {
    free(command.strBeginning);
  }
}

/** @brief Liczy miasta w komendzie.
 * Liczy potencjalne nazwy miast w linii wejścia.
 * @param str  - wskaźnik na linię wejścia;
 * @param size  - długość linii wejścia.
 * @return Zwraca liczbę potencjalnych miast.
 */
static int citiesNumber(char *str, size_t size) {
  int semicolon = 0;
  for (size_t i = 0; i < size; i++) {
    if (str[i] == '\0') {
      semicolon++;
    }
  }
  return (semicolon - 1) / 3 + 1;
}

///Czyta liczbę całkowitą. W przypadku błędu zwraca 0.
static int readInt(char *str) {
  if ((str[0] < '0' || str[0] > '9') && str[0] != '-') {
    return 0;
  }
  char *endptr = NULL;
  int64_t bigNumber = strtol(str, &endptr, BASE);
  if (bigNumber > INT_MAX || bigNumber < INT_MIN) {
    return 0;
  }
  if (endptr && strcmp(endptr, "") != 0 && strcmp(endptr, "\n") != 0) {
    return 0;
  }
  int value = bigNumber;
  return value;
}

///Czyta liczbę nieujemną. W przypadku błędu zwraca 0.
static unsigned readUnsigned(char *str) {
  if (str[0] < '0' || str[0] > '9') {
    return 0;
  }
  char *endptr = NULL;
  int64_t bigNumber = strtol(str, &endptr, BASE);
  if (bigNumber > UINT32_MAX || bigNumber < 0) {
    return 0;
  }
  if (endptr && strcmp(endptr, "") != 0 && strcmp(endptr, "\n") != 0) {
    return 0;
  }
  unsigned value = bigNumber;
  return value;
}

///Przenosi wskaźnik na następny napis we wczytanej linii wejścia.
static int movePointer(char *str, size_t *i, size_t size) {
  size_t actSize = strlen(str) + *i;
  while ((*i) < actSize) {
    (*i)++;
  }
  (*i)++;
  if ((*i) >= size) {
    return WRONG_COMMAND;
  }
  return ADD_ROAD;
}

///Wypełnia danymi strukturę komendy dla polecenia addRoad.
static int fillAddRoad(Command *command, char *str, size_t size) {
  size_t i = 0;
  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city1 = str + i;
  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city2 = str + i;
  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  if (!(str + i) || strcmp(str + i, "") == 0) {
    return WRONG_COMMAND;
  }

  command->length = readUnsigned(str + i);
  if (command->length <= 0) {
    return WRONG_COMMAND;
  }

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  if ((str + i) && strcmp(str + i, "\n") == 0) {
    return WRONG_COMMAND;
  }

  command->lastRepair = readInt(str + i);
  if (command->lastRepair == 0) {
    return WRONG_COMMAND;
  }

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return ADD_ROAD;
}

///Wypełnia danymi strukturę komendy dla polecenia getRouteDescription.
static int fillRouteDescr(Command *command, char *str, size_t size) {
  size_t i = 0;

  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }

  command->routeID = readUnsigned(str + i);
  if (command->routeID == 0) {
    return WRONG_COMMAND;
  }

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }

  return GET_ROUTE_DESCR;
}

///Wypełnia danymi strukturę komendy dla polecenia repairRoad.
static int fillRepairRoad(Command *command, char *str, size_t size) {
  size_t i = 0;

  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city1 = str + i;

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city2 = str + i;

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }

  command->lastRepair = readInt(str + i);
  if (command->lastRepair == 0) {
    return WRONG_COMMAND;
  }

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return REPAIR_ROAD;
}

/**@brief Alokuje pamięć w komendzie.
 * Alokuje pamięć potrzebną do wywołania manualnego tworzenia nowej drogi
 * krajowej.
 * @param command - struktura komendy.
 * @param number - liczba miast w tablicy.
 * @return Zwraca @p true, jeśli alokacja pamięci się powiodła, a @p false jeśli
 * nie.
 */
static bool allocateMemory(Command *command, int number) {
  if (!command->cities) {
    command->cities = malloc(number * sizeof(char*));
  }
  if (!command->lengthArr) {
    command->lengthArr = malloc((number - 1) * sizeof(unsigned));
  }

  if (!command->lastRepairArr) {
    command->lastRepairArr = malloc((number - 1) * sizeof(int));
  }

  if (!(command->cities) || !(command->lengthArr) ||
     !(command->lastRepairArr)) {
    return false;
  }
  for (int i = 0; i < number; i++) {
    command->cities[i] = NULL;
  }
  return true;
}

/// Wypełnia danymi strukturę komendy dla polecenia oznaczającego manualne dodanie drogi krajowej.
static int fillGetRoute(Command *command, char *str, size_t size) {
  command->routeID = readUnsigned(str);
  if (command->routeID == 0) {
    return WRONG_COMMAND;
  }

  command->citiesNumber = citiesNumber(str, size);
  if (command->citiesNumber < 2) {
    return WRONG_COMMAND;
  }

  if (!allocateMemory(command, command->citiesNumber)) {
    return MEMORY_ERROR;
  }

  int counter = 0;
  size_t i = 0;
  while (counter < command->citiesNumber - 1) {
    if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
      return WRONG_COMMAND;
    }
    command->cities[counter] = str + i;

    if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
      return WRONG_COMMAND;
    }

    command->lengthArr[counter] = readUnsigned(str + i);
    if (command->lengthArr[counter] == 0) {
      return WRONG_COMMAND;
    }

    if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
      return WRONG_COMMAND;
    }

    command->lastRepairArr[counter] = readInt(str +i);
    if (command->lastRepairArr[counter] == 0) {
      return WRONG_COMMAND;
    }

    counter++;
  }
  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->cities[counter] = str + i;
  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  str[i - 2] = '\0';
  return GET_ROUTE;
}

/// Wypełnia danymi strukturę komendy dla polecenia newRoute.
static int fillNewRoute(Command *command, char *str, size_t size) {
  size_t i = 0;
  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }

  command->routeID = readUnsigned(str + i);
  if (command->routeID == 0) {
    return WRONG_COMMAND;
  }

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city1 = str + i;

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city2 = str + i;

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return NEW_ROUTE;
}

/// Wypełnia danymi strukturę komendy dla polecenia fillGetRoute.
static int fillExtendRoute(Command *command, char *str, size_t size) {
  size_t i = 0;

  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }

  command->routeID = readUnsigned(str + i);
  if (command->routeID == 0) {
    return WRONG_COMMAND;
  }

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city1 = str + i;

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return EXTEND_ROUTE;
}

/// Wypełnia danymi strukturę komendy dla polecenia removeRoad.
static int fillRemoveRoad(Command *command, char *str, size_t size) {
  size_t i = 0;
  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city1 = str + i;

  if (movePointer(str + i, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->city2 = str + i;

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return REMOVE_ROAD;
}

/// Wypełnia danymi strukturę komendy dla polecenia removeRoute.
static int fillRemoveRoute(Command *command, char *str, size_t size) {
  size_t i = 0;
  if (movePointer(str, &i, size) == WRONG_COMMAND) {
    return WRONG_COMMAND;
  }
  command->routeID = readUnsigned(str + i);
  if (command->routeID == 0) {
    return WRONG_COMMAND;
  }

  movePointer(str + i, &i, size);
  if (i != size + 1) {
    return WRONG_COMMAND;
  }
  return REMOVE_ROUTE;
}

/**@brief Rozpoznaje typ komendy i wypełnia jej strukturę danymi.
 * W zależności od początku linii wejścia wypełnia odpowiednie pola struktury
 * Command odpowiednimi danymi.
 * @param command - struktura komendy;
 * @param str - wskaźnik na wczytaną linię wejścia;
 * @param size - długość linii wejścia.
 * @return Zwraca wartość WRONG_COMMAND, jeśli polecenie miało niewłaściwy
 * format, MEMORY_ERROR jeśli wystąpił błąd alokacji pamięci i odpowiednią stałą
 * ze zbioru {GET_ROUTE, ADD_ROAD, REPAIR_ROAD, GET_ROUTE_DESCR, NEW_ROUTE,
 * EXTEND_ROUTE, REMOVE_ROAD, REMOVE_ROUTE} (w zależności od rozpoznanego typu
 * komendy).
 */
static int identifyCommand(Command *command, char *str, size_t size) {
  if (size < 1) {
    return WRONG_COMMAND;
  }
  else if (str[0] >= '0' && str[0] <= '9') {
    return fillGetRoute(command, str, size);
  }
  else if (strstr(str, ADD_ROAD_TEXT) == str) {
    return fillAddRoad(command, str, size);
  }
  else if (strstr(str, GET_DESCR_TEXT) == str) {
    return fillRouteDescr(command, str, size);
  }
  else if (strstr(str, REPAIR_TEXT) == str) {
    return fillRepairRoad(command, str, size);
  }
  else if (strstr(str, NEW_ROUTE_TEXT) == str) {
    return fillNewRoute(command, str, size);
  }
  else if (strstr(str, EXTEND_ROUTE_TEXT) == str) {
    return fillExtendRoute(command, str, size);
  }
  else if (strstr(str, REMOVE_ROAD_TEXT) == str) {
    return fillRemoveRoad(command, str, size);
  }
  else if (strstr(str, REMOVE_ROUTE_TEXT) == str) {
    return fillRemoveRoute(command, str, size);
  }
  else {
    command->commandType = WRONG_COMMAND;
    return WRONG_COMMAND;
  }
}

/**@brief Dzieli linię wejścia.
 * Dzieli linię wejścia na linię osobnych napisów zastępując znaki ';' znakami
 * '\0'.
 * @param str - wskaźnik na niepustą linię wejścia.
 */
static void editInput(char *str) {
  size_t i = 0;
  while (str[i] != '\0') {
    if (str[i] == ';') {
      str[i] = '\0';
    }
    i++;
  }
}

///Sprawdza, czy napis jest zakończony znakiem '\n'
static bool endline(char *str, size_t size) {
  size_t i = 0;
  while (i < size && str[i] != '\n') {
    i++;
  }
  return (i < size);
}

Command readLine() {
  char *str = NULL;
  size_t strSize = 0;
  Command command = initCommand();

  if (getline(&str, &strSize, stdin) == -1) {
    command.commandType = EOF_FOUND;
    if (str) free(str);
    return command;
  }
  command.strBeginning = str;
  if (!endline(str, strlen(str))) {
    return command;
  }
  if (str && (strcmp(str, "\n") == 0 || str[0] == '#')) {
    command.commandType = IGNORE;
    return command;
  }
  size_t size = strlen(str);
  if (str[size - 2] == ';') {
    command.commandType = WRONG_COMMAND;
    return command;
  }
  editInput(str);
  command.commandType = identifyCommand(&command, str, size);
  return command;
}

/**
 * Funkcja sprawdzająca poprawność wczytanej nazwy miasta.
 * @param cityName - wskaźnik na napis reprezentujący nazwę miasta.
 * @param size - długość napisu.
 * @return Zwraca @p true, jeśli nazwa miasta jest poprawna - nie zawiera znaków
 * o kodach pomiędzy 0 a 31 ani średnika. W przeciwnym wypadku zwraca @p false.
 */
static bool validCityName(char *cityName, unsigned size) {
  for (unsigned i = 0; i < size; i++) {
    if (!cityName) {
      return false;
    }
    if (cityName[i] == ';' || ((int) cityName[i] < 32 && (int) cityName[i] > -1)) {
      return false;
    }
  }
  if (strcmp(cityName, "") == 0) {
    return false;
  }
  return true;
}

///Sprawdza poprawność numeru drogi krajowej.
static bool validRouteId(unsigned routeId) {
  return (routeId > 0 && routeId <= 999);
}

///Sprawdza poprawność długości odcinka drogi.
static bool validLength(unsigned length) {
  return (length > 0);
}

///Sprawdza poprawność roku budowy/ostatniego remontu.
static bool validYear(int year) {
  return (year != 0);
}

///Zastępuje w napisie znak '\n' znakiem '\0'.
static void deleteEndline (char *str) {
  int i = 0;
  while (str[i] != '\0' && str[i] != '\n') {
    i++;
  }
  if (str[i] == '\n') str[i] = '\0';
}

bool validCommand (Command command) {
  int type = command.commandType;
  if (type == WRONG_COMMAND) {
    return false;
  }
  if (type == GET_ROUTE) {
    for (int i = 0; i < command.citiesNumber - 1; i++) {
      if (!validCityName(command.cities[i], strlen(command.cities[i]))) {
        return false;
      }
      if (!validLength(command.lengthArr[i])) {
        return false;
      }
      if (!validRouteId(command.routeID)) {
        return false;
      }
      if (!validYear(command.lastRepairArr[i])) {
        return false;
      }
      deleteEndline(command.cities[i]);
    }
  }
  if (type == ADD_ROAD || type == REPAIR_ROAD) {
    if (!validCityName(command.city1, strlen(command.city1))) {
      return false;
    }
    if (!validCityName(command.city2, strlen(command.city2))) {
      return false;
    }
    if (!validYear(command.lastRepair)) {
      return false;
    }
    deleteEndline(command.city1);
    deleteEndline(command.city2);
  }
  if (type == ADD_ROAD && !validLength(command.length)) {
    return false;
  }
  if (type == GET_ROUTE_DESCR && !validRouteId(command.routeID)) {
    return false;
  }
  if (type == NEW_ROUTE) {
    deleteEndline(command.city1);
    deleteEndline(command.city2);
    return (validRouteId(command.routeID) &&
    validCityName(command.city1, strlen(command.city1)) &&
    validCityName(command.city2, strlen(command.city2)));
  }
  if (type == EXTEND_ROUTE) {
    deleteEndline(command.city1);
    return (validRouteId(command.routeID) &&
    validCityName(command.city1, strlen(command.city1)));
  }
  if (type == REMOVE_ROAD) {
    deleteEndline(command.city1);
    deleteEndline(command.city2);
    return (validCityName(command.city1, strlen(command.city1)) &&
    validCityName(command.city2, strlen(command.city2)));
  }
  if (type == REMOVE_ROUTE) {
    return validRouteId(command.routeID);
  }
  return true;
}
