/** @file
 * Implementacja klasy przechowującej mapę dróg krajowych
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 20.03.2019
 */

#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "road_hashmap.h"
#include "dijkstra.h"
#include "output.h"

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
struct Map {
  CityHashMap *allCities; ///< Wskaźnik na hashmapę miast.
  Route **allRoutes; ///< Wskaźnik na tablicę dróg krajowych.
  RoadList *allRoads; ///< Wskaźnik na listę odcinków dróg.
};

Map *newMap(void) {
  Map *new = malloc(sizeof(Map));
  if (!new) {
    return NULL;
  }
  if (!(new->allCities = newCityHashMap())) {
    free(new);
    return NULL;
  }
  new->allRoutes = malloc(ROUTES_NUMBER * sizeof(Route *));
  if (!new->allRoutes) {
    freeCityHashMap(new->allCities);
    free(new);
    return NULL;
  }
  for (int i = 0; i < ROUTES_NUMBER; i++) {
    new->allRoutes[i] = NULL;
  }
  new->allRoads = NULL;
  return new;
}

void deleteMap(Map *map) {
  if (map) {
    if (map->allCities) {
      freeCityHashMap(map->allCities);
    }
    if (map->allRoutes) {
      for (int i = 0; i < ROUTES_NUMBER; i++) {
        deleteRoute(map->allRoutes[i]);
      }
      free(map->allRoutes);
    }
    if (map->allRoads) {
      freeRoads(map->allRoads);
    }
    free(map);
  }
}

/**
 * Sprawdza poprawność nazwy miasta.
 * @param cityName - wskaźnik na napis reprezentujący nazwę miasta.
 * @return Zwraca @p true, jeśli nazwa miasta jest poprawna - nie zawiera znaków
 * o kodach pomiędzy 0 a 31 ani średnika. W przeciwnym wypadku zwraca @p false.
 */
static bool validCityName(const char *cityName) {
  unsigned length = strlen(cityName);
  if (strcmp(cityName, "") == 0) {
    return false;
  }
  for (unsigned i = 0; i < length; i++) {
    if (((int) (cityName[i]) >= 0 && (int) (cityName[i]) <= 31 &&
      cityName[i] != '\n') || cityName[i] == ';') {
      return false;
    }
  }
  if (cityName[length] != '\0') {
    return false;
  }
  return true;
}

/**@brief Łączy miasta odcinkiem drogi.
 * Łączy podane miasta znajdujące się w hashmapie odcinkiem drogi. Zakłada
 * poprawność parametrów.
 * @param map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param city1 - wskaźnik na strukturę @p City dla pierszego miasta;
 * @param city2 - wskaźnik na strukturę @p City dla drugiego miasta;
 * @param length - długość odcinka drogi;
 * @param builtYear - rok budowy.
 * @return Zwraca @p false jeśli wystąpił błąd alokacji pamięci lub miasta są
 * już połączone odcinkiem drogi. W przeciwnym wypadku zwraca @p true.
 */
static bool connectCities(Map *map, City *city1, City *city2, unsigned length,
        int builtYear) {
  Road *road = newRoad(builtYear, length);
  if (!road) {
    return false;
  }
  road->city1 = city1;
  road->city2 = city2;
  if (!addRoadToHashmap(road, city1->roads) ||
      !addRoadToHashmap(road, city2->roads)) {
    return false;
  }
  return addToRoadList(road, &(map->allRoads));
}

/**
 * Sprawdza, czy w mapie istnieje odcinek drogi między dwoma miastami.
 * @param map - wskaźnik na struktruę przechowującą mapę dróg;
 * @param cityName1 - wskaźnik na napis reprezentujący nazwę pierwszego miasta;
 * @param cityName2 - wskaźnik na napis reprezentujący nazwę drugiego miasta.
 * @return Zwraca @p true, jeśli odcinek między podanymi miastami znajduje się
 * na mapie. W przeciwnym razie zwraca @p false.
 */
static bool isRoad(Map *map, const char *cityName1, const char *cityName2) {
  City *city1 = findCity(cityName1, map->allCities);
  City *city2 = findCity(cityName2, map->allCities);

  if (!city1 || !city2) {
    return false;
  }
  return findRoad(cityName1, city2->roads);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
  if (strcmp(city1, city2) == 0 ||
      (!validCityName(city1) || !validCityName(city2)) ||
      builtYear == 0 || length == 0) {
    return false;
  }

  if (isRoad(map, city1, city2)) {
    return false;
  }

  City *firstCity = addCity(city1, map->allCities);
  City *secondCity = addCity(city2, map->allCities);
  if (!firstCity || !secondCity) {
    return false;
  }
  return connectCities(map, firstCity, secondCity, length, builtYear);
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
  if (!map || !validCityName(city1) || !validCityName(city2) ||
      repairYear == 0) {
    return false;
  }
  City *first = findCity(city1, map->allCities);
  if (!first) {
    return false;
  }
  Road *road = findRoad(city2, first->roads);
  if (!road) {
    return NULL;
  }
  if (road->lastRepair > repairYear) {
    return false;
  }
  road->lastRepair = repairYear;
  return true;
}

/**
 * Sprawdza poprawność numeru drogi krajowej.
 * @param id - numer drogi krajowej
 * @return Zwraca @p true, jeśli numer drogi krajowej mieści się w zakresie od 1
 * do 999 włącznie. W przeciwnym razie zwraca @p false.
 */
static bool validRouteId(unsigned id) {
  if (id == 0 || id >= ROUTES_NUMBER) {
    return false;
  }
  return true;
}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
  if (!validRouteId(routeId) || !map || !validCityName(city1) ||
      !validCityName(city2)) {
    return false;
  }
  if (map->allRoutes[routeId]) {
    return false;
  }
  City *firstCity = findCity(city1, map->allCities);
  City *secondCity = findCity(city2, map->allCities);
  if (!firstCity || !secondCity || city1 == city2) {
    return false;
  }
  RoadList *roadList = findBestRoute(firstCity, secondCity, map->allCities, 0,
          NULL);
  if (!roadList) {
    return false;
  }
  Route *route = createRoute(roadList, firstCity, secondCity, routeId);
  if (!route) {
    return false;
  }
  map->allRoutes[routeId] = route;
  return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
  if (!map || !validRouteId(routeId) || !validCityName(city) ||
      !(map->allRoutes[routeId])) {
    return false;
  }

  City *newEnd = findCity(city, map->allCities);
  if (!newEnd || newEnd->routesPassing[routeId]) {
    return false;
  }
  RoadList *roadList = findBestRoute(map->allRoutes[routeId]->city2, newEnd,
          map->allCities, routeId, NULL);
  if (!roadList) {
    return false;
  }
  modifyRoute(roadList, newEnd, map->allRoutes[routeId]);
  return true;
}

/**Uzupełnia lukę w drodze krajowej.
 * Zastępuje w drodze krajowej wskazany odcinek drogi w najkrótszy możliwy
 * sposób, w drugiej kolejnośći biorąc pod uwagę datę remontu najdawniej
 * odnawianego odcinka drogi (analogicznie do funkcji newRoute()).
 * @param map - wskaźnik na mapę dróg;
 * @param route - wskaźnik na modyfikowaną drogę krajową;
 * @param brake - wskaźnik na odcinek drogi, przez który droga krajowa ma nie
 * przebiegać.
 * @return Zwraca @p false, jeśli wystąpił błąd alokacji pamięci lub jeśli nie
 * można jednoznacznie wyznaczyć nowego przebiegu drogi krajowej. W przeciwnym
 * wypadku zwraca @p true.
 */
static bool patchRoute(Map *map, Route *route, Road *brake) {
  RoadList *patch = NULL;
  brake->city1->routesPassing[route->rotueID] = false;
  brake->city2->routesPassing[route->rotueID] = false;
  RoadList *temp = route->roads;
  City *firstCity = brake->city2;
  City *secondCity = brake->city1;
  if (temp->road == brake) {
    if (isEqual(brake->city1, route->city1)) {
      firstCity = brake->city1;
      secondCity = brake->city2;
    }
    if (!(patch = findBestRoute(firstCity, secondCity, map->allCities,
                                route->rotueID, brake))) {
      brake->city1->routesPassing[route->rotueID] = true;
      brake->city2->routesPassing[route->rotueID] = true;
      return false;
    }
    route->roads = patch;
    RoadList *end = findEnd(patch);
    end->next = temp->next;
    route->city1 = firstCity;
    if (!(temp->next)) {
      route->city2 = secondCity;
    }
    free(temp);
  }
  else {
    while (temp && temp->next && temp->next->road != brake) {
      temp = temp->next;
    }
    if (isEqual(temp->road->city1, brake->city1) ||
        isEqual(temp->road->city2, brake->city1)) {
      firstCity = brake->city1;
      secondCity = brake->city2;
    }
    if (!(patch = findBestRoute(firstCity, secondCity, map->allCities,
                                route->rotueID, brake))) {
      brake->city1->routesPassing[route->rotueID] = true;
      brake->city2->routesPassing[route->rotueID] = true;
      return false;
    }
    if (!(temp->next->next)) {
      route->city2 = secondCity;
    }
    RoadList *temp2 = temp->next;
    temp->next = patch;
    RoadList *end = findEnd(patch);
    end->next = temp2->next;
    free(temp2);
  }
  brake->city1->routesPassing[route->rotueID] = true;
  brake->city2->routesPassing[route->rotueID] = true;
  Pair pair = lengthAndOldestRoad(patch, route->rotueID, route);
  (route->length) += pair.value1;
  route->oldestRoad += pair.value2;
  return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
  if (!map || !validCityName(city1) || !validCityName(city2)) {
    return false;
  }

  City *firstCity = findCity(city1, map->allCities);
  City *secondCity = findCity(city2, map->allCities);
  if (!firstCity || !secondCity) {
    return false;
  }

  Road *road = findRoad(city1, secondCity->roads);
  if (!road) {
    return false;
  }

  RouteList *temp1 = road->routes;
  while (temp1) {
    if (!(patchRoute(map, temp1->route, road))) {
      return false;
    }
    temp1 = temp1->next;
  }

  if (!cutRoad(map->allRoads, road)) {
    if (map->allRoads->road == road) {
      RoadList *temp2 = map->allRoads;
      map->allRoads = map->allRoads->next;
      free(temp2);
    }
  }
  deleteRoad(road);
  return true;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
  if (!map || !validRouteId(routeId)) {
    return NULL;
  }

  return getRouteDescriptionOut(map->allRoutes[routeId], routeId);
}

/**
 * Sprawdza, czy dla talblicy miast istnieją odcinki dróg między kolejnymi
 * miastami i czy nie konfliktują z podanymi długościami i datami napraw.
 * @param map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param cities - tablica napisów reprezentujących nazwy kolejnych miast;
 * @param lengths - tablica długości kolejnych odcinków dróg;
 * @param lastRepairs - tablica dat remontów kolejnych odcinków dróg;
 * @param citiesNumber - długość tablicy miast.
 * @return Zwraca @p false jeśli w tablicy nazw miast istnieją takie dwa pod
 * indeksami "i", "i + 1", że w mapie dróg istnieje odcinek drogi między
 * miastami o tych nazwach i długość tego odcinka jest różna od wartości pod
 * i - tym indeksem w tablicy długości lub jego rok ostatniego remontu jest
 * późniejszy niż i - ty rok w tablicy remontów.
 */
static bool checkRoads(Map *map, char **cities, unsigned *lengths,
        int *lastRepairs, int citiesNumber) {
  for (int i = 0; i < citiesNumber - 1; i++) {
    City *city1 = findCity(cities[i], map->allCities);
    if (city1) {
      Road *road = findRoad(cities[i + 1], city1->roads);
      if (road && (road->length != lengths[i] ||
         road->lastRepair > lastRepairs[i])) {
        return false;
      }
    }
  }

  return true;
}

/**
 * Tworzy z tablic miast, długości i dat remontów listę odcinków dróg. Jeśli
 * jakieś miasto lub odcinek drogi nie istnieje, to go tworzy. Jeśli odcinek
 * drogi już istnieje, ale ma wcześniejszy rok budowy lub ostatniego remontu,
 * to modyfikuje ten atrybut odcinka drogi. Jeśli odcinek drogi już istnieje,
 * ale ma inną długość albo późniejszy rok budowy lub ostatniego remontu zwalnia
 * listę i kończy działanie. Zmiany dokonane w mapie pozostają nawet w przypadku
 * błędu.
 * @param map - wskaźnik na strukturę przechowującą mapę dróg i miast;
 * @param cities - tablica napisów reprezentujących dodawane miasta;
 * @param lengths - tablica długości kolejnych odcików drogi;
 * @param lastRepairs - tablica dat ostatnich remontów odcinków drogi;
 * @param citiesNumber - liczba miast w tablicy miast.
 * @return Zwraca NULL, jeśli wystąpił błąd - odcinek drogi już istniał, ale
 * miał inną długość albo późniejszy rok budowy lub ostatniego remontu lub jeśli
 * nie udało się zaalokować pamięci. W przeciwnym razie zwraca listę odcików
 * dróg.
 */
static RoadList *recoverRoadList(Map *map, char **cities, unsigned *lengths,
                                 int *lastRepairs, int citiesNumber) {
  if (!checkRoads(map, cities, lengths, lastRepairs, citiesNumber)) {
    return false;
  }

  RoadList *roadList = NULL;

  for (int i = 0; i < citiesNumber - 1; i++) {
    Road *road = NULL;
    City *city1 = NULL;

    if (!addRoad(map, cities[i], cities[i + 1], lengths[i], lastRepairs[i])
       && !repairRoad(map, cities[i], cities[i + 1], lastRepairs[i])) {
      city1 = findCity(cities[i], map->allCities);
      road = findRoad(cities[i + 1], city1->roads);
      if (!road) {
        freeRoadList(roadList);
        return NULL;
      }
    }

    city1 = findCity(cities[i], map->allCities);
    road = findRoad(cities[i + 1], city1->roads);
    addToRoadList(road, &roadList);
  }
  return roadList;
}

bool getRoute(Map *map, unsigned routeId,  char **cities, unsigned *lengths,
        int *lastRepairs, int citiesNumber) {
  if (!map || !validRouteId(routeId) || map->allRoutes[routeId]) {
    return false;
  }
  RoadList* roadList = recoverRoadList(map, cities, lengths, lastRepairs,
                                       citiesNumber);
  if (!roadList) {
    return false;
  }

  reverseRoadList(&roadList);

  City *firstCity = findCity(cities[0], map->allCities);
  City *secondCity = findCity(cities[citiesNumber - 1], map->allCities);
  Route *route = createRoute(roadList, firstCity, secondCity, routeId);
  if (!route) {
    return false;
  }

  map->allRoutes[routeId] = route;
  return true;
}

bool removeRoute(Map *map, unsigned routeId) {
  if (!map || !validRouteId(routeId) || !map->allRoutes[routeId]) {
    return false;
  }

  RoadList *temp = map->allRoutes[routeId]->roads;
  if (!temp) {
    return false;
  }

  while (temp) {
    deleteRouteFromRoad(temp->road, routeId, map->allRoutes[routeId]);
    temp = temp->next;
  }

  deleteRoute(map->allRoutes[routeId]);
  map->allRoutes[routeId] = NULL;
  return true;
}