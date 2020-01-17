/** @file
 * Implementacja structures.h
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl
 * @date 01.09.2019
 */

#include "structures.h"

#include <stdlib.h>
#include <string.h>

CityList *newCityNode() {
  CityList *new = malloc(sizeof(CityList));
  if (!new) {
    return NULL;
  }
  new->city = NULL;
  new->next = NULL;
  return new;
}

void deleteRoadList(RoadList *head) {
  if (head) {
    deleteRoadList(head->next);
    free(head);
  }
}

void freeRoadList(RoadList *head) {
  if (head) {
    freeRoadList(head->next);
    free(head);
  }
}

void freeRoad(Road *road) {
  if (road)  {
    if (road->routes) {
      deleteRouteList(road->routes);
    }
    free(road);
  }
}

void freeRoads(RoadList *head) {
  if (head) {
    freeRoad(head->road);
    freeRoads(head->next);
    free(head);
  }
}

void deleteRoute(Route *route) {
  if (route) {
    deleteRoadList(route->roads);
    free(route);
  }
}

/**@brief Tworzy nową strukturę.
 * Alokuje pamięć na nową strukturę wierzchołka listy odcinków dróg
 * zawierającego wskaźnik na podany odcinek drogi.
 * @param road - wskaźnik na strukturę odcinka drogi.
 * @return Zwraca wskaźnik na utworzoną strukturę lub NULL w przypadku błędu
 * alokacji pamięci.
 */
static RoadList *newRoadNode(Road *road) {
  RoadList *new = malloc(sizeof(RoadList));
  if (!new) {
    return NULL;
  }
  new->road = road;
  new->next = NULL;
  return new;
}

bool addToRoadList(Road *road, RoadList **head) {
  RoadList *new;
  if (!(new = newRoadNode(road))) {
    return false;
  }
  new->next = *head;
  *head = new;
  return true;
}

Road *newRoad(int year, unsigned length) {
  Road *new = malloc(sizeof(Road));
  if (!new) {
    return NULL;
  }
  new->lastRepair = year;
  new->length = length;
  new->city1 = NULL;
  new->city2 = NULL;
  new->previous1 = NULL;
  new->previous2 = NULL;
  new->routes = NULL;
  return new;
}

bool isEqual(City *city1, City *city2) {
  return (city1 == city2);
}

///Funkcja zwracająca wartość minimum dwóch liczb całkowitych.
static int min(int x, int y) {
  if (x < y) {
    return x;
  }
  return y;
}

Pair lengthAndOldestRoad(RoadList *roads, unsigned routeId, Route *route) {
  RoadList *temp = roads;
  unsigned distance = 0;
  int oldestRoad = temp->road->lastRepair;
  while (temp) {
    oldestRoad = min(oldestRoad, temp->road->lastRepair);
    distance += temp->road->length;
    addRouteToList(route, &(temp->road->routes));
    temp->road->city1->routesPassing[routeId] = true;
    temp->road->city2->routesPassing[routeId] = true;
    temp = temp->next;
  }
  Pair pair;
  pair.value1 = distance;
  pair.value2 = oldestRoad;
  return pair;
}

Route *createRoute(RoadList *roads, City *start, City *finish, unsigned routeId) {
  Route *route = malloc(sizeof(Route));
  if (!route) {
    return NULL;
  }
  route->roads = roads;
  route->city1 = start;
  route->city2 = finish;
  Pair pair = lengthAndOldestRoad(roads, routeId, route);
  route->length = pair.value1;
  route->oldestRoad = pair.value2;
  route->rotueID = routeId;
  return route;
}

RoadList *findEnd(RoadList *head) {
  RoadList *temp = head;
  while (temp && temp->next) {
    temp = temp->next;
  }
  return temp;
}

void modifyRoute(RoadList *newRoads, City *newEnd, Route *route) {
  RoadList *end = findEnd(route->roads);
  end->next = newRoads;
  route->city2 = newEnd;
  Pair pair = lengthAndOldestRoad(newRoads, route->rotueID, route);
  route->oldestRoad = pair.value2;
  route->length = pair.value1;
}

/**@brief Szuka poprzedniego elementu list.
 * Szuka element listy poprzedzający element zawierający podany odcinek drogi.
 * @param head - początek listy, w której znajduje się szukany element;
 * @param road - odcinek drogi.
 * @return Zwraca NULL, jeśli podany odcinek drogi jest pierwszy na liście lub
 * lista nie zawiera podanego odcinka. W przeciwnym razie zwraca wskaźnik na
 * element poprzedzający element z odcinkiem.
 */
static RoadList *findPreviousRoadListElement(RoadList *head, Road* road) {
  RoadList *previous = head;
  if (!previous || !previous->next) {
    return NULL;
  }
  while (previous && previous->next && previous->next->road != road) {
    previous = previous->next;
  }
  if (!(previous->next)) {
    return NULL;
  }
  return previous;
}

bool cutRoad(RoadList *head, Road *road) {
  RoadList *previous = findPreviousRoadListElement(head, road);
  if (!previous) {
    return false;
  }
  RoadList *temp = previous->next;
  previous->next = temp->next;
  free(temp);
  return true;
}

void deleteRouteFromRoad(Road *road, unsigned routeId, Route *route) {
  removeRouteFromList(route, &(road->routes));
  road->city1->routesPassing[routeId] = false;
  road->city2->routesPassing[routeId] = false;
}

void reverseRoadList(RoadList **head) {
  RoadList *front = *head;
  RoadList *mid = *head;
  RoadList *back = NULL;
  while (mid) {
    front = mid->next;
    mid->next = back;
    back = mid;
    mid = front;
  }
  *head = back;
}

RouteList *newRouteNode(Route *route) {
  RouteList *new = malloc(sizeof(RouteList));
  if (!new) {
    return NULL;
  }
  new->route = route;
  new->next = NULL;
  return new;
}

bool addRouteToList(Route *route, RouteList **head) {
  RouteList *newHead = newRouteNode(route);
  if (!newHead) {
    return false;
  }
  newHead->next = *head;
  *head = newHead;
  return true;
}

void deleteRouteList(RouteList *head) {
  if (head) {
    deleteRouteList(head->next);
    free(head);
  }
}

void removeRouteFromList(Route *route, RouteList **head) {
  if ((*head)->route == route) {
    RouteList *temp = (*head)->next;
    free(*head);
    *head = temp;
  }
  else {
    RouteList *prev = *head;
    RouteList *temp = (*head)->next;
    while (temp) {
      if (temp->route == route) {
        prev->next = temp->next;
        free(temp);
        break;
      }
      temp = temp->next;
      prev = temp;
    }
  }
}