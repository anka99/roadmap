#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "output.h"
#include "structures.h"

void executeError(int n) {
  fprintf(stderr, "ERROR %d\n", n);
}

char const *getRouteDescriptionOut(Route *route, unsigned routeId) {
  if (!route) {
    char *str = malloc(sizeof(char));
    if (!str) {
      return NULL;
    }
    str[0] = '\0';
    return str;
  }
  long descriptLength = snprintf(NULL, 0, "%u%c%s%c", routeId, ';',
                                 route->city1->name, ';');
  char *description = malloc((descriptLength + 1) * sizeof(char));
  const char *previousCity = route->city1->name;
  RoadList *temp = route->roads;
  snprintf(description, descriptLength + 1, "%u%c%s%c", routeId, ';',
           route->city1->name, ';');
  while (temp) {
    int fragmentLength = 0;
    const char *cityName = NULL;
    if (strcmp(temp->road->city1->name, previousCity) != 0) {
      cityName = temp->road->city1->name;
    }
    else {
      cityName = temp->road->city2->name;
    }
    fragmentLength = snprintf(NULL, 0, "%u%c%d%c%s%c", temp->road->length, ';',
                              temp->road->lastRepair, ';', cityName, ';');
    descriptLength += fragmentLength + 1;
    char *fragment = malloc(
            (descriptLength) * sizeof(char));
    snprintf(fragment, descriptLength, "%s%u%c%d%c%s%c", description,
             temp->road->length, ';', temp->road->lastRepair, ';', cityName, ';');
    free(description);
    description = malloc((fragmentLength + descriptLength + 1) * sizeof(char));
    strcpy(description, fragment);
    free(fragment);
    previousCity = cityName;
    temp = temp->next;
  }
  description[strlen(description) - 1] = '\0';
  return description;
}