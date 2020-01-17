#include "map.h"
#include "input.h"
#include "execute.h"

int main(void) {
  Map *map = newMap();
  if (!map) {
    return 0;
  }
  int line = 1;
  Command command = readLine();
  while (command.commandType != EOF_FOUND) {
    if (!executeCommand(command, map, &line)) {
      deleteCommand(command);
      deleteMap(map);
      return 0;
    }
    deleteCommand(command);
    command = readLine();
  }
  deleteMap(map);
  return 0;
}