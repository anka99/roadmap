/** @file
 * Interfejs klasy przechowującej implementację struktur.
 *
 * @author Anna Kalisz <ak406173@students.mimuw.edu.pl
 * @date 01.09.2019
 */

#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

#include <stdbool.h>

///Maksymalna ilość dróg krajowych plus jeden (0 jest niepoprawnym numerem).
#define ROUTES_NUMBER 1000

/**
 * Struktura przechowująca odcinek drogi.
 */
typedef struct Road Road;

/**
 * Struktura przechowująca miasto.
 */
typedef struct City City;

/**
 * Struktura przechowująca drogę krajową.
 */
typedef struct Route Route;

/**
 * Struktura przechowująca hashmapę odcinków dróg.
 */
typedef struct RoadHashMap RoadHashMap;

/**
 * Struktura listy odcinków dróg.
 */
typedef struct RoadList RoadList;

/**
 * Struktura listy miast.
 */
typedef struct CityList CityList;

/**
 * Struktura listy dróg krajowych.
 */
typedef struct RouteList RouteList;

/**
 * Struktura przechowująca parę liczb.
 */
typedef struct Pair {
  unsigned value1; ///< Liczba dodatnia.
  int value2; ///< Liczba całkowita.
} Pair;

/**
 * Struktura przechowująca odcinek drogi.
 */
struct Road {
  City *city1; ///< Wskaźnik na jedno z miast na końcu odcinka.
  unsigned index1; ///< Indeks w hashmapie pierwszego miasta.
  RoadList *previous1; ///< Poprzedni element w hashmapie.
  City *city2; ///< Wskaźnik na drugie z miast na końcu odcinka.
  unsigned index2; ///< Indeks w hashmapie drugiego miasta.
  RoadList *previous2; ///< Poprzedni element w hashmapie.
  unsigned length; ///< Długość odcinka.
  int lastRepair; ///< Data ostatniego remontu odcinka.
  RouteList *routes; ///< Lista dróg krajowych zawierających odcinek.
};

/**
 * Struktura przechowująca miasto.
 */
struct City {
  unsigned index; ///< Indeks miasta w hashmapie.
  const char *name; ///< Wskaźnik na napis reprezentujący nazwę miasta.
  RoadHashMap *roads; ///< Wskaźnik na hashmapę dróg wychodzących z miasta.

  ///Indeks w tablicy miast używanej przy wyznaczaniu drogi krajowej.
  unsigned citiesArrayIndex;
  bool *routesPassing; ///< Tablica dróg krajowych przechodzących przez miasto.
};

/**
 * Struktura przechowująca drogę krajową.
 */
struct Route {
  unsigned rotueID; ///< Numer drogi krajowej.
  unsigned length; ///< Długość drogi krajowej.
  int oldestRoad; ///< Data remontu najstarszego odcinka.
  RoadList *roads; ///< Lista odcinków dróg, z których składa się droga krajowa.
  City *city1; ///< Początek drogi krajowej.
  City *city2; ///< Koniec drogi krajowej.
};

/**
 * Struktura listy miast.
 */
struct CityList {
  City *city; ///< Wskaźnik na strukturę miasta.
  struct CityList *next; ///< Następny element.
};

/**
 * Struktura listy odcinków dróg.
 */
struct RoadList {
  Road *road; ///< Wskaźnik na strukturę odcinka drogi.
  struct RoadList *next; ///< Następny element.
};

/**
 * Struktura przechowująca listę dróg krajowych.
 */
struct RouteList {
  Route *route; ///< Wskaźnik na strukturę drogi krajowej.
  RouteList *next; ///< Następny element.
};

/** @brief Tworzy nową strukturę.
 * Alokuje pamięć potrzebną do stworzenia pustego struktury CityList.
 * @return Zwraca wskaźnik zaalokowaną pamięć lub NULL w przypadku, gdy nie
 * udało się zaalokować pamięci.
 */
CityList *newCityNode();

/**@brief Usuwa drogę krajową.
 * Usuwa drogę krajową zwalniając zaalokowaną na nią pamięć. Nie rusza miast ani
 * odcinków dróg.
 * @param route
 */
void deleteRoute(Route *route);

/**@brief Dodaje element do listy.
 * Dodaje na początek listy dróg element zawierający odcinek drogi.
 * @param road - dodawany odcinek drogi;
 * @param head - początek listy, do której jest dodawany nowy odcinek.
 * @return - @p false, jeśli wystąpił błąd alokacji pamięci. W przeciwnym razie
 * @p true.
 */
bool addToRoadList (Road *road, RoadList **head);

/**@brief Usuwa listę odcinków dróg.
 * Zwalnia pamięć zaalokowaną na listę odcinków dróg. Nie zwalnia samych
 * odcinków.
 * @param head - początek listy.
 */
void deleteRoadList(RoadList *head);

/**@brief Tworzy nową strukturę.
 * Tworzy nową strukturę odcinka drogi o podanej długości i roku powstania.
 * @param year - rok powstania drogi;
 * @param length - długość odcinka drogi.
 * @return Zwraca wskaźnik na utworzoną strukturę lub NULL, jeśli wystąpi błąd
 * alokacji pamięci.
 */
Road *newRoad(int year, unsigned length);

///Zwalnia listę odcinków dróg razem z odcinkami.
void freeRoadList(RoadList *head);

///Usuwa strukturę odcinka drogi zwalniając zaalokowaną pamięć.
void freeRoad(Road *road);

/**@brief Porównuje 2 miasta
 * Sprawdza czy miasta są takie same poprzez porównanie ich nazw. Zakłada, że
 * istnieje co najwyżej jedno miasto o danej nazwie.
 * @param city1 - wskaźnik na pierwsze miasto;
 * @param city2 - wskaźnik na drugie miasto.
 * @return Zwraca wartość @p true, jeśli miasta mają takie same nazwy lub jesli
 * obydwa wskaźniki wskazują na NULL. W przeciwnym razie zwraca @p false.
 */
bool isEqual(City *city1, City *city2);

/**@brief Podaje długość i najstarszy odcinek.
 * Podaje łączną długość odcinków dróg w liście oraz datę ostatniego remontu
 * najstarszego odcinka. Zakłada poprawność parametrów. Ponadto oznacza każdy
 * odcinek oraz jego końce jako należące do drogi krajowej o podanym numerze.
 * @param roads - lista odcinków dróg krajowych;
 * @param routeId - numer drogi krajowej;
 * @param route - wskaźnik na drogę krajową.
 * @return Zwraca strukturę @p Pair zawierającą łączną długość oraz najstarszy
 * odcinek.
 */
Pair lengthAndOldestRoad(RoadList *roads, unsigned routeId, Route *route);

/**@brief Tworzy nową strukturę.
 * Tworzy nową strukturę drogi krajowej składającej się z podanych odcinków
 * dróg. Zakłada poprawność parametrów.
 * @param roads - lista odcinków dróg;
 * @param start - miasto początkowe;
 * @param finish - miasto końcowe;
 * @param routeId - numer drogi krajowej.
 * @return Zwraca NULL, jeśli nie powiodła się alokacja pamięci. W przeciwnym
 * razie zwraca wskaźnik na  utworzoną strukturę drogi krajowej.
 */
Route *createRoute(RoadList *roads, City *start, City *finish,
        unsigned routeId);

/**@brief Szuka końca listy.
 * Szuka końca listy odcinków dróg o podanym początku.
 * @param head - początek listy.
 * @return Zwraca NULL, jeśli lista jest pusta. W przeciwnym wypadku zwraca
 * wskaźnik na ostatni element listy.
 */
RoadList *findEnd(RoadList *head);

/**@brief Modyfikuje drogę krajową.
 * Modyfikuje istniejącą drogę krajową wydłużająć ją o podaną listą odcinków
 * dróg. Zakłada poprawność parametrów.
 * @param newRoads - lista odcinków dróg;
 * @param newEnd - miasto będące nowym końcem drogi krajowej;
 * @param route - modyfikowana droga.
 */
void modifyRoute(RoadList *newRoads, City *newEnd, Route *route);

/**Zwalnia pamięć, która zotała zaalokowana na każdy odcinek drogi znajdujący
 * się w liście. Następnie zwalnia samą listę.
 * @param head - wskaźnik na początek listy odcinków dróg.
 */
void freeRoads(RoadList *head);

/**@brief Usuwa odcinek drogi z listy dróg.
 * Usuwa podany odcinek jeśli lista dróg go zawiera i jeśli nie jest jej
 * pierwszym elementem.
 * @param head - wskaźnik na początek listy;
 * @param road - wskaźnik na usuwany odcinek drogi.
 * @return Zwraca @p true, jeśli odcinek udało się usunąć. Jeśli odcinek
 * znajduje się w pierwszym wierzchołku listy - zwraca @p false.
 */
bool cutRoad(RoadList *head, Road *road);

/** @brief Usuwa drogę krajową z odcinka drogi.
 * Wymazuje wszystkie informacje przechowywane w odcinku drogi i miastach na
 * jego końcach o przechodzącej przez nie drodze krajowej.
 * @param road - wskaźnik na odcinek drogi;
 * @param routeId - numer drogi krajowej;
 * @param route - wskaźnik na usuwaną drogę krajową.
 */
void deleteRouteFromRoad(Road *road, unsigned routeId, Route *route);

/** @brief Odwraca listę.
 * Odwraca kolejność listy odcików dróg i odpowiednio zmienia wskaźnik na jej
 * początek.
 * @param head - wskaźnik na początek listy odcinków dróg.
 */
void reverseRoadList(RoadList **head);

/**Tworzy nową strukturę.
 * Alokuje pamięć potrzebną do utworzenia nowego wierzchołka listy dróg
 * krajowych zawierającego wskaźnik na drogę krajową.
 * @param route - wskaźnik na drogę krajową.
 * @return Wskaźnik na utworzoną strukturę lub NULL jeśli nie udało się
 * zaalokować pamięci.
 */
RouteList *newRouteNode(Route *route);

/**@brief Dodaje element do listy.
 * Dodaje do początku listy dróg krajowych nowy element zawierający daną drogę
 * krajową.
 * @param route - wskaźnik na drogę krajową;
 * @param head - wskaźnik na wskaźnik na początek listy dróg krajowych.
 * @return Zwraca @p true, jeśli operacja się powiodła lub @p false, jeśli nie
 * udało się zaalokować pamięci.
 */
bool addRouteToList(Route *route, RouteList **head);

/**Usuwa strukturę.
 * Usuwa strukturę listy dróg krajowych i zwalnia całą zaalokowaną pamięć. Nie
 * usuwa dróg krajowych.
 * @param head - wskaźnik na początek listy dróg krajowych.
 */
void deleteRouteList(RouteList *head);

/**Usuwa element listy.
 * Usuwa z listy element zawierający wskaźnik na podaną drogę krajową i zwalnia
 * zaalokowaną na niego pamięć. Nie usuwa drogi krajowej. Jeśli usuwany element
 * znajduje się na początku listy, odpowiednio zmienia wskaźnik na początek.
 * @param route - wskaźnik na usuwaną z listy drogę krajową;
 * @param head - wskaźnik na wskaźnik na początek listy.
 */
void removeRouteFromList(Route *route, RouteList **head);

#endif //DROGI_STRUCTURES_H
