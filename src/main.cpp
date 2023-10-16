#include "../include/shell.h"

int main() {
  Shell *shell = new Shell((char *)"./data/SSD.data");
  shell->run();
  return 0;
}