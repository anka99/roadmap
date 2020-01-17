/**@file
 * Implementacja dijkstra.h.
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl>
 * @date 01.09.2019
 */
#include <stdlib.h>

#include "dijkstra.h"
#include "structures.h"
#include "priority_queue.h"
#include "road_hashmap.h"

#define INFINITY 0 ///< Stała traktowana jako nieskończenie duża.

/**
 * Struktura tablicy miast.
 */
typedef struct CitiesArray {
  City *city; ///< Wskaźnik na strukturę miasta.
  unsigned distance; ///< Odległość miasta od startu.
  int oldestRoad; ///< Data remontu najstarszego aktualnie odcinka drogi.
  bool checked; ///< Informacja, czy dane miasto zostało sprawdzone.
  unsigned previousCity; ///< Poprzednie maisto w ścieżce wyznaczonej od startu.
  Road *connection; ///< Wskaźnik na odcinek drogi łączący miasto z poprzednim.
} CitiesArray;

/**
 * Tablica, do której zostaną wpisane miasta pod odpowiednimi indeksami.
 */
static CitiesArray *citiesArray = NULL;

/**@brief Tworzy tablicę miast.
 * Alokuje potrzebną pamięć i przepisuje miasta z hashmapy do tablicy dodając
 * pola zawierające potrzebne informacje: odległość od startu, data budowy
 * najstarszego odcinka drogi, informację na temat tego, czy miasto zostało już
 * sprawdzone. Umieszcza miasto początkowe pod indeksem 0 a miasto końcowe pod
 * indeksem 1.
 * @param start - wskaźnik na strukturę przechowującą miasto początkowe;
 * @param finish - wskaźnik na strukturę przechowującą miasto końcowe;
 * @param hashMap - hashmapa zawierająca miasta.
 * @return Zwraca NULL, jeśli wystąpi błąd alokacji pamięci lub parametr jest
 * niepoprawny. W przeciwnym wypadku zwraca wypełnioną tablicę miast.
 */
static CitiesArray *newCitiesArray(City *start, City *finish,
        CityHashMap *hashMap) {
  if (numberOfCities(hashMap) == 0) {
    return NULL;
  }

  CityList **cities = showCities(hashMap);
  if (!cities) {
    return NULL;
  }

  CitiesArray *new = malloc(numberOfCities(hashMap) * sizeof(CitiesArray));
  if (!new) {
    return NULL;
  }

  unsigned counter = 2;
  for (int i = 2; i < CITIES_NUMBER; i++) {
    CityList *temp = cities[i];
    while (temp) {
      unsigned tempCounter = counter;
      if (isEqual(temp->city, start)) {
        counter = 0;
      }
      else if (isEqual(temp->city, finish)) {
        counter = 1;
      }
      new[counter].city = temp->city;
      new[counter].checked = false;
      new[counter].distance = INFINITY;
      new[counter].oldestRoad = INFINITY;
      new[counter].previousCity = 0;
      new[counter].connection = NULL;
      temp->city->citiesArrayIndex = counter;
      temp = temp->next;
      if (counter == tempCounter) {
        tempCounter++;
      }
      counter = tempCounter;
    }
  }
  return new;
}

/**@brief Porównuje miasta.
 * Porównuje miasta znajdujące się w tablicy miast pod wskazanymi indeksami.
 * @param index1 - indeks pierwszego miasta w talicy miast;
 * @param index2 - indeks drugiego miasta w tablicy miast.
 * @return Zwraca 1, jeśli droga prowadząca do miasta pod pierwszym indeksem od
 * miasta początkowego jest dłuższa niż do drugiego miasta. W przypadku tej
 * samej odległośći bierze pod uwagę datę remontu najstarszego odcinka drogi.
 * Jeśli nie można rozstrzygnąć, która z dróg jest dłuższa, zwraca 0. W
 * przeciwnym wypadku zwraca -1.
 */
static int comparator(unsigned index1, unsigned index2) {
  unsigned distance1 = citiesArray[index1].distance;
  unsigned distance2 = citiesArray[index2].distance;
  int oldestRoad1 = citiesArray[index1].oldestRoad;
  int oldestRoad2 = citiesArray[index2].oldestRoad;
  if (distance1 == distance2) {
    if (oldestRoad1 == oldestRoad2) {
      return 0;
    }
    if (oldestRoad1 < oldestRoad2) {
      return 1;
    }
    return -1;
  }
  if (distance1 > distance2) {
    return 1;
  }
  return -1;
}

/**
 * Wyznacza minimum dwóch liczb całkowitych, z uwzględnieniem stałej INFINITY
 * jako większej od każdej liczby całkowitej.
 * @param x - pierwsza liczba całkowita;
 * @param y - druga liczba całkowita.
 * @return Zwraca INFINITY, jeśli któraś z podanych liczb jest równa INFINITY.
 * W przeciwnym wypadku zwraca minimum z podanych liczb.
 */
static int dijkstraMin(int x, int y) {
  if (x == INFINITY) {
    return y;
  }
  if (y == INFINITY) {
    return x;
  }
  if (x < y) {
    return x;
  }
  return y;
}

/**
 * Porównuje 2 liczby dodatnie z uwzględnieniem stałej INFINITY.
 * @param distance1 - pierwsza liczba dodatnia;
 * @param distance2 - druga liczba dodatnia.
 * @return Zwraca 0, jeśli liczby są sobie równe. W przeciwnym wypadku, jeśli
 * pierwsza liczba jest równa INFINITY lub żadna z liczb nie jest równa INFINITY
 * a pierwsza liczba jest większa od drugiej - zwraca 1. W przeciwnym wypadku
 * zwraca -1.
 */
static int compareDistances(unsigned distance1, unsigned distance2) {
  if (distance1 == distance2) {
    return 0;
  }
  if (distance1 == INFINITY) {
    return 1;
  }
  if (distance2 == INFINITY) {
    return -1;
  }
  if (distance1 > distance2) {
    return 1;
  }
  return -1;
}

/**
 * Porównuje 2 komórki tablicy miast, biorąc pod uwagę w pierwszej kolejności
 * odległość, a w drugiej datę remonru najdawniej remontowanego odcinka drogi.
 * @param crate1 - pierwsza komórka tablicy miast;
 * @param crate2 - druga komórka tablicy miast.
 * @return Zwraca 1, jeśli pierwsza komórka zawiera informację o gorszej ścieżce
 * (dłuższej lub w przypadku równych odległości - zawierającej najstarszy
 * odcinek). Jeśli ścieżki są tak samo dobre, zwraca 0. W przeciwnym wypadku
 * zwraca -1.
 */
static int compareCrates(CitiesArray crate1, CitiesArray crate2) {
  if (compareDistances(crate1.distance, crate2.distance) == 1) {
    return 1;
  }
  if (compareDistances(crate1.distance, crate2.distance) == -1) {
    return -1;
  }
  if (crate1.oldestRoad < crate2.oldestRoad) {
    return 1;
  }
  if (crate1.oldestRoad > crate2.oldestRoad) {
    return -1;
  }
  return 0;
}

/**
 * Tworzy nową komórkę tablicy miast o podanym dystancie i najstarszym odcinku.
 * @param distance - odległość od startu;
 * @param oldestRoad - data remontu najdawniej remontowanego odcinka.
 * @return Zwraca utworzoną komórkę.
 */
static CitiesArray newCrate (unsigned distance, int oldestRoad) {
  CitiesArray new;
  new.oldestRoad = oldestRoad;
  new.distance = distance;
  return new;
}

#define TRUE 1 ///< Stała, oznaczająca że algorytm ma zakończyć działanie.
#define FALSE 0 ///< Stała, oznaczająca że algorytm ma nie kończyć działania.
#define ERROR -1 ///< Stała, oznaczająca że wystąpił błąd alokacji pamięci.

/**@brief Kolejny krok algorytmu Dijkstry.
 * Funkcja usuwa z kolejki priorytetowej jej pierwszy element. Następnie dla
 * każdego miasta połączonego z miastem o usuniętym indeksie dodaje je do
 * kolejki, jeśli nie zostało wcześniej sprawdzone i jeśli nie przechodzi przez
 * nie wyznaczana droga krajowa.
 * @param queue - wskaźnik na strukturę kolejki priorytetowej;
 * @param forbiddenId - numer wyznaczanej drogi krajowej;
 * @param explicit - zmienna logiczna przyjmująca wartość @p true, jeśli droga
 * krajowa jest wyznaczona jednoznacznie, a @p false, jeśli nie jest.
 * @param forbiddenRoad  - numer wyznaczanej drogi krajowej.
 * @return Zwraca TRUE, jeśli została znaleziona droga krajowa; FALSE, jeśli
 * algorym ma kontynuować szukanie lub ERROR, jeśli wystąpił błąd alokacji
 * pamięci.
 */
static int checkCity(PriorityQueue *queue, unsigned forbiddenId, bool *explicit,
        Road *forbiddenRoad) {
  if (isEmpty(queue)) {
    return ERROR;
  }
  int top = pop(queue);
  if (top == 1) {
    return TRUE;
  }
  City *city = citiesArray[top].city;
  for (int i = 0; i < ROADS_NUMBER; i++) {
    RoadList *temp = showElement(i, city->roads);
    while (temp) {
      Road *road = temp->road;
      if (road != forbiddenRoad) {
        unsigned index;
        if (!isEqual(city, road->city1)) {
          index = road->city1->citiesArrayIndex;
        }
        else {
          index = road->city2->citiesArrayIndex;
        }
        if (!(citiesArray[index].city->routesPassing[forbiddenId]) &&
           !(citiesArray[index].checked)) {
          unsigned newDistance = citiesArray[top].distance + road->length;
          int newOldest = dijkstraMin(citiesArray[top].oldestRoad,
                  road->lastRepair);
          int compare = compareCrates(newCrate(newDistance, newOldest),
                  citiesArray[index]);
          if (compare < 1) {
            if (index == 1) {
              if (compare == 0) {
                *explicit = false;
              }
              else {
                *explicit = true;
              }
            }
            deleteIndex((int) index, queue);
            citiesArray[index].connection = road;
            citiesArray[index].previousCity = city->citiesArrayIndex;
            (citiesArray[index].distance) = newDistance;
            citiesArray[index].oldestRoad = newOldest;
            if (!insert(index, queue, comparator)) {
              return ERROR;
            }
          }
        }
      }
      temp = temp->next;
    }
  }
  citiesArray[top].checked = true;
  return FALSE;
}

/**@brief Usuwa strukturę.
 * Usuwa tablicę miast zwalniając zaalokowaną na nią pamięć.
 * @param array - tablica miast.
 */
static void freeCitiesArray(CitiesArray *array) {
  if (array) {
    free(array);
  }
}

/**
 * Korzystając z globalnej tablicy miast oddtwarza listę odcinków dróg, z
 * z których składa się wyznaczona droga krajowa.
 * @return Zwraca utworzoną listę odcinków dróg lub NULL, jeśli wystąpi błąd
 * alokacji pamięci.
 */
static RoadList *recoverRoadList() {
  RoadList *list = NULL;
  unsigned start = 1;
  while (start != 0) {
    if (!(addToRoadList(citiesArray[start].connection, &list))) {
      freeRoadList(list);
      return NULL;
    }
    start = citiesArray[start].previousCity;
  }
  return list;
}

RoadList *findBestRoute(City *start, City *finish, CityHashMap *hashMap,
                        unsigned forbiddenId, Road *forbiddenRoad) {
  RoadList *roadList = NULL;
  PriorityQueue *queue = newPriorityQueue(numberOfCities(hashMap));
  if (!queue) {
    return NULL;
  }

  citiesArray = newCitiesArray(start, finish, hashMap);
  if (!citiesArray) {
    freePriorityQueue(queue);
    return NULL;
  }

  insert(0, queue, comparator);
  bool explicit = true;
  int end = FALSE;
  while (end != TRUE) {
    end = checkCity(queue, forbiddenId, &explicit, forbiddenRoad);
    if (end == ERROR) {
      freePriorityQueue(queue);
      freeCitiesArray(citiesArray);
      return NULL;
    }
  }
  if (!explicit) {
    freePriorityQueue(queue);
    freeCitiesArray(citiesArray);
    return NULL;
  }
  roadList = recoverRoadList();
  freePriorityQueue(queue);
  freeCitiesArray(citiesArray);
  return roadList;
}