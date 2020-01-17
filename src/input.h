/** @file
 * Interfejs klasy odpowiadającej za wczytywanie poleceń
 *
 * @author Anna Kalisz <ak406173@studensts.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef DROGI_INPUT_H
#define DROGI_INPUT_H

#include <stdbool.h>

#define MEMORY_ERROR -2 ///< Kod oznaczający błąd alokacji pamięci
#define EOF_FOUND -3 ///< Kod oznaczający koniec pliku
#define IGNORE -4 ///< Kod oznaczający, że wczytaną linię należy ignorować

#define WRONG_COMMAND -1 ///< Kod oznaczający, że polecenie jest niepoprawne
#define GET_ROUTE 0 ///< Kod oznaczający polecenie getRoute
#define ADD_ROAD 1 ///< Kod oznaczający polecenie addRoad
#define REPAIR_ROAD 2 ///< Kod oznaczający polecenie repairRoad
#define GET_ROUTE_DESCR 3 ///< Kod oznaczający polecenie getRouteDescription
#define NEW_ROUTE 4 ///< Kod oznaczający polecenie newRoute
#define EXTEND_ROUTE 5 ///< Kod oznaczający polecenie extendRoute
#define REMOVE_ROAD 6 ///< Kod oznaczający polecenie removeRoad
#define REMOVE_ROUTE 7 ///< Kod oznaczający polecenie removeRoute

/**
 * Struktura przechowująca informacje o wczytanym poleceniu.
 */
typedef struct Command {
  int commandType; ///< Informacje na temat typu komendy lub ewentualnego błędu
  unsigned routeID; ///< Numer drogi krajowej
  char **cities; ///< Tablica wskaźników na nazwy miast
  unsigned *lengthArr; ///<Tablica długości odcików dróg
  int *lastRepairArr; ///<Tablica dat ostatnich remontów odcinków dróg
  int lastRepair; ///< Data ostatniego remontu odcinka drogi
  int citiesNumber; ///< Liczba miast w tablicy nazw miast
  char *city1; ///< Wskaźnik na nazwę pierwszego z miast
  char *city2; ///< Wskaźnik na nazwę drugiego z miast
  unsigned length; ///< Długość odcinka drogi
  char *strBeginning; ///<Wskaźnik na wczytaną linię wejścia
} Command;

/** @brief Czyta pojedynczą linię wejścia.
 * @return Zwraca strukturę Command zawierającą informację na temat rodzaju
 * komendy, błędu alokacji pamięci lub innego błędu.
 */
Command readLine();

/** @brief Sprawdza poprawność wczytanej komendy.
 * W zależności od jej rodzaju sprawdza, czy potrzebne do jej wykonania
 * parametry mają prawidłowe wartości.
 * @param command - wczytana komenda.
 * @return @p true - jeśli wszystkie potrzebne paramety były poprawne;
 * @p false - jeśli któryś był błędny.
 */
bool validCommand (Command command);

/** @brief Zwalnia pamięć zaalokowaną w strukturze command.
 * Zwalnia (jeśli zostały zaalokowane) pola: cities, lengthArr, lastRepairArr
 * oraz wskaźnik na wczytaną linię polecenia (str).
 * @param command - wczytana komenda.
 */
void deleteCommand(Command command);

#endif //DROGI_INPUT_H
