#include <stdio.h>
#include <unistd.h>

// So apparently c used to have the "implicit int" rule: type int is assumed if no type is specified in variable declarations. c99 removed this. gcc throws a warning.

void main() {
  static var_name = 5;

  printf("This can't really work: %d\n", var_name);
}
