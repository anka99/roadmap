#ifndef DROGI_OUTPUT_H
#define DROGI_OUTPUT_H

#include "structures.h"

/**
 * Wypisuje na standardowe wyjście diagnostyczne jednoliniowy komunikat ERROR n,
 * gdzie n jest numerem linii w danych wejściowych zawierającym to polecenie.
 * Linie numerowane są od jedynki (ignorowane linie są uwzględniane).
 * @param n - numer linii wejścia.
 */
void executeError(int n);

/**
 * Tworzy napis zawierający informacje o drodze krajowej.
 * @param allRoutes - wskaźnik na tablicę dróg krajowcyh;
 * @param routeId - numer drogi krajowej.
 * @return Zwraca NULL, jeśli nie udało się zaalokować pamięci. W przeciwnym
 * wypadku zwraca wskaźnik na napis spełniający zasady określone przy funkcji
 * getRouteDescription.
 */
char const *getRouteDescriptionOut(Route *allRoutes, unsigned routeId);

#endif //DROGI_OUTPUT_H
